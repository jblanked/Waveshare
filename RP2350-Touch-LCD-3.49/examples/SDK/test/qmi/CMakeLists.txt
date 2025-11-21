file(GLOB QMI_SOURCES "*.c" "*.cpp")
add_library(qmi ${QMI_SOURCES})

target_link_libraries(qmi
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_i2c
)