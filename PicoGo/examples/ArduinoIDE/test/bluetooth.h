// original author: Waveshare Team/MKesenheimer, translated from https://github.com/MKesenheimer/pico-go/tree/master

#pragma once
#include "stdio.h"
#include <stdlib.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define UART_BUFFER_SIZE 256

#ifdef __cplusplus
extern "C"
{
#endif
    int bluetooth_get_buffer(char *buffer, size_t buffer_size);
    bool bluetooth_data_available();
    void bluetooth_init();
#ifdef __cplusplus
}
#endif