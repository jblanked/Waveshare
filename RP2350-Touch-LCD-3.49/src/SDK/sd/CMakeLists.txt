file(GLOB SD_SOURCES "*.c" "*.cpp")
add_library(sd ${SD_SOURCES})

target_link_libraries(sd
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_i2c
        hardware_spi
        hardware_pio
)