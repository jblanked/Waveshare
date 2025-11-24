// original author: Waveshare Team/MKesenheimer, translated from https://github.com/MKesenheimer/pico-go/tree/master
// 1.14inch LCD Module
#pragma once

#include "stdio.h"
#include <stdlib.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "st7789_lcd.pio.h"

#include "fonts.h"

#define LCD_WIDTH 240
#define LCD_HEIGHT 135

#define PIN_DIN 11
#define PIN_CLK 10
#define PIN_CS 9
#define PIN_DC 8
#define PIN_RESET 12
#define PIN_BL 13
#define ST7789_PIO pio1

#define SERIAL_CLK_DIV 1.f

#define LCD_DEFAULT_FONT_SIZE FONT_SMALL

// RGB565 Color definitions
#ifndef COLOR_WHITE
#define COLOR_WHITE 0xFFFF
#endif

#ifndef COLOR_BLACK
#define COLOR_BLACK 0x0000
#endif

#ifndef COLOR_BLUE
#define COLOR_BLUE 0x001F
#endif

#ifndef COLOR_CYAN
#define COLOR_CYAN 0x07FF
#endif

#ifndef COLOR_RED
#define COLOR_RED 0xF800
#endif

#ifndef COLOR_LIGHTGREY
#define COLOR_LIGHTGREY 0xD69A
#endif

#ifndef COLOR_DARKGREY
#define COLOR_DARKGREY 0x7BEF
#endif

#ifndef COLOR_GREEN
#define COLOR_GREEN 0x07E0
#endif

#ifndef COLOR_DARKCYAN
#define COLOR_DARKCYAN 0x03EF
#endif

#ifndef COLOR_DARKGREEN
#define COLOR_DARKGREEN 0x03E0
#endif

#ifndef COLOR_SKYBLUE
#define COLOR_SKYBLUE 0x867D
#endif

#ifndef COLOR_VIOLET
#define COLOR_VIOLET 0x915C
#endif

#ifndef COLOR_BROWN
#define COLOR_BROWN 0x9A60
#endif

#ifndef COLOR_TRANSPARENT
#define COLOR_TRANSPARENT 0x0120
#endif

#ifndef COLOR_YELLOW
#define COLOR_YELLOW 0xFFE0
#endif

#ifndef COLOR_ORANGE
#define COLOR_ORANGE 0xFDA0
#endif

#ifndef COLOR_PINK
#define COLOR_PINK 0xFE19
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    // Core display functions
    void lcd_init();
    void lcd_reset(void);
    void lcd_swap(void);

    // Framebuffer drawing functions
    void lcd_draw_pixel(uint8_t x, uint8_t y, uint16_t color);
    void lcd_fill(uint16_t color);
    void lcd_blit(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *buffer);

    // Shape drawing functions
    void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void lcd_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    void lcd_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    void lcd_draw_circle(uint16_t center_x, uint16_t center_y, uint16_t radius, uint16_t color);
    void lcd_fill_circle(uint16_t center_x, uint16_t center_y, uint16_t radius, uint16_t color);
    void lcd_fill_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);

    // Text rendering functions
    void lcd_draw_char(uint16_t x, uint16_t y, char c, uint16_t color);
    void lcd_draw_text(uint16_t x, uint16_t y, const char *text, uint16_t color);
    uint8_t lcd_get_font_height(void);
    uint8_t lcd_get_font_width(void);
    void lcd_set_font(FontSize size);

    // Low-level LCD communication functions
    void lcd_write_cmd(PIO pio, uint sm, const uint8_t *cmd, size_t count);

#ifdef __cplusplus
}
#endif