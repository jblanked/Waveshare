file(GLOB LCD_SOURCES "*.c" "*.cpp")
add_library(lcd ${LCD_SOURCES})

# Generate PIO header from .pio file
pico_generate_pio_header(lcd ${CMAKE_CURRENT_LIST_DIR}/st7789_lcd.pio)

target_link_libraries(lcd
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_spi
        hardware_pio
        hardware_pwm
)