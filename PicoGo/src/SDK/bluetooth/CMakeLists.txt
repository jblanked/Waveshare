file(GLOB BLUETOOTH_SOURCES "*.c" "*.cpp")
add_library(bluetooth ${BLUETOOTH_SOURCES})

target_link_libraries(bluetooth
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_spi
        hardware_pio
        hardware_pwm
)