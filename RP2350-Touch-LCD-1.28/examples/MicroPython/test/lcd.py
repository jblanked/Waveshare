class LCD:
    """LCD class for Waveshare RP2350 Touch LCD"""

    def __init__(self):
        from waveshare_lcd import init

        # Initialize LCD in horizontal mode
        init(True)

    def blit(self, x: int, y: int, w: int, h: int, data: bytes):
        """Blit a bitmap to the specified position"""
        from waveshare_lcd import blit

        blit(x, y, w, h, data)

    def draw_pixel(self, x: int, y: int, color: int):
        """Draw a single pixel at the specified position with the given color"""
        from waveshare_lcd import draw_pixel

        draw_pixel(x, y, color)

    def fill_screen(self, color: int):
        """Fill the entire screen with the specified color"""
        from waveshare_lcd import fill_screen

        fill_screen(color)

    def set_font(self, font: int):
        """Set the font for text rendering"""
        from waveshare_lcd import set_font

        set_font(font)

    def swap(self):
        """Swap the display buffer to update the screen"""
        from waveshare_lcd import swap

        swap()

    def draw_text(self, x: int, y: int, text: str, color: int):
        """Draw text at the specified position with the given color"""
        from waveshare_lcd import draw_text

        draw_text(x, y, text, color)

    def fill_rect(self, x: int, y: int, w: int, h: int, color: int):
        """Draw a filled rectangle at the specified position with the given color"""
        from waveshare_lcd import fill_rect

        fill_rect(x, y, w, h, color)

    def draw_rect(self, x: int, y: int, w: int, h: int, color: int):
        """Draw a rectangle outline at the specified position with the given color"""
        from waveshare_lcd import draw_rect

        draw_rect(x, y, w, h, color)

    def fill_circle(self, x: int, y: int, r: int, color: int):
        """Draw a filled circle at the specified position with the given color"""
        from waveshare_lcd import fill_circle

        fill_circle(x, y, r, color)

    def draw_circle(self, x: int, y: int, r: int, color: int):
        """Draw a circle outline at the specified position with the given color"""
        from waveshare_lcd import draw_circle

        draw_circle(x, y, r, color)

    def draw_line(self, x1: int, y1: int, x2: int, y2: int, color: int):
        """Draw a line from (x1, y1) to (x2, y2) with the given color"""
        from waveshare_lcd import draw_line

        draw_line(x1, y1, x2, y2, color)

    def fill_triangle(
        self, x1: int, y1: int, x2: int, y2: int, x3: int, y3: int, color: int
    ):
        """Draw a filled triangle with the given vertices and color"""
        from waveshare_lcd import fill_triangle

        fill_triangle(x1, y1, x2, y2, x3, y3, color)

    def reset(self):
        """Reset the LCD display"""
        from waveshare_lcd import reset

        reset()
