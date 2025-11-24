class Motor:
    """Class to control a Waveshare motor driver via MicroPython."""

    def __init__(self):
        from waveshare_motor import init

        # Initialize Motor
        init()

    def backward(self, speed: int) -> None:
        """Move the motor backward at the specified speed."""
        from waveshare_motor import backward

        backward(speed)

    def forward(self, speed: int) -> None:
        """Move the motor forward at the specified speed."""
        from waveshare_motor import forward

        forward(speed)

    def left(self, speed: int) -> None:
        """Turn the motor left at the specified speed."""
        from waveshare_motor import left

        left(speed)

    def right(self, speed: int) -> None:
        """Turn the motor right at the specified speed."""
        from waveshare_motor import right

        right(speed)

    def set(self, left_speed: int, right_speed: int) -> None:
        """Set the speed of the left and right motors."""
        from waveshare_motor import set as motor_set

        motor_set(left_speed, right_speed)

    def stop(self) -> None:
        """Stop the motor."""
        from waveshare_motor import stop

        stop()
