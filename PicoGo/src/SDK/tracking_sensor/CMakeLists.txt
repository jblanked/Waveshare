file(GLOB TRACKING_SENSOR_SOURCES "*.c" "*.cpp")
add_library(tracking_sensor ${TRACKING_SENSOR_SOURCES})

# Generate PIO header from .pio file
pico_generate_pio_header(tracking_sensor ${CMAKE_CURRENT_LIST_DIR}/spi.pio)

target_link_libraries(tracking_sensor
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_spi
        hardware_pio
        hardware_pwm
)