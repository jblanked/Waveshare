def main():
    """RP2350 Touch LCD Demo in MicroPython"""

    from time import sleep_ms
    from waveshare_lcd import (
        COLOR_BLACK,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_BLUE,
        COLOR_YELLOW,
        COLOR_CYAN,
        COLOR_VIOLET,
        COLOR_WHITE,
        LCD_WIDTH,
        LCD_HEIGHT,
    )
    from touch import Touch
    from battery import Battery
    from qmi import QMI

    from lcd import LCD
    from sd import SD
    from frames import FRAME_1, FRAME_2, FRAME_3

    # Initialize LCD in horizontal mode
    lcd = LCD()

    # Initialize battery monitoring
    bat = Battery()

    # initialize touch controller
    inp = Touch()

    # Initialize QMI8658 sensor
    qmi = QMI()

    # Initialize SD card
    sd = SD()

    if not sd.active:
        from sys import exit

        print("SD card not found")
        exit()

    # test SD card functionality
    sd.create_directory("/test_dir")
    sd.write("/test_dir/test_file.txt", "RP2350 Touch LCD MicroPython Demo\n")
    content = sd.read("/test_dir/test_file.txt", 0, 0)
    print(f"SD Card File Content: {content.decode('utf-8')}")

    twenty_y = int(LCD_HEIGHT * 0.04292)
    twenty_x = int(LCD_WIDTH * 0.04292)

    # Demo 1: Static shapes
    lcd.fill_screen(COLOR_BLACK)

    # Draw some shapes to demonstrate the API
    lcd.fill_rect(
        int(twenty_x * 3),
        int(twenty_y * 7),
        int(twenty_x * 5),
        int(twenty_y * 3),
        COLOR_RED,
    )
    # Red filled rectangle
    lcd.draw_rect(
        int(twenty_x * 10),
        int(twenty_y),
        int(twenty_x * 4),
        int(twenty_y * 3),
        COLOR_GREEN,
    )
    # Green rectangle outline
    lcd.fill_circle(int(twenty_x * 18), int(twenty_y * 8), int(twenty_x), COLOR_BLUE)
    # Blue filled circle
    lcd.draw_circle(int(twenty_x * 12), int(twenty_y * 8), int(twenty_x), COLOR_YELLOW)
    # Yellow circle outline
    lcd.draw_line(
        int(twenty_x),
        int(twenty_y * 12),
        int(twenty_x * 23),
        int(twenty_y * 12),
        COLOR_CYAN,
    )
    # Cyan horizontal line
    lcd.fill_triangle(
        int(twenty_x * 12),
        int(twenty_y * 15),
        int(twenty_x * 10),
        int(twenty_y * 19),
        int(twenty_x * 14),
        int(twenty_y * 19),
        COLOR_VIOLET,
    )
    # Violet triangle
    lcd.draw_text(
        int(twenty_x * 5), int(twenty_y * 13), "RP2350 Touch LCD Demo", COLOR_WHITE
    )
    # Push static shapes to display
    lcd.swap()

    sleep_ms(5000)  # Show static demo for 5 seconds

    frame = 0
    rotations = 10

    while rotations > 0:
        rotations -= 1
        frame = 0
        while frame < 3:
            lcd.fill_screen(COLOR_BLACK)
            lcd.blit(
                LCD_WIDTH // 2,
                LCD_HEIGHT // 2,
                16,
                15,
                FRAME_1 if frame == 0 else (FRAME_2 if frame == 1 else FRAME_3),
            )
            lcd.swap()
            sleep_ms(200)
            frame += 1

    # Demo 2: Animation loop
    angle = 0

    while True:
        # Clear screen
        lcd.fill_screen(COLOR_BLACK)
        x, y = inp.point
        if x != 0 or y != 0:
            point_text = f"Point: X={x} Y={y}"
            battery_text = f"Battery: {bat.percentage}% {bat.voltage}mV"
            lcd.draw_text(int(twenty_x * 6), int(twenty_y * 3), point_text, COLOR_WHITE)
            lcd.draw_text(
                int(twenty_x * 7), int(twenty_y * 5), battery_text, COLOR_WHITE
            )
            inp.reset()

        # Read accelerometer and gyroscope data
        acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, tim_count = qmi.read_xyz()

        # Format and display accelerometer data
        acc_text = f"Acc: X={acc_x:4.1f} Y={acc_y:4.1f} Z={acc_z:4.1f}"
        gyro_text = f"Gyro: X={gyro_x:4.1f} Y={gyro_y:4.1f} Z={gyro_z:4.1f}"
        lcd.draw_text(int(twenty_x * 4.5), int(twenty_y * 6), acc_text, COLOR_WHITE)
        lcd.draw_text(int(twenty_x * 4.5), int(twenty_y * 18), gyro_text, COLOR_WHITE)

        # Draw a circle that changes size
        radius = int(30 + (angle % 10))
        lcd.draw_circle(int(twenty_x * 12), int(twenty_y * 12), radius, COLOR_CYAN)

        # Draw rotating line
        line_x = int(twenty_x * 12 + (angle % 40) - 20)
        line_y = int(twenty_y * 12 + (angle % 30) - 15)
        lcd.draw_line(
            int(twenty_x * 12), int(twenty_y * 12), line_x, line_y, COLOR_YELLOW
        )

        # Update display
        lcd.swap()

        # Update for next frame
        angle += 1


if __name__ == "__main__":
    main()
