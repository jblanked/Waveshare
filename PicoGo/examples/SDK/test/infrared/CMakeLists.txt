file(GLOB INFRARED_SOURCES "*.c" "*.cpp")
add_library(infrared ${INFRARED_SOURCES})

target_link_libraries(infrared
        pico_stdlib
        hardware_gpio
)
