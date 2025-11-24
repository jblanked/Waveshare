def main():
    """RP2350 LCD Demo in MicroPython"""
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
        COLOR_ORANGE,
        COLOR_PINK,
    )
    from battery import Battery
    from bluetooth import Bluetooth
    from lcd import LCD
    from motor import Motor
    from tracking_sensor import TrackingSensor
    from ultrasonic_sensor import UltrasonicSensor
    from infrared import (
        Infrared,
        IR_REMOTE_UP,
        IR_REMOTE_LEFT,
        IR_REMOTE_OK,
        IR_REMOTE_RIGHT,
        IR_REMOTE_DOWN,
        IR_REMOTE_EQ,
        IR_REMOTE_VOLUME_UP,
        IR_REMOTE_VOLUME_DOWN,
        IR_REMOTE_NEXT,
        IR_REMOTE_PREV,
        IR_REMOTE_PLAY_PAUSE,
        IR_REMOTE_CHANNEL_DOWN,
        IR_REMOTE_CHANNEL,
        IR_REMOTE_CHANNEL_UP,
        IR_REMOTE_100_PLUS,
        IR_REMOTE_200_PLUS,
        IR_REMOTE_0,
        IR_REMOTE_1,
        IR_REMOTE_3,
        IR_REMOTE_7,
        IR_REMOTE_9,
    )

    from frames import FRAME_1, FRAME_2, FRAME_3

    # Initialize LCD in horizontal mode
    lcd = LCD()

    # Initialize motors
    motor = Motor()

    # Initialize Bluetooth
    bluetooth = Bluetooth()

    # Initialize tracking sensor
    tracking_sensor = TrackingSensor()

    # Initialize ultrasonic sensor
    ultrasonic_sensor = UltrasonicSensor()

    # Initialize battery monitoring
    bat = Battery()

    # Initialize infrared sensors and remote
    infrared = Infrared()

    # Move in a circle :D
    motor.set(50, 50)
    sleep_ms(2000)  # Run motors for 2 seconds
    motor.stop()

    # Demo 1: Static shapes
    lcd.fill_screen(COLOR_BLACK)

    # Draw some shapes to demonstrate the API
    lcd.fill_rect(30, 40, 50, 17, COLOR_RED)  # Red filled rectangle
    lcd.draw_rect(100, 6, 40, 17, COLOR_GREEN)  # Green rectangle outline
    lcd.fill_circle(180, 45, 20, COLOR_BLUE)  # Blue filled circle
    lcd.draw_circle(120, 45, 20, COLOR_YELLOW)  # Yellow circle outline
    lcd.draw_line(10, 68, 230, 68, COLOR_CYAN)  # Cyan horizontal line
    lcd.fill_triangle(120, 85, 100, 107, 140, 107, COLOR_VIOLET)  # Violet triangle
    lcd.draw_text(50, 74, "RP2350 LCD Demo", COLOR_WHITE)  # White text

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
                110,
                62,
                16,
                15,
                FRAME_1 if frame == 0 else (FRAME_2 if frame == 1 else FRAME_3),
            )
            lcd.swap()
            sleep_ms(200)
            frame += 1

    # Demo 2: Animation loop
    angle = 0

    # Tracking sensor variables
    tracking_sensor_text = ""

    # Infrared remote control variables
    key = 0
    speed = 50
    no_key_counter = 0
    ir_status_text = ""

    while True:
        # Clear screen
        lcd.fill_screen(COLOR_BLACK)

        # Read tracking sensor values and line position
        line_position, tracking_sensor_values = tracking_sensor.line

        # Display line position
        tracking_sensor_text = f"Line Pos: {line_position}"
        lcd.draw_text(10, 10, tracking_sensor_text, COLOR_WHITE)

        # Display individual tracking sensor values
        tracking_sensor_text = f"S0:{tracking_sensor_values[0]} S1:{tracking_sensor_values[1]} S2:{tracking_sensor_values[2]}"
        lcd.draw_text(10, 26, tracking_sensor_text, COLOR_CYAN)

        tracking_sensor_text = (
            f"S3:{tracking_sensor_values[3]} S4:{tracking_sensor_values[4]}"
        )
        lcd.draw_text(10, 42, tracking_sensor_text, COLOR_CYAN)

        # Get distance from ultrasonic sensor
        distance = ultrasonic_sensor.distance
        distance_text = f"Distance: {distance:.2f} cm"
        lcd.draw_text(10, 58, distance_text, COLOR_ORANGE)

        # Get battery percentage and voltage
        battery_percentage = bat.percentage
        battery_voltage = bat.voltage
        battery_text = f"Battery: {battery_percentage}% ({battery_voltage:.2f} V)"
        lcd.draw_text(10, 74, battery_text, COLOR_GREEN)

        # Read infrared obstacle avoidance sensors
        dsr_status = infrared.dsr  # Right sensor
        dsl_status = infrared.dsl  # Left sensor

        # Display obstacle sensor status
        obstacle_text = f"IR Obs L:{dsl_status} R:{dsr_status}"
        lcd.draw_text(
            10,
            90,
            obstacle_text,
            COLOR_RED if (dsl_status == 0 or dsr_status == 0) else COLOR_GREEN,
        )

        # Check for infrared remote key press
        key = infrared.remote_key

        if key != 0:
            no_key_counter = 0

            if key == IR_REMOTE_UP:  # 2 key - Forward
                motor.forward(speed)
                ir_status_text = "IR: FORWARD"
            elif key == IR_REMOTE_LEFT:  # 4 key - Left
                motor.left(20)
                ir_status_text = "IR: LEFT"
            elif key == IR_REMOTE_OK:  # 5 key - Stop
                motor.stop()
                ir_status_text = "IR: STOP"
            elif key == IR_REMOTE_RIGHT:  # 6 key - Right
                motor.right(20)
                ir_status_text = "IR: RIGHT"
            elif key == IR_REMOTE_DOWN:  # 8 key - Backward
                motor.backward(speed)
                ir_status_text = "IR: BACKWARD"
            elif key == IR_REMOTE_EQ:  # EQ key - Reset speed to 50
                speed = 50
                ir_status_text = "IR: Speed=50"
            elif key == IR_REMOTE_VOLUME_UP:  # + key - Increase speed
                if speed + 10 <= 100:
                    speed += 10
                ir_status_text = "IR: Speed UP"
            elif key == IR_REMOTE_VOLUME_DOWN:  # - key - Decrease speed
                if speed - 10 >= 0:
                    speed -= 10
                ir_status_text = "IR: Speed DOWN"
            # Read other keys but do nothing
            elif key == IR_REMOTE_NEXT:  # >>| key
                ir_status_text = "IR: NEXT"
            elif key == IR_REMOTE_PREV:  # |<< key
                ir_status_text = "IR: PREV"
            elif key == IR_REMOTE_PLAY_PAUSE:  # play/pause key
                ir_status_text = "IR: PLAY/PAUSE"
            elif key == IR_REMOTE_CHANNEL_DOWN:  # CH- key
                ir_status_text = "IR: CH-"
            elif key == IR_REMOTE_CHANNEL:  # CH key
                ir_status_text = "IR: CH"
            elif key == IR_REMOTE_CHANNEL_UP:  # CH+ key
                ir_status_text = "IR: CH+"
            elif key == IR_REMOTE_100_PLUS:  # 100+ key
                ir_status_text = "IR: 100+"
            elif key == IR_REMOTE_200_PLUS:  # 200+ key
                ir_status_text = "IR: 200+"
            elif key == IR_REMOTE_0:  # 0 key
                ir_status_text = "IR: 0"
            elif key == IR_REMOTE_1:  # 1 key
                ir_status_text = "IR: 1"
            elif key == IR_REMOTE_3:  # 3 key
                ir_status_text = "IR: 3"
            elif key == IR_REMOTE_7:  # 7 key
                ir_status_text = "IR: 7"
            elif key == IR_REMOTE_9:  # 9 key
                ir_status_text = "IR: 9"
            else:
                ir_status_text = f"IR: 0x{key:02X}"
        else:
            # No key pressed
            no_key_counter += 1

            # Auto-stop if no key for ~800 iterations (timeout)
            if no_key_counter > 800:
                motor.stop()
                no_key_counter = 0
                ir_status_text = "IR: Timeout"
            elif no_key_counter == 1:
                ir_status_text = "IR: Ready"

        # Display IR status and speed
        lcd.draw_text(10, 106, ir_status_text, COLOR_YELLOW)

        ir_speed_text = f"Speed: {speed}%"
        lcd.draw_text(10, 122, ir_speed_text, COLOR_PINK)

        # Update display
        lcd.swap()

        # Update for next frame
        angle += 1

        sleep_ms(50)  # Small delay for readability


if __name__ == "__main__":
    main()
