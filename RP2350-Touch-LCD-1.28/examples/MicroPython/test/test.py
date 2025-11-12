def main():
    """RP2350 Touch LCD Demo in MicroPython"""
    from time import sleep_ms
    from waveshare_touch import TOUCH_GESTURE_NONE
    from waveshare_lcd import (
        COLOR_BLACK,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_BLUE,
        COLOR_YELLOW,
        COLOR_CYAN,
        COLOR_VIOLET,
        COLOR_WHITE,
    )
    from touch import Touch
    from battery import Battery
    from qmi import QMI
    from lcd import LCD

    # Initialize LCD in horizontal mode
    lcd = LCD()

    # Initialize battery monitoring
    bat = Battery()

    # initialize touch controller
    inp = Touch()

    # Initialize QMI8658 sensor
    qmi = QMI()

    # Demo 1: Static shapes
    lcd.fill_screen(COLOR_BLACK)

    # Draw some shapes to demonstrate the API
    lcd.fill_rect(30, 70, 50, 30, COLOR_RED)  # Red filled rectangle
    lcd.draw_rect(100, 10, 40, 30, COLOR_GREEN)  # Green rectangle outline
    lcd.fill_circle(180, 80, 20, COLOR_BLUE)  # Blue filled circle
    lcd.draw_circle(120, 80, 20, COLOR_YELLOW)  # Yellow circle outline
    lcd.draw_line(10, 120, 230, 120, COLOR_CYAN)  # Cyan horizontal line
    lcd.fill_triangle(120, 150, 100, 190, 140, 190, COLOR_VIOLET)  # Violet triangle
    lcd.draw_text(50, 130, "RP2350 Touch LCD Demo", COLOR_WHITE)  # White text

    # Push static shapes to display
    lcd.swap()

    sleep_ms(5000)  # Show static demo for 5 seconds

    # Demo 2: Animation loop
    angle = 0

    while True:
        # Clear screen
        lcd.fill_screen(COLOR_BLACK)

        if inp.gesture != TOUCH_GESTURE_NONE:
            gesture_text = f"Gesture: {inp.gesture}"
            point_text = f"Point: X={inp.point[0]} Y={inp.point[1]}"
            battery_text = f"Battery: {bat.percentage}% {bat.voltage}mV"
            lcd.draw_text(80, 10, gesture_text, COLOR_WHITE)
            lcd.draw_text(60, 30, point_text, COLOR_WHITE)
            lcd.draw_text(70, 50, battery_text, COLOR_WHITE)
            inp.reset()

        # Read accelerometer and gyroscope data
        acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, tim_count = qmi.read_xyz()

        # Format and display accelerometer data
        acc_text = f"Acc: X={acc_x:4.1f} Y={acc_y:4.1f} Z={acc_z:4.1f}"
        gyro_text = f"Gyro: X={gyro_x:4.1f} Y={gyro_y:4.1f} Z={gyro_z:4.1f}"
        lcd.draw_text(30, 60, acc_text, COLOR_WHITE)
        lcd.draw_text(30, 180, gyro_text, COLOR_WHITE)

        # Draw a circle that changes size
        radius = int(15 + (angle % 10))
        lcd.draw_circle(120, 120, radius, COLOR_CYAN)

        # Draw rotating line
        line_x = int(120 + (angle % 40) - 20)
        line_y = int(120 + (angle % 30) - 15)
        lcd.draw_line(120, 120, line_x, line_y, COLOR_YELLOW)

        # Update display
        lcd.swap()

        # Update for next frame
        angle += 1


if __name__ == "__main__":
    main()
