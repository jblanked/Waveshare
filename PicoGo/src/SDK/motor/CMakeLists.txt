file(GLOB MOTOR_SOURCES "*.c" "*.cpp")
add_library(motor ${MOTOR_SOURCES})

target_link_libraries(motor
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_spi
        hardware_pio
        hardware_pwm
)