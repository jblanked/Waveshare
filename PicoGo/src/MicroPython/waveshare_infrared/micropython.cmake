# Create a C module for Waveshare infrared extension
add_library(usermod_waveshare_infrared INTERFACE)

target_sources(usermod_waveshare_infrared INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_infrared.c
    ${CMAKE_CURRENT_LIST_DIR}/infrared.c
)

target_include_directories(usermod_waveshare_infrared INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_waveshare_infrared INTERFACE
    MODULE_WAVESHARE_INFRARED_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_infrared)
