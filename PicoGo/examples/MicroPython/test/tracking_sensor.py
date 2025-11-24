from micropython import const

NUM_SENSORS = const(5)  # Number of sensors in the tracking sensor array


class TrackingSensor:
    """Class to interface with the Waveshare Tracking Sensor."""

    def __init__(self):
        from waveshare_tracking_sensor import init

        # Initialize Tracking Sensor
        init()

    @property
    def analog(self) -> list[int]:
        """Get the analog readings from the tracking sensor."""
        from waveshare_tracking_sensor import analog_read

        return analog_read()

    @property
    def calibrated(self) -> list[int]:
        """Get the calibrated readings from the tracking sensor."""
        from waveshare_tracking_sensor import read_calibrated

        return read_calibrated()

    @property
    def calibrated_min(self) -> list[int]:
        """Get the calibrated minimum values from the tracking sensor."""
        from waveshare_tracking_sensor import get_calibrated_min

        return get_calibrated_min()

    @property
    def calibrated_max(self) -> list[int]:
        """Get the calibrated maximum values from the tracking sensor."""
        from waveshare_tracking_sensor import get_calibrated_max

        return get_calibrated_max()

    @property
    def line(self) -> tuple[int, list[int]]:
        """Get the line position and sensor values from the tracking sensor."""
        from waveshare_tracking_sensor import read_line

        line_position = 0
        return read_line(line_position, False)

    def sensor_calibrate(self) -> None:
        """Calibrate the tracking sensor."""
        from waveshare_tracking_sensor import sensor_calibrate

        sensor_calibrate()

    def sensor_fixed_calibration(self) -> None:
        """Perform fixed calibration on the tracking sensor."""
        from waveshare_tracking_sensor import sensor_fixed_calibration

        sensor_fixed_calibration()
