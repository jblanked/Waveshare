file(GLOB BATTERY_SOURCES "*.c" "*.cpp")
add_library(battery ${BATTERY_SOURCES})

target_link_libraries(battery
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_spi
        hardware_adc
)