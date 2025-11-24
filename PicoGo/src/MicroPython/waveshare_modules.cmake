# Combined Waveshare Modules for MicroPython

# Include waveshare_lcd module
add_library(usermod_waveshare_lcd INTERFACE)

# Generate PIO header from .pio file
pico_generate_pio_header(usermod_waveshare_lcd
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_lcd/st7789_lcd.pio
)

target_sources(usermod_waveshare_lcd INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_lcd/waveshare_lcd.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_lcd/lcd.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_lcd/font8.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_lcd/font12.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_lcd/font16.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_lcd/font20.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_lcd/font24.c
)

target_include_directories(usermod_waveshare_lcd INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_lcd
)

target_compile_definitions(usermod_waveshare_lcd INTERFACE
    MODULE_WAVESHARE_LCD_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_lcd)

# Link against the required Pico SDK libraries for LCD
target_link_libraries(usermod_waveshare_lcd INTERFACE
    pico_stdlib
    pico_printf
    pico_float
    hardware_gpio
    hardware_spi
    hardware_pwm
)

# Include waveshare_battery module
add_library(usermod_waveshare_battery INTERFACE)

target_sources(usermod_waveshare_battery INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_battery/waveshare_battery.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_battery/battery.c
)

target_include_directories(usermod_waveshare_battery INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_battery
)

target_compile_definitions(usermod_waveshare_battery INTERFACE
    MODULE_WAVESHARE_BATTERY_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_battery)

# Link against the required Pico SDK libraries for battery
target_link_libraries(usermod_waveshare_battery INTERFACE
    pico_stdlib
    pico_printf
    pico_float
    hardware_gpio
    hardware_spi
    hardware_adc
)

# Include waveshare_bluetooth module
add_library(usermod_waveshare_bluetooth INTERFACE)

target_sources(usermod_waveshare_bluetooth INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_bluetooth/waveshare_bluetooth.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_bluetooth/bluetooth.c
)

target_include_directories(usermod_waveshare_bluetooth INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_bluetooth
)

target_compile_definitions(usermod_waveshare_bluetooth INTERFACE
    MODULE_WAVESHARE_BLUETOOTH_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_bluetooth)

# Link against the required Pico SDK libraries for bluetooth
target_link_libraries(usermod_waveshare_bluetooth INTERFACE
    pico_stdlib
    pico_printf
    pico_float
    hardware_gpio
    hardware_spi
    hardware_pio
    hardware_pwm
)

# Include waveshare_infrared module
add_library(usermod_waveshare_infrared INTERFACE)

target_sources(usermod_waveshare_infrared INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_infrared/waveshare_infrared.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_infrared/infrared.c
)

target_include_directories(usermod_waveshare_infrared INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_infrared
)

target_compile_definitions(usermod_waveshare_infrared INTERFACE
    MODULE_WAVESHARE_INFRARED_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_infrared)

# Link against the required Pico SDK libraries for infrared
target_link_libraries(usermod_waveshare_infrared INTERFACE
    pico_stdlib
    hardware_gpio
)

# Include waveshare_motor module
add_library(usermod_waveshare_motor INTERFACE)

target_sources(usermod_waveshare_motor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_motor/waveshare_motor.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_motor/motor.c
)

target_include_directories(usermod_waveshare_motor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_motor
)

target_compile_definitions(usermod_waveshare_motor INTERFACE
    MODULE_WAVESHARE_MOTOR_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_motor)

# Link against the required Pico SDK libraries for motor
target_link_libraries(usermod_waveshare_motor INTERFACE
    pico_stdlib
    pico_printf
    pico_float
    hardware_gpio
    hardware_spi
    hardware_pio
    hardware_pwm
)

# Include waveshare_tracking_sensor module
add_library(usermod_waveshare_tracking_sensor INTERFACE)

# Generate PIO header from .pio file
pico_generate_pio_header(usermod_waveshare_tracking_sensor
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_tracking_sensor/spi.pio
)

target_sources(usermod_waveshare_tracking_sensor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_tracking_sensor/waveshare_tracking_sensor.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_tracking_sensor/tracking_sensor.c
)

target_include_directories(usermod_waveshare_tracking_sensor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_tracking_sensor
)

target_compile_definitions(usermod_waveshare_tracking_sensor INTERFACE
    MODULE_WAVESHARE_TRACKING_SENSOR_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_tracking_sensor)

# Link against the required Pico SDK libraries for tracking_sensor
target_link_libraries(usermod_waveshare_tracking_sensor INTERFACE
        pico_stdlib
        pico_printf
        pico_float
        hardware_gpio
        hardware_spi
        hardware_pio
        hardware_pwm
)

# Include waveshare_ultrasonic_sensor module
add_library(usermod_waveshare_ultrasonic_sensor INTERFACE)

target_sources(usermod_waveshare_ultrasonic_sensor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_ultrasonic_sensor/waveshare_ultrasonic_sensor.c
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_ultrasonic_sensor/ultrasonic_sensor.c
)

target_include_directories(usermod_waveshare_ultrasonic_sensor INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/waveshare_ultrasonic_sensor
)

target_compile_definitions(usermod_waveshare_ultrasonic_sensor INTERFACE
    MODULE_WAVESHARE_ULTRASONIC_SENSOR_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_waveshare_ultrasonic_sensor)

# Link against the required Pico SDK libraries for ultrasonic_sensor
target_link_libraries(usermod_waveshare_ultrasonic_sensor INTERFACE
    pico_stdlib
    pico_printf
    pico_float
    hardware_gpio
)