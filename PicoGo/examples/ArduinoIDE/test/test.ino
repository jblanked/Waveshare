#include "Arduino.h"
#include "battery.h"
#include "bluetooth.h"
#include "infrared.h"
#include "lcd.h"
#include "motor.h"
#include "tracking_sensor.h"
#include "ultrasonic_sensor.h"
#include "assets.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello, RP2350-LCD-1.14!");

    lcd_init(); // Initialize LCD in horizontal mode

    motor_init(); // Initialize motors

    bluetooth_init(); // Initialize Bluetooth

    tracking_sensor_init(); // Initialize tracking_sensors

    ultrasonic_sensor_init(); // Initialize ultrasonic sensor

    battery_init(); // Initialize battery monitoring

    infrared_init(); // Initialize infrared sensors and remote

    motor_set(50, 50); // move in a circle :D
    sleep_ms(2000);    // Run motors for 2 seconds
    motor_stop();      // Stop motors

    // Demo 1: Static shapes
    lcd_fill(COLOR_BLACK);

    uint8_t ten_y = (uint8_t)(LCD_HEIGHT * 0.04167); // 6
    // 0.5625 division from 240 to 135

    // Draw some shapes to demonstrate the API
    lcd_fill_rect(30, 40, 50, 17, COLOR_RED);                     // Red filled rectangle
    lcd_draw_rect(100, 6, 40, 17, COLOR_GREEN);                   // Green rectangle outline
    lcd_fill_circle(180, 45, 20, COLOR_BLUE);                     // Blue filled circle
    lcd_draw_circle(120, 45, 20, COLOR_YELLOW);                   // Yellow circle outline
    lcd_draw_line(10, 68, 230, 68, COLOR_CYAN);                   // Cyan horizontal line
    lcd_fill_triangle(120, 85, 100, 107, 140, 107, COLOR_VIOLET); // Violet triangle
    lcd_draw_text(50, 74, "RP2350 LCD Demo", COLOR_WHITE);        // White text

    // Push static shapes to display
    lcd_swap();

    sleep_ms(5000); // Show static demo for 5 seconds

    uint8_t frame = 0;
    uint8_t rotations = 10;
    while (rotations--)
    {
        frame = 0;
        while (frame < 3)
        {
            lcd_fill(COLOR_BLACK);
            lcd_blit(110, 62, 16, 15, frame == 0 ? frame_0 : (frame == 1 ? frame_1 : frame_2));
            lcd_swap();
            sleep_ms(200);
            frame++;
        }
    }
}

uint8_t angle = 0;

// tracking_Sensor variables
uint16_t tracking_sensor_values[NUM_SENSORS];
uint16_t line_position;
char tracking_sensor_text[60];

// Infrared remote control variables
uint8_t key = 0;
uint8_t speed = 50;
uint32_t no_key_counter = 0;
char ir_status_text[50];
char ir_speed_text[30];
char obstacle_text[40];

void loop()
{
    // Clear screen
    lcd_fill(COLOR_BLACK);

    // Read tracking_sensor values and line position
    tracking_sensor_read_line(&line_position, tracking_sensor_values, false);

    // Display line position
    snprintf(tracking_sensor_text, sizeof(tracking_sensor_text), "Line Pos: %d", line_position);
    lcd_draw_text(10, 10, tracking_sensor_text, COLOR_WHITE);

    // Display individual tracking_sensor values
    snprintf(tracking_sensor_text, sizeof(tracking_sensor_text), "S0:%d S1:%d S2:%d", tracking_sensor_values[0], tracking_sensor_values[1], tracking_sensor_values[2]);
    lcd_draw_text(10, 26, tracking_sensor_text, COLOR_CYAN);

    snprintf(tracking_sensor_text, sizeof(tracking_sensor_text), "S3:%d S4:%d", tracking_sensor_values[3], tracking_sensor_values[4]);
    lcd_draw_text(10, 42, tracking_sensor_text, COLOR_CYAN);

    // Get distance from ultrasonic sensor
    float distance = ultrasonic_get_distance();
    char distance_text[30];
    snprintf(distance_text, sizeof(distance_text), "Distance: %.2f cm", distance);
    lcd_draw_text(10, 58, distance_text, COLOR_ORANGE);

    // Get battery percentage and voltage
    uint8_t battery_percentage = battery_get_percentage();
    float battery_voltage = battery_get_voltage();
    char battery_text[40];
    snprintf(battery_text, sizeof(battery_text), "Battery: %d%% (%.2f V)", battery_percentage, battery_voltage);
    lcd_draw_text(10, 74, battery_text, COLOR_GREEN);

    // Read infrared obstacle avoidance sensors
    uint8_t dsr_status = infrared_get_dsr_status(); // Right sensor
    uint8_t dsl_status = infrared_get_dsl_status(); // Left sensor

    // Display obstacle sensor status
    snprintf(obstacle_text, sizeof(obstacle_text), "IR Obs L:%d R:%d", dsl_status, dsr_status);
    lcd_draw_text(10, 90, obstacle_text,
                  (dsl_status == 0 || dsr_status == 0) ? COLOR_RED : COLOR_GREEN);

    // Check for infrared remote key press
    key = infrared_get_remote_key();

    if (key != 0)
    {
        no_key_counter = 0;

        switch (key)
        {
        case IR_REMOTE_UP: // 2 key - Forward
            motor_forward(speed);
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: FORWARD");
            break;

        case IR_REMOTE_LEFT: // 4 key - Left
            motor_left(20);
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: LEFT");
            break;

        case IR_REMOTE_OK: // 5 key - Stop
            motor_stop();
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: STOP");
            break;

        case IR_REMOTE_RIGHT: // 6 key - Right
            motor_right(20);
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: RIGHT");
            break;

        case IR_REMOTE_DOWN: // 8 key - Backward
            motor_backward(speed);
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: BACKWARD");
            break;

        case IR_REMOTE_EQ: // EQ key - Reset speed to 50
            speed = 50;
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: Speed=50");
            break;

        case IR_REMOTE_VOLUME_UP: // + key - Increase speed
            if (speed + 10 <= 100)
            {
                speed += 10;
            }
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: Speed UP");
            break;

        case IR_REMOTE_VOLUME_DOWN: // - key - Decrease speed
            if (speed - 10 >= 0)
            {
                speed -= 10;
            }
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: Speed DOWN");
            break;
        // read other keys but do nothing
        case IR_REMOTE_NEXT: // >>| key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: NEXT");
            break;
        case IR_REMOTE_PREV: // |<< key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: PREV");
            break;
        case IR_REMOTE_PLAY_PAUSE: // play/pause key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: PLAY/PAUSE");
            break;
        case IR_REMOTE_CHANNEL_DOWN: // CH- key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: CH-");
            break;
        case IR_REMOTE_CHANNEL: // CH key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: CH");
            break;
        case IR_REMOTE_CHANNEL_UP: // CH+ key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: CH+");
            break;
        case IR_REMOTE_100_PLUS: // 100+ key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: 100+");
            break;
        case IR_REMOTE_200_PLUS: // 200+ key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: 200+");
            break;
        case IR_REMOTE_0: // 0 key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: 0");
            break;
        case IR_REMOTE_1: // 1 key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: 1");
            break;
        case IR_REMOTE_3: // 3 key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: 3");
            break;
        case IR_REMOTE_7: // 7 key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: 7");
            break;
        case IR_REMOTE_9: // 9 key
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: 9");
            break;
        default:
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: 0x%02X", key);
            break;
        }
    }
    else
    {
        // No key pressed
        no_key_counter++;

        // Auto-stop if no key for ~800 iterations (timeout)
        if (no_key_counter > 800)
        {
            motor_stop();
            no_key_counter = 0;
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: Timeout");
        }
        else if (no_key_counter == 1)
        {
            snprintf(ir_status_text, sizeof(ir_status_text), "IR: Ready");
        }
    }

    // Display IR status and speed
    lcd_draw_text(10, 106, ir_status_text, COLOR_YELLOW);

    snprintf(ir_speed_text, sizeof(ir_speed_text), "Speed: %d%%", speed);
    lcd_draw_text(10, 122, ir_speed_text, COLOR_PINK);

    // Update display
    lcd_swap();

    // Update for next frame
    angle++;

    sleep_ms(50); // Small delay for readability
}