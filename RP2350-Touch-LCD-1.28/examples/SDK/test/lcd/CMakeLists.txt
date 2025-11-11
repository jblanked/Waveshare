file(GLOB LCD_SOURCES "*.c" "*.cpp")
add_library(lcd ${LCD_SOURCES})

target_link_libraries(lcd
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_spi
        hardware_pwm
)