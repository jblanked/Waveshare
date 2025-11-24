#include "infrared.h"

static bool is_initialized = false;

// Static variable to store the last key received
static volatile uint8_t last_key = 0;
static volatile bool new_key_available = false;

// IR interrupt handler - reads and stores the IR key
static void infrared_irq_handler(uint gpio, uint32_t events)
{
    if (gpio != IR_PIN || !(events & GPIO_IRQ_EDGE_FALL))
        return;

    uint32_t count = 0;

    // Wait for 9ms start pulse (low) to finish
    while (gpio_get(IR_PIN) == 0 && count < 100)
    {
        count++;
        busy_wait_us(100);
    }

    // Check if pulse was valid (should be around 90 counts for 9ms)
    if (count < 10)
        return;

    count = 0;
    // Wait for 4.5ms space (high) to finish
    while (gpio_get(IR_PIN) == 1 && count < 50)
    {
        count++;
        busy_wait_us(100);
    }

    // Read 32 bits
    uint8_t idx = 0;
    uint8_t cnt = 0;
    uint8_t data[4] = {0, 0, 0, 0};

    for (uint8_t i = 0; i < 32; i++)
    {
        count = 0;
        // Wait for bit pulse (low ~560us) to finish
        while (gpio_get(IR_PIN) == 0 && count < 10)
        {
            count++;
            busy_wait_us(100);
        }

        count = 0;
        // Measure space duration (high)
        // Short space (~560us) = 0, Long space (~1690us) = 1
        while (gpio_get(IR_PIN) == 1 && count < 20)
        {
            count++;
            busy_wait_us(100);
        }

        // If count > 7, it's a long space (logical 1)
        if (count > 7)
        {
            data[idx] |= (1 << cnt);
        }

        if (cnt == 7)
        {
            cnt = 0;
            idx++;
        }
        else
        {
            cnt++;
        }
    }

    // Verify checksum and store
    if ((data[0] + data[1] == 0xFF) && (data[2] + data[3] == 0xFF))
    {
        last_key = data[2];
    }
}

// Initialize infrared sensors and remote control pin
void infrared_init(void)
{
    if (is_initialized)
    {
        return;
    }
    // Initialize obstacle avoidance sensors as inputs
    gpio_init(DSR_PIN);
    gpio_set_dir(DSR_PIN, GPIO_IN);
    gpio_pull_up(DSR_PIN);

    gpio_init(DSL_PIN);
    gpio_set_dir(DSL_PIN, GPIO_IN);
    gpio_pull_up(DSL_PIN);

    // Initialize IR receiver pin as input with interrupt
    gpio_init(IR_PIN);
    gpio_set_dir(IR_PIN, GPIO_IN);
    gpio_pull_up(IR_PIN);

    // Set up interrupt on falling edge (start of IR signal)
    gpio_set_irq_enabled_with_callback(IR_PIN, GPIO_IRQ_EDGE_FALL, true, &infrared_irq_handler);

    is_initialized = true;
}

// Get status of right obstacle sensor (0 = obstacle detected, 1 = clear)
uint8_t infrared_get_dsr_status(void)
{
    return gpio_get(DSR_PIN);
}

// Get status of left obstacle sensor (0 = obstacle detected, 1 = clear)
uint8_t infrared_get_dsl_status(void)
{
    return gpio_get(DSL_PIN);
}

// Get the last received key
// Returns key code or 0 if no key has been received
uint8_t infrared_get_remote_key(void)
{
    uint8_t key = last_key;
    last_key = 0; // Clear after reading to prevent repeating commands
    return key;
}