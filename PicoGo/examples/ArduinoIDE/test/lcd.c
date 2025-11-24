#include "lcd.h"

static bool lcd_initialized = false; // flag to indicate if the LCD is initialized

static uint8_t framebuffer[LCD_WIDTH * LCD_HEIGHT];
static uint16_t palette[256]; // 256-color palette for RGB332
static uint8_t backlight_level;

static FontTable *current_font = NULL;

static PIO _pio;
static uint _sm;

// Format: cmd length (including cmd byte), post delay in units of 5 ms, then cmd payload
// Note the delays have been shortened a little
static const uint8_t st7789_init_seq[] = {
    1, 20, 0x01,                        // Software reset
    1, 10, 0x11,                        // Exit sleep mode
    2, 2, 0x3a, 0x55,                   // Set colour mode to 16 bit
    2, 0, 0x36, 0x70,                   // Set MADCTL: RGB order, horizontal orientation
    5, 0, 0x2a, 0x00, 0x28, 0x01, 0x17, // CASET: column addresses
    5, 0, 0x2b, 0x00, 0x35, 0x00, 0xBB, // RASET: row addresses
    1, 2, 0x21,                         // Inversion on, then 10 ms delay
    1, 2, 0x13,                         // Normal display on, then 10 ms delay
    1, 2, 0x29,                         // Main screen turn on, then wait 500 ms
    0                                   // Terminate list
};

static void lcd_set_dc_cs(bool dc, bool cs)
{
    sleep_us(1);
    gpio_put_masked((1u << PIN_DC) | (1u << PIN_CS), !!dc << PIN_DC | !!cs << PIN_CS);
    sleep_us(1);
}

static void st7789_start_pixels(PIO pio, uint sm)
{
    uint8_t cmd = 0x2c; // RAMWR
    lcd_write_cmd(pio, sm, &cmd, 1);
    lcd_set_dc_cs(1, 0);
}

/******************************************************************************
 * function: Convert a 16-bit RGB565 color to an 8-bit RGB332 color
 * parameter:
 *    color : 16-bit RGB565 color value
 * returns: 8-bit RGB332 color value
 ******************************************************************************/
static uint8_t lcd_color565_to_332(uint16_t color)
{
    return ((color & 0xE000) >> 8) | ((color & 0x0700) >> 6) | ((color & 0x0018) >> 3);
}
/******************************************************************************
 * function: Convert 8-bit RGB332 components to a 16-bit RGB565 color
 * parameter:
 *    r : Red component (0-255)
 *    g : Green component (0-255)
 *    b : Blue component (0-255)
 * returns: 16-bit RGB565 color value
 ******************************************************************************/
static uint16_t lcd_color332_to_565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/******************************************************************************
function: Draw a single pixel to the framebuffer
parameter:
    X     : X coordinate (0 to LCD_WIDTH-1)
    Y     : Y coordinate (0 to LCD_HEIGHT-1)
    color : RGB565 color value
returns: none
******************************************************************************/
void lcd_draw_pixel(uint8_t x, uint8_t y, uint16_t color)
{
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
    {
        return; // bounds check
    }
    // Convert to 8-bit and store
    framebuffer[y * LCD_WIDTH + x] = lcd_color565_to_332(color);
}

/******************************************************************************
function: Draw a line between two points using Bresenham's algorithm
parameter:
    x1    : Starting X coordinate
    y1    : Starting Y coordinate
    x2    : Ending X coordinate
    y2    : Ending Y coordinate
    color : RGB565 color value
returns: none
******************************************************************************/
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int dx = abs((int)x2 - (int)x1);
    int dy = abs((int)y2 - (int)y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    const uint8_t color_index = lcd_color565_to_332(color);
    while (true)
    {
        // Draw pixel if within bounds
        if (x1 < LCD_WIDTH && y1 < LCD_HEIGHT)
        {
            framebuffer[y1 * LCD_WIDTH + x1] = color_index;
        }

        // Check if we've reached the end point
        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

/******************************************************************************
function: Draw a rectangle outline to the framebuffer
parameter:
    x      : Top-left X coordinate
    y      : Top-left Y coordinate
    width  : Width of rectangle
    height : Height of rectangle
    color  : RGB565 color value
returns: none
******************************************************************************/
void lcd_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    const uint8_t color_index = lcd_color565_to_332(color);
    // Draw four lines to form rectangle
    lcd_draw_line(x, y, x + width - 1, y, color_index);                           // Top
    lcd_draw_line(x, y + height - 1, x + width - 1, y + height - 1, color_index); // Bottom
    lcd_draw_line(x, y, x, y + height - 1, color_index);                          // Left
    lcd_draw_line(x + width - 1, y, x + width - 1, y + height - 1, color_index);  // Right
}

/******************************************************************************
function: Draw a filled rectangle to the framebuffer
parameter:
    x      : Top-left X coordinate
    y      : Top-left Y coordinate
    width  : Width of rectangle
    height : Height of rectangle
    color  : RGB565 color value
returns: none
******************************************************************************/
void lcd_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    // Bounds clipping
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
        return;

    if (x + width > LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT)
        height = LCD_HEIGHT - y;

    const uint8_t color_index = lcd_color565_to_332(color);

    // Fast fill using optimized loops
    for (uint16_t py = y; py < y + height; py++)
    {
        for (uint16_t px = x; px < x + width; px++)
        {
            framebuffer[py * LCD_WIDTH + px] = color_index;
        }
    }
}

/******************************************************************************
function: Draw a single character to the framebuffer
parameter:
    x     : Top-left X coordinate
    y     : Top-left Y coordinate
    c     : Character to draw
    color : RGB565 color value
returns: none
******************************************************************************/
void lcd_draw_char(uint16_t x, uint16_t y, char c, uint16_t color)
{
    if (current_font == NULL || c < 32 || c > 126)
        return; // invalid font or character

    // Calculate bytes per row (width rounded up to nearest byte boundary)
    uint8_t bytes_per_row = (current_font->width + 7) / 8;
    const uint8_t *char_data = &current_font->table[(c - 32) * current_font->height * bytes_per_row];

    for (uint8_t row = 0; row < current_font->height; row++)
    {
        const uint8_t *row_data = &char_data[row * bytes_per_row];

        for (uint8_t col = 0; col < current_font->width; col++)
        {
            uint8_t byte_index = col / 8;
            uint8_t bit_index = 7 - (col % 8);

            if (row_data[byte_index] & (1 << bit_index))
            {
                lcd_draw_pixel(x + col, y + row, color);
            }
        }
    }
}

/******************************************************************************
function: Draw a circle outline to the framebuffer
parameter:
    center_x : Center X coordinate
    center_y : Center Y coordinate
    radius   : Radius in pixels
    color    : RGB565 color value
returns: none
******************************************************************************/
void lcd_draw_circle(uint16_t center_x, uint16_t center_y, uint16_t radius, uint16_t color)
{
    if (radius == 0 || radius > 100)
        return;

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    const uint8_t color_index = lcd_color565_to_332(color);

    while (x <= y)
    {
        // Draw 8 symmetric points
        if (center_x + x < LCD_WIDTH && center_y + y < LCD_HEIGHT)
            framebuffer[(center_y + y) * LCD_WIDTH + (center_x + x)] = color_index;
        if (center_x - x < LCD_WIDTH && center_y + y < LCD_HEIGHT)
            framebuffer[(center_y + y) * LCD_WIDTH + (center_x - x)] = color_index;
        if (center_x + x < LCD_WIDTH && center_y - y < LCD_HEIGHT)
            framebuffer[(center_y - y) * LCD_WIDTH + (center_x + x)] = color_index;
        if (center_x - x < LCD_WIDTH && center_y - y < LCD_HEIGHT)
            framebuffer[(center_y - y) * LCD_WIDTH + (center_x - x)] = color_index;
        if (center_x + y < LCD_WIDTH && center_y + x < LCD_HEIGHT)
            framebuffer[(center_y + x) * LCD_WIDTH + (center_x + y)] = color_index;
        if (center_x - y < LCD_WIDTH && center_y + x < LCD_HEIGHT)
            framebuffer[(center_y + x) * LCD_WIDTH + (center_x - y)] = color_index;
        if (center_x + y < LCD_WIDTH && center_y - x < LCD_HEIGHT)
            framebuffer[(center_y - x) * LCD_WIDTH + (center_x + y)] = color_index;
        if (center_x - y < LCD_WIDTH && center_y - x < LCD_HEIGHT)
            framebuffer[(center_y - x) * LCD_WIDTH + (center_x - y)] = color_index;

        if (d < 0)
            d += 4 * x + 6;
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void lcd_draw_text(uint16_t x, uint16_t y, const char *text, uint16_t color)
{
    if (current_font == NULL)
        return; // invalid font

    uint16_t cursor_x = x;
    uint16_t cursor_y = y;

    while (*text)
    {
        char ch = *text;

        if (ch == '\n')
        {
            cursor_x = x;                     // Reset to start of line
            cursor_y += current_font->height; // Move down one line
        }
        else if (ch == ' ')
        {
            // Handle space - just advance position without drawing
            cursor_x += current_font->width;
        }
        else
        {
            // Check if character would exceed screen width
            if (cursor_x + current_font->width > LCD_WIDTH)
            {
                // Wrap to next line
                cursor_x = x;
                cursor_y += current_font->height;
            }

            // Check if we're still within screen height
            if (cursor_y + current_font->height <= LCD_HEIGHT)
            {
                lcd_draw_char(cursor_x, cursor_y, ch, color);
            }

            cursor_x += current_font->width;
        }
        text++;
    }
}

/******************************************************************************
function: Draw a filled circle to the framebuffer
parameter:
    center_x : Center X coordinate
    center_y : Center Y coordinate
    radius   : Radius in pixels
    color    : RGB565 color value
returns: none
******************************************************************************/
void lcd_fill_circle(uint16_t center_x, uint16_t center_y, uint16_t radius, uint16_t color)
{
    if (radius == 0 || radius > 100)
        return;

    const uint8_t color_index = lcd_color565_to_332(color);

    int radius_squared = radius * radius;

    // Calculate bounding box
    int start_x = (center_x > radius) ? (center_x - radius) : 0;
    int end_x = (center_x + radius < LCD_WIDTH) ? (center_x + radius) : (LCD_WIDTH - 1);
    int start_y = (center_y > radius) ? (center_y - radius) : 0;
    int end_y = (center_y + radius < LCD_HEIGHT) ? (center_y + radius) : (LCD_HEIGHT - 1);

    // Fill using distance check
    for (int y = start_y; y <= end_y; y++)
    {
        int dy = y - center_y;
        int dy_squared = dy * dy;

        for (int x = start_x; x <= end_x; x++)
        {
            int dx = x - center_x;
            int distance_squared = dx * dx + dy_squared;

            if (distance_squared <= radius_squared)
            {
                framebuffer[y * LCD_WIDTH + x] = color_index;
            }
        }
    }
}

/******************************************************************************
function: Draw a filled triangle to the framebuffer
parameter:
    x1, y1 : First vertex coordinates
    x2, y2 : Second vertex coordinates
    x3, y3 : Third vertex coordinates
    color  : RGB565 color value
returns: none
******************************************************************************/
void lcd_fill_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
    // Sort vertices by Y coordinate (y1 <= y2 <= y3)
    if (y1 > y2)
    {
        uint16_t temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    if (y2 > y3)
    {
        uint16_t temp = x2;
        x2 = x3;
        x3 = temp;
        temp = y2;
        y2 = y3;
        y3 = temp;
    }
    if (y1 > y2)
    {
        uint16_t temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    // Handle degenerate case
    if (y1 == y3)
        return;

    const uint8_t color_index = lcd_color565_to_332(color);

    // Fill the triangle using horizontal scanlines
    for (uint16_t y = y1; y <= y3; y++)
    {
        if (y >= LCD_HEIGHT)
            break;

        int x_left, x_right;
        bool has_intersection = false;

        // Find left edge intersection
        if (y3 != y1)
        {
            x_left = x1 + (int)((x3 - x1) * (int)(y - y1)) / (int)(y3 - y1);
            has_intersection = true;
        }

        // Find right edge intersection
        if (y <= y2 && y2 != y1)
        {
            x_right = x1 + (int)((x2 - x1) * (int)(y - y1)) / (int)(y2 - y1);
        }
        else if (y > y2 && y3 != y2)
        {
            x_right = x2 + (int)((x3 - x2) * (int)(y - y2)) / (int)(y3 - y2);
        }
        else
        {
            x_right = x_left;
        }

        if (!has_intersection)
            continue;

        // Ensure x_left <= x_right
        if (x_left > x_right)
        {
            int temp = x_left;
            x_left = x_right;
            x_right = temp;
        }

        // Clamp to screen bounds
        if (x_left < 0)
            x_left = 0;
        if (x_right >= LCD_WIDTH)
            x_right = LCD_WIDTH - 1;

        // Draw horizontal line
        for (int x = x_left; x <= x_right; x++)
        {
            framebuffer[y * LCD_WIDTH + x] = color_index;
        }
    }
}

/******************************************************************************
function: Fill the entire framebuffer with a solid color
parameter:
    color : RGB565 color value to fill with
returns: none
******************************************************************************/
void lcd_fill(uint16_t color)
{
    const uint8_t color_index = lcd_color565_to_332(color);
    for (uint32_t i = 0; i < LCD_HEIGHT * LCD_WIDTH; i++)
    {
        framebuffer[i] = color_index;
    }
}

/******************************************************************************
function: Copy an external image buffer into the framebuffer at specified position
parameter:
    x      : Top-left X coordinate
    y      : Top-left Y coordinate
    width  : Width of the buffer to blit
    height : Height of the buffer to blit
    buffer : Pointer to RGB332 pixel data array (8-bit per pixel)
returns: none
******************************************************************************/
void lcd_blit(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *buffer)
{
    for (uint16_t j = 0; j < height; j++)
    {
        for (uint16_t i = 0; i < width; i++)
        {
            if ((x + i) < LCD_WIDTH && (y + j) < LCD_HEIGHT)
            {
                framebuffer[(y + j) * LCD_WIDTH + (x + i)] = buffer[j * width + i];
            }
        }
    }
}

/********************************************************************************
function: Get the current backlight brightness level
parameter: none
returns: Brightness level from 0 (off) to 100 (full)
********************************************************************************/
uint8_t lcd_get_backlight_level(void)
{
    return backlight_level;
}

/********************************************************************************
function: Get the current font height
parameter: none
returns: Font height in pixels
********************************************************************************/
uint8_t lcd_get_font_height(void)
{
    if (current_font != NULL)
    {
        return current_font->height;
    }
    return 0;
}

/********************************************************************************
function: Get the current font width
parameter: none
returns: Font width in pixels
********************************************************************************/
uint8_t lcd_get_font_width(void)
{
    if (current_font != NULL)
    {
        return current_font->width;
    }
    return 0;
}

static void _lcd_init(PIO pio, uint sm, const uint8_t *init_seq)
{
    const uint8_t *cmd = init_seq;
    while (*cmd)
    {
        lcd_write_cmd(pio, sm, cmd + 2, *cmd);
        sleep_ms(*(cmd + 1) * 5);
        cmd += *cmd + 2;
    }
}

/********************************************************************************
function: Initialize the LCD display hardware and framebuffer
parameter:
    horizontal : true for horizontal/landscape mode, false for vertical/portrait mode
returns: none
note: Can only be called once; subsequent calls are ignored
********************************************************************************/
void lcd_init()
{
    if (lcd_initialized)
    {
        return; // already initialized
    }

    _pio = ST7789_PIO;
    _sm = 0;
    uint offset = pio_add_program(_pio, &st7789_lcd_program);
    st7789_lcd_program_init(_pio, _sm, offset, PIN_DIN, PIN_CLK, SERIAL_CLK_DIV);

    gpio_init(PIN_CS);
    gpio_init(PIN_DC);
    gpio_init(PIN_RESET);
    gpio_init(PIN_BL);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_set_dir(PIN_RESET, GPIO_OUT);
    gpio_set_dir(PIN_BL, GPIO_OUT);

    gpio_put(PIN_CS, 1);
    gpio_put(PIN_RESET, 1);
    _lcd_init(_pio, _sm, st7789_init_seq);
    gpio_put(PIN_BL, 1);

    // init palette
    for (int i = 0; i < 256; i++)
    {
        // Extract RGB332 components
        uint8_t r3 = (i >> 5) & 0x07; // 3 bits for red
        uint8_t g3 = (i >> 2) & 0x07; // 3 bits for green
        uint8_t b2 = i & 0x03;        // 2 bits for blue

        // Convert to 8-bit RGB
        uint8_t r8 = (r3 * 255) / 7; // Scale 3-bit to 8-bit
        uint8_t g8 = (g3 * 255) / 7; // Scale 3-bit to 8-bit
        uint8_t b8 = (b2 * 255) / 3; // Scale 2-bit to 8-bit

        // Convert to RGB565 for the palette
        palette[i] = lcd_color332_to_565(r8, g8, b8);
    }

    lcd_set_font(LCD_DEFAULT_FONT_SIZE); // Set default font

    lcd_initialized = true; // set the flag to indicate initialization is done
}

/********************************************************************************
function: Hardware reset sequence for the LCD display
parameter: none
returns: none
note: Toggles the reset pin and performs timing delays
********************************************************************************/
void lcd_reset(void)
{
    gpio_put(PIN_RESET, 1);
    sleep_ms(100);
    gpio_put(PIN_RESET, 0);
    sleep_ms(100);
    gpio_put(PIN_RESET, 1);
    gpio_put(PIN_CS, 0);
    sleep_ms(100);
}

void lcd_set_font(FontSize size)
{
    switch (size)
    {
    case FONT_XTRA_SMALL:
        current_font = (FontTable *)&Font8;
        break;
    case FONT_SMALL:
        current_font = (FontTable *)&Font12;
        break;
    case FONT_MEDIUM:
        current_font = (FontTable *)&Font16;
        break;
    case FONT_LARGE:
        current_font = (FontTable *)&Font20;
        break;
    case FONT_XTRA_LARGE:
        current_font = (FontTable *)&Font24;
        break;
    default:
        current_font = (FontTable *)&Font16; // Default to medium if invalid size
        break;
    }
}

/******************************************************************************
function: Send the framebuffer contents to the physical display
parameter: none
returns: none
note: Call this after drawing operations to update the screen. This is the
      only function that actually writes to the display hardware, preventing
      screen tearing and ensuring atomic frame updates.
******************************************************************************/
void lcd_swap(void)
{
    // start sending pixel data
    st7789_start_pixels(_pio, _sm);

    // Convert 8-bit palette indices to 16-bit RGB565 and send
    for (uint32_t i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
    {
        uint8_t palette_index = framebuffer[i];
        uint16_t color = palette[palette_index];
        uint8_t high_byte = color >> 8;
        uint8_t low_byte = color & 0xFF;
        st7789_lcd_put(_pio, _sm, high_byte);
        st7789_lcd_put(_pio, _sm, low_byte);
    }
}

/******************************************************************************
function: Send a command byte to the LCD controller
parameter:
    pio   : PIO instance to use
    sm    : State machine number
    cmd   : Pointer to command byte(s)
    count : Number of bytes to send (including command byte)
returns: none
note: Low-level function used internally for LCD communication
******************************************************************************/
void lcd_write_cmd(PIO pio, uint sm, const uint8_t *cmd, size_t count)
{
    st7789_lcd_wait_idle(pio, sm);
    lcd_set_dc_cs(0, 0);
    st7789_lcd_put(pio, sm, *cmd++);
    if (count >= 2)
    {
        st7789_lcd_wait_idle(pio, sm);
        lcd_set_dc_cs(1, 0);
        for (size_t i = 0; i < count - 1; ++i)
            st7789_lcd_put(pio, sm, *cmd++);
    }
    st7789_lcd_wait_idle(pio, sm);
    lcd_set_dc_cs(1, 1);
}