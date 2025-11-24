file(GLOB ULTRASONIC_SENSOR_SOURCES "*.c" "*.cpp")
add_library(ultrasonic_sensor ${ULTRASONIC_SENSOR_SOURCES})

target_link_libraries(ultrasonic_sensor
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
)