#include "bluetooth.h"

static bool bluetooth_initialized = false; // flag to indicate if the bluetooth is initialized
static bool bluetooth_data_is_available = false;
static char s_buffer[UART_BUFFER_SIZE] = {0};
static uint16_t s_buffer_index = 0;

static void bluetooth_on_uart_rx()
{
    static bool started = false;
    while (uart_is_readable(UART_ID))
    {
        if (!started)
        {
            s_buffer_index = 0;
            started = true;
            s_buffer[0] = '\0';
        }
        char ch = uart_getc(UART_ID);

        // bounds checking
        if (s_buffer_index < UART_BUFFER_SIZE - 1)
        {
            s_buffer[s_buffer_index++] = ch;
        }

        if (ch == ';' || ch == '\n')
        {
            s_buffer[s_buffer_index] = '\0';
            bluetooth_data_is_available = true;
            started = false;
        }
    }
}

void bluetooth_init()
{
    if (bluetooth_initialized)
    {
        return; // already initialized
    }
    // Set up UART with
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_fifo_enabled(UART_ID, false);
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, bluetooth_on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
    bluetooth_initialized = true; // set the flag to indicate initialization is done
}

int bluetooth_get_buffer(char *buffer, size_t buffer_size)
{
    if (!bluetooth_data_is_available)
    {
        return 0; // No data available
    }
    int ret = snprintf(buffer, buffer_size, "%s", s_buffer);
    bluetooth_data_is_available = false;
    return ret;
}

bool bluetooth_data_available()
{
    return bluetooth_data_is_available;
}