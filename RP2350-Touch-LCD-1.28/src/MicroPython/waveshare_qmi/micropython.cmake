# Create a C module for Waveshare qmi extension
add_library(usermod_waveshare_qmi INTERFACE)

target_sources(usermod_waveshare_qmi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_qmi.c
    ${CMAKE_CURRENT_LIST_DIR}/qmi.c
)

target_include_directories(usermod_waveshare_qmi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_waveshare_qmi INTERFACE
    MODULE_WAVESHARE_QMI_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_qmi)
