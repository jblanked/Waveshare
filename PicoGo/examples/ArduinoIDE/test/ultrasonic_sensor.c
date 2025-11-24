#include "ultrasonic_sensor.h"

static bool is_initialized = false;

void ultrasonic_sensor_init(void)
{
    if (is_initialized)
    {
        return;
    }
    gpio_init(TRIG_PIN);
    gpio_init(ECHO_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
    gpio_put(TRIG_PIN, 0);
    is_initialized = true;
}

float ultrasonic_get_distance(void)
{
    // Send trigger pulse
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Wait for echo to go high
    while (gpio_get(ECHO_PIN) == 0)
    {
        tight_loop_contents();
    }

    // Record start time
    uint64_t ts = time_us_64();

    // Wait for echo to go low
    while (gpio_get(ECHO_PIN) == 1)
    {
        tight_loop_contents();
    }

    // Record end time
    uint64_t te = time_us_64();

    // Calculate distance in cm
    float distance = ((te - ts) * 0.034f) / 2.0f;

    return distance;
}