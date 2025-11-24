# Create a C module for Waveshare ultrasonic_sensor extension
add_library(usermod_waveshare_ultrasonic_sensor INTERFACE)

target_sources(usermod_waveshare_ultrasonic_sensor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_ultrasonic_sensor.c
    ${CMAKE_CURRENT_LIST_DIR}/ultrasonic_sensor.c
)

target_include_directories(usermod_waveshare_ultrasonic_sensor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_waveshare_ultrasonic_sensor INTERFACE
    MODULE_WAVESHARE_ULTRASONIC_SENSOR_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_ultrasonic_sensor)
