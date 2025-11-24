// original author: Waveshare Team/MKesenheimer, translated from https://github.com/MKesenheimer/pico-go/tree/master

#pragma once
#include "stdio.h"
#include <stdlib.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"

#define PIN_PWMA 16
#define PIN_PWMB 21
//
#define PIN_AIN2 17
#define PIN_AIN1 18
#define PIN_BIN1 19
#define PIN_BIN2 20
//
#define MOTOR_FREQ 1000

#ifdef __cplusplus
extern "C"
{
#endif
    void motor_init(void);
    void motor_forward(uint8_t speed);
    void motor_backward(uint8_t speed);
    void motor_left(uint8_t speed);
    void motor_right(uint8_t speed);
    void motor_stop();
    void motor_set(int left, int right);

#ifdef __cplusplus
}
#endif