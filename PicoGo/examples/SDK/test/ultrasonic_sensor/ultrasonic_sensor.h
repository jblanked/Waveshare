#pragma once
#include "stdio.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define ECHO_PIN 15
#define TRIG_PIN 14

#ifdef __cplusplus
extern "C"
{
#endif
    // Initialize the ultrasonic sensor pins
    void ultrasonic_sensor_init(void);

    // Get distance measurement in centimeters
    float ultrasonic_get_distance(void);
#ifdef __cplusplus
}
#endif