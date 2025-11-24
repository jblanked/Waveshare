from micropython import const

ECHO_PIN = const(15)  # Echo pin number
TRIG_PIN = const(14)  # Trigger pin number


class UltrasonicSensor:
    """A class to represent an ultrasonic sensor for distance measurement."""

    def __init__(self) -> None:
        from waveshare_ultrasonic_sensor import init

        # Initialize Ultrasonic Sensor
        init()

    @property
    def distance(self) -> float:
        """Get the distance measured by the ultrasonic sensor."""
        from waveshare_ultrasonic_sensor import get_distance

        return get_distance()
