# Create a C module for Waveshare tracking_sensor extension
add_library(usermod_waveshare_tracking_sensor INTERFACE)

# Generate PIO header from .pio file
pico_generate_pio_header(usermod_waveshare_tracking_sensor ${CMAKE_CURRENT_LIST_DIR}/spi.pio)

target_sources(usermod_waveshare_tracking_sensor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_tracking_sensor.c
    ${CMAKE_CURRENT_LIST_DIR}/tracking_sensor.c
)

target_include_directories(usermod_waveshare_tracking_sensor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_waveshare_tracking_sensor INTERFACE
    MODULE_WAVESHARE_TRACKING_SENSOR_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_tracking_sensor)
