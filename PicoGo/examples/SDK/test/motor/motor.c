#include "motor.h"
static bool motor_initialized = false; // flag to indicate if the motor is initialized
static uint _slice_num_a;
static uint _slice_num_b;

void motor_init(void)
{
    if (motor_initialized)
    {
        return; // already initialized
    }
    gpio_init(PIN_AIN1);
    gpio_init(PIN_AIN2);
    gpio_init(PIN_BIN1);
    gpio_init(PIN_BIN2);
    gpio_set_dir(PIN_AIN1, GPIO_OUT);
    gpio_set_dir(PIN_AIN2, GPIO_OUT);
    gpio_set_dir(PIN_BIN1, GPIO_OUT);
    gpio_set_dir(PIN_BIN2, GPIO_OUT);

    // Tell PWM pins they are allocated to the PWM
    gpio_set_function(PIN_PWMA, GPIO_FUNC_PWM);
    gpio_set_function(PIN_PWMB, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);

    // Find out which PWM slice is connected
    _slice_num_a = pwm_gpio_to_slice_num(PIN_PWMA);
    pwm_init(_slice_num_a, &config, true);
    _slice_num_b = pwm_gpio_to_slice_num(PIN_PWMB);
    pwm_init(_slice_num_b, &config, true);

    // disable PWM at startup
    pwm_set_enabled(_slice_num_a, true);
    pwm_set_enabled(_slice_num_b, true);

    motor_initialized = true; // set the flag to indicate initialization is done
}

void motor_forward(uint8_t speed)
{
    if ((speed >= 0) && (speed <= 100))
    {
        const int level = speed * 0xFFFF / 100;
        pwm_set_chan_level(_slice_num_a, PWM_CHAN_A, level);
        pwm_set_chan_level(_slice_num_b, PWM_CHAN_B, level);
        gpio_put(PIN_AIN1, 0);
        gpio_put(PIN_AIN2, 1);
        gpio_put(PIN_BIN1, 0);
        gpio_put(PIN_BIN2, 1);
    }
}

void motor_backward(uint8_t speed)
{
    if ((speed >= 0) && (speed <= 100))
    {
        const int level = speed * 0xFFFF / 100;
        pwm_set_chan_level(_slice_num_a, PWM_CHAN_A, level);
        pwm_set_chan_level(_slice_num_b, PWM_CHAN_B, level);
        gpio_put(PIN_AIN1, 1);
        gpio_put(PIN_AIN2, 0);
        gpio_put(PIN_BIN1, 1);
        gpio_put(PIN_BIN2, 0);
    }
}

void motor_left(uint8_t speed)
{
    if ((speed >= 0) && (speed <= 100))
    {
        const int level = speed * 0xFFFF / 100;
        pwm_set_chan_level(_slice_num_a, PWM_CHAN_A, level);
        pwm_set_chan_level(_slice_num_b, PWM_CHAN_B, level);
        gpio_put(PIN_AIN1, 1);
        gpio_put(PIN_AIN2, 0);
        gpio_put(PIN_BIN1, 0);
        gpio_put(PIN_BIN2, 1);
    }
}

void motor_right(uint8_t speed)
{
    if ((speed >= 0) && (speed <= 100))
    {
        const int level = speed * 0xFFFF / 100;
        pwm_set_chan_level(_slice_num_a, PWM_CHAN_A, level);
        pwm_set_chan_level(_slice_num_b, PWM_CHAN_B, level);
        gpio_put(PIN_AIN1, 0);
        gpio_put(PIN_AIN2, 1);
        gpio_put(PIN_BIN1, 1);
        gpio_put(PIN_BIN2, 0);
    }
}

void motor_stop()
{
    pwm_set_chan_level(_slice_num_a, PWM_CHAN_A, 0);
    pwm_set_chan_level(_slice_num_b, PWM_CHAN_B, 0);
    gpio_put(PIN_AIN1, 0);
    gpio_put(PIN_AIN2, 0);
    gpio_put(PIN_BIN1, 0);
    gpio_put(PIN_BIN2, 0);
}

void motor_set(int left, int right)
{
    if (left >= 0 && left <= 100)
    {
        const int level_left = left * 0xFFFF / 100;
        gpio_put(PIN_AIN1, 1);
        gpio_put(PIN_AIN2, 0);
        pwm_set_chan_level(_slice_num_a, PWM_CHAN_A, level_left);
    }
    else if (left < 0 && left >= -100)
    {
        const int level_left = (-left) * 0xFFFF / 100;
        gpio_put(PIN_AIN1, 0);
        gpio_put(PIN_AIN2, 1);
        pwm_set_chan_level(_slice_num_a, PWM_CHAN_A, level_left);
    }

    if (right >= 0 && right <= 100)
    {
        const int level_right = right * 0xFFFF / 100;
        gpio_put(PIN_BIN1, 0);
        gpio_put(PIN_BIN2, 1);
        pwm_set_chan_level(_slice_num_b, PWM_CHAN_B, level_right);
    }
    else if (right < 0 && right >= -100)
    {
        const int level_right = (-right) * 0xFFFF / 100;
        gpio_put(PIN_BIN1, 1);
        gpio_put(PIN_BIN2, 0);
        pwm_set_chan_level(_slice_num_b, PWM_CHAN_B, level_right);
    }
}