file(GLOB TOUCH_SOURCES "*.c" "*.cpp")
add_library(touch ${TOUCH_SOURCES})

target_link_libraries(touch
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_i2c
)