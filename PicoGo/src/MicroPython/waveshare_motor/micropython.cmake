# Create a C module for Waveshare motor extension
add_library(usermod_waveshare_motor INTERFACE)

target_sources(usermod_waveshare_motor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_motor.c
    ${CMAKE_CURRENT_LIST_DIR}/motor.c
)

target_include_directories(usermod_waveshare_motor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_waveshare_motor INTERFACE
    MODULE_WAVESHARE_MOTOR_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_motor)
