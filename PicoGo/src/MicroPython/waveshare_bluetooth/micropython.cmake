# Create a C module for Waveshare bluetooth extension
add_library(usermod_waveshare_bluetooth INTERFACE)

target_sources(usermod_waveshare_bluetooth INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_bluetooth.c
    ${CMAKE_CURRENT_LIST_DIR}/bluetooth.c
)

target_include_directories(usermod_waveshare_bluetooth INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_waveshare_bluetooth INTERFACE
    MODULE_WAVESHARE_BLUETOOTH_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_bluetooth)
