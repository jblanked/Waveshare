#include "Arduino.h"
#include "battery.h"
#include "lcd.h"
#include "qmi.h"
#include "touch.h"
#include "assets.h"

TouchVector last_touch_point = {0, 0};
uint8_t last_gesture = 0;
void touch_callback(uint gpio, uint32_t events)
{
    if (gpio != TOUCH_INT_PIN)
    {
        return;
    }

    last_gesture = touch_get_gesture();
    switch (last_gesture)
    {
    case TOUCH_GESTURE_UP:
        Serial.printf("Gesture: UP\r\n");
        break;
    case TOUCH_GESTURE_DOWN:
        Serial.printf("Gesture: DOWN\r\n");
        break;
    case TOUCH_GESTURE_LEFT:
        Serial.printf("Gesture: LEFT\r\n");
        break;
    case TOUCH_GESTURE_RIGHT:
        Serial.printf("Gesture: RIGHT\r\n");
        break;
    case TOUCH_GESTURE_CLICK:
        Serial.printf("Gesture: CLICK\r\n");
        break;
    case TOUCH_GESTURE_DOUBLE_CLICK:
        Serial.printf("Gesture: DOUBLE CLICK\r\n");
        break;
    case TOUCH_GESTURE_LONG_PRESS:
        Serial.printf("Gesture: LONG PRESS\r\n");
        break;
    default:
        break;
    }

    if (last_gesture != TOUCH_GESTURE_NONE)
    {
        last_touch_point = touch_get_point();
        if (last_touch_point.x != 0 || last_touch_point.y != 0)
        {
            Serial.printf("Touch at (%d, %d)\r\n", last_touch_point.x, last_touch_point.y);
            Serial.printf("Battery percentage: %d%%, voltage: %.2f V\r\n", battery_get_percentage(), battery_get_voltage());
        }
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello, RP2350-Touch-LCD-1.28!");

    lcd_init(true); // Initialize LCD in horizontal mode

    touch_init(TOUCH_GESTURE_MODE);      // Initialize touch in gesture mode
    touch_set_callback(&touch_callback); // Set touch interrupt callback

    battery_init(); // Initialize battery monitoring

    qmi_init(); // Initialize QMI8658 sensor

    // Demo 1: Static shapes
    lcd_fill(COLOR_BLACK);

    // Draw some shapes to demonstrate the API
    lcd_fill_rect(30, 70, 50, 30, COLOR_RED);                      // Red filled rectangle
    lcd_draw_rect(100, 10, 40, 30, COLOR_GREEN);                   // Green rectangle outline
    lcd_fill_circle(180, 80, 20, COLOR_BLUE);                      // Blue filled circle
    lcd_draw_circle(120, 80, 20, COLOR_YELLOW);                    // Yellow circle outline
    lcd_draw_line(10, 120, 230, 120, COLOR_CYAN);                  // Cyan horizontal line
    lcd_fill_triangle(120, 150, 100, 190, 140, 190, COLOR_VIOLET); // Violet triangle
    lcd_draw_text(50, 130, "RP2350 Touch LCD Demo", COLOR_WHITE);  // White text

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
            lcd_blit(110, 110, 16, 15, frame == 0 ? frame_0 : (frame == 1 ? frame_1 : frame_2));
            lcd_swap();
            sleep_ms(200);
            frame++;
        }
    }
}

uint8_t angle = 0;
float acc[3], gyro[3];
unsigned int tim_count = 0;
char acc_text[50];
char gyro_text[50];

void loop()
{
    // Clear screen
    lcd_fill(COLOR_BLACK);

    qmi_read_xyz(acc, gyro, &tim_count);
    snprintf(acc_text, sizeof(acc_text), "X=%4.1f Y=%4.1f Z=%4.1f", acc[0], acc[1], acc[2]);
    snprintf(gyro_text, sizeof(gyro_text), "X=%4.1f Y=%4.1f Z=%4.1f", gyro[0], gyro[1], gyro[2]);
    lcd_draw_text(30, 60, acc_text, COLOR_WHITE);
    lcd_draw_text(30, 180, gyro_text, COLOR_WHITE);

    // Draw a circle that changes size
    uint8_t radius = 15 + (angle % 10);
    lcd_draw_circle(120, 120, radius, COLOR_CYAN);

    // Draw rotating line
    int line_x = 120 + (angle % 40) - 20;
    int line_y = 120 + (angle % 30) - 15;
    lcd_draw_line(120, 120, line_x, line_y, COLOR_YELLOW);

    // Update display
    lcd_swap();

    // Update for next frame
    angle++;
}