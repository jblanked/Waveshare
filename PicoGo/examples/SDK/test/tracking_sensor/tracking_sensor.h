// original author: Waveshare Team/MKesenheimer, translated from https://github.com/MKesenheimer/pico-go/tree/master

#pragma once
#include "stdio.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#define NUM_SENSORS 5
//
#define CLK_PIN 6
#define MOSI_PIN 7
#define MISO_PIN 27
#define CS_PIN 28

#ifdef __cplusplus
extern "C"
{
#endif

    // Initialize the sensor with specified pins
    void tracking_sensor_init();

    // Read raw analog values from all sensors
    // buffer: array of NUM_SENSORS uint16_t values to store results
    void tracking_sensor_analog_read(uint16_t *buffer);

    // Calibrate sensors by reading multiple samples
    void tracking_sensor_calibrate(void);

    // Get calibrated minimum values
    // buffer: array of NUM_SENSORS uint16_t values to store results
    void tracking_sensor_get_calibrated_min(uint16_t *buffer);

    // Get calibrated maximum values
    // buffer: array of NUM_SENSORS uint16_t values to store results
    void tracking_sensor_get_calibrated_max(uint16_t *buffer);

    // Set fixed calibration values
    void tracking_sensor_fixed_calibration(void);

    // Read calibrated sensor values (scaled 0-1000)
    // buffer: array of NUM_SENSORS uint16_t values to store results
    void tracking_sensor_read_calibrated(uint16_t *buffer);

    // Read line position and sensor values
    // position: pointer to store the line position (output)
    // tracking_sensor_values: array of NUM_SENSORS uint16_t values to store sensor readings (output)
    // white_line: true if detecting white line on dark background, false otherwise
    void tracking_sensor_read_line(uint16_t *position, uint16_t *tracking_sensor_values, bool white_line);

#ifdef __cplusplus
}
#endif