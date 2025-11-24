# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(test C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_subdirectory(battery)
add_subdirectory(bluetooth)
add_subdirectory(infrared)
add_subdirectory(lcd)
add_subdirectory(motor)
add_subdirectory(tracking_sensor)
add_subdirectory(ultrasonic_sensor)

add_executable(test main.c )

pico_set_program_name(test "test")
pico_set_program_version(test "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(test 0)
pico_enable_stdio_usb(test 1)

# Add the standard library to the build
target_link_libraries(test
        pico_stdlib
        pico_printf
        pico_float
        pico_status_led
        hardware_gpio
        hardware_i2c
        hardware_spi
        hardware_pio
        hardware_clocks
        hardware_adc
        hardware_pwm
        battery
        bluetooth
        infrared
        lcd
        motor
        tracking_sensor
        ultrasonic_sensor
        )

# Add the standard include files to the build
target_include_directories(test PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/battery
        ${CMAKE_CURRENT_LIST_DIR}/bluetooth
        ${CMAKE_CURRENT_LIST_DIR}/infrared
        ${CMAKE_CURRENT_LIST_DIR}/lcd
        ${CMAKE_CURRENT_LIST_DIR}/motor
        ${CMAKE_CURRENT_LIST_DIR}/tracking_sensor
        ${CMAKE_CURRENT_LIST_DIR}/ultrasonic_sensor
)

pico_add_extra_outputs(test)

