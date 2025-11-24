#include "tracking_sensor.h"
#include "spi.pio.h"
#include "pico/binary_info.h"
#include <string.h>

// Static variables
static uint16_t _calibrated_min[NUM_SENSORS];
static uint16_t _calibrated_max[NUM_SENSORS];
static PIO _pio;
static uint _sm;
static uint _successive_not_on_line = 0;
static uint _max_fails = 20;
static uint16_t _last_value = 0;
static bool is_initialized = false;

void tracking_sensor_init()
{
    if (is_initialized)
    {
        return;
    }
    _pio = pio0;
    uint offset = pio_add_program(_pio, &spi_cpha0_program);
    _sm = pio_claim_unused_sm(_pio, true);

    pio_spi_init(_pio, _sm, offset,
                 12,            // 12 bits per SPI frame
                 156.25f / 2.f, // 800 kHz @ 125 clk_sys
                 false,         // CPHA = 0
                 false,         // CPOL = 0
                 CLK_PIN,
                 MOSI_PIN,
                 MISO_PIN);

    // Make the 'SPI' pins available to picotool
    bi_decl(bi_4pins_with_names(MISO_PIN, "SPI RX", MOSI_PIN, "SPI TX", CLK_PIN, "SPI SCK", CS_PIN, "SPI CS"));

    gpio_init(CS_PIN);
    gpio_put(CS_PIN, 1);
    gpio_set_dir(CS_PIN, GPIO_OUT);

    is_initialized = true;
}

void tracking_sensor_analog_read(uint16_t *buffer)
{
    uint16_t value[NUM_SENSORS + 1];
    memset(value, 0, sizeof(value));
    uint offset = 0;
    for (uint32_t i = offset; i < offset + NUM_SENSORS + 1; ++i)
    {
        gpio_put(CS_PIN, 0);
        // set channel
        pio_sm_put_blocking(_pio, _sm, i << 28);
        // get last channel value
        value[i - offset] = pio_sm_get_blocking(_pio, _sm) & 0xfff;
        gpio_put(CS_PIN, 1);
        value[i - offset] >>= 2;
        busy_wait_us(50);
    }
    // Copy all but first value to buffer
    memcpy(buffer, &value[1], NUM_SENSORS * sizeof(uint16_t));
}

void tracking_sensor_calibrate(void)
{
    for (size_t i = 0; i < NUM_SENSORS; ++i)
    {
        _calibrated_min[i] = 1023;
        _calibrated_max[i] = 0;
    }

    for (size_t j = 0; j < 10; ++j)
    {
        uint16_t tracking_sensor_values[NUM_SENSORS];
        tracking_sensor_analog_read(tracking_sensor_values);
        for (size_t i = 0; i < NUM_SENSORS; ++i)
        {
            if (_calibrated_max[i] < tracking_sensor_values[i] && tracking_sensor_values[i] != 0)
                _calibrated_max[i] = tracking_sensor_values[i];
            if (_calibrated_min[i] > tracking_sensor_values[i] && tracking_sensor_values[i] != 0)
                _calibrated_min[i] = tracking_sensor_values[i];
        }
    }
}

void tracking_sensor_get_calibrated_min(uint16_t *buffer)
{
    memcpy(buffer, _calibrated_min, NUM_SENSORS * sizeof(uint16_t));
}

void tracking_sensor_get_calibrated_max(uint16_t *buffer)
{
    memcpy(buffer, _calibrated_max, NUM_SENSORS * sizeof(uint16_t));
}

void tracking_sensor_fixed_calibration(void)
{
    uint16_t min_vals[] = {117, 129, 124, 127, 101};
    uint16_t max_vals[] = {841, 899, 925, 945, 823};
    memcpy(_calibrated_min, min_vals, NUM_SENSORS * sizeof(uint16_t));
    memcpy(_calibrated_max, max_vals, NUM_SENSORS * sizeof(uint16_t));
}

void tracking_sensor_read_calibrated(uint16_t *buffer)
{
    uint16_t value = 0;
    tracking_sensor_analog_read(buffer);

    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        uint16_t denominator = _calibrated_max[i] - _calibrated_min[i];
        if (denominator != 0)
            value = (buffer[i] - _calibrated_min[i]) * 1000 / denominator;
        if (value < 0)
            value = 0;
        else if (value > 1000)
            value = 1000;
        buffer[i] = value;
    }
}

void tracking_sensor_read_line(uint16_t *position, uint16_t *tracking_sensor_values, bool white_line)
{
    tracking_sensor_read_calibrated(tracking_sensor_values);
    double avg = 0;
    double sum1 = 0;
    bool on_line = false;
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        uint16_t value = tracking_sensor_values[i];
        if (white_line)
            value = 1000 - value;

        // keep track of whether we see the line at all
        if (value < 800)
            on_line = true;

        // only average in values that are above a noise threshold
        if (value > 50)
            avg += value * ((i + 1) * 1000); // this is for the weighted total,
        sum1 += value;                       // this is for the denominator
    }

    if (on_line)
        _successive_not_on_line = 0;
    else
        _successive_not_on_line++;
    if (_successive_not_on_line >= _max_fails)
        _successive_not_on_line = _max_fails;

    if (_successive_not_on_line >= _max_fails)
    {
        // If last read to the left of center, return min.
        if (_last_value < 3050)
        {
            _last_value = 2500;
        }
        // If last read to the right of center, return the max.
        else
        {
            _last_value = 3500;
        }
    }

    if (on_line)
    {
        if (sum1 != 0)
            _last_value = (uint16_t)(avg / sum1);
    }
    *position = _last_value;
}