class QMI:
    """QMI8658 6-axis IMU sensor interface"""

    def __init__(self):
        from waveshare_qmi import init

        init()

    @property
    def temperature(self) -> float:
        """Get temperature in degrees Celsius"""
        from waveshare_qmi import read_temperature

        return read_temperature()

    def read_acc_xyz(self) -> tuple[float, float, float]:
        """Read accelerometer X, Y, Z data in g"""
        from waveshare_qmi import read_acc_xyz as rax

        return rax()

    def read_ae(self) -> tuple[float, float, float, float, float, float, int]:
        """Read accelerometer and gyroscope X, Y, Z data and time count"""
        from waveshare_qmi import read_ae as rae

        return rae()

    def read_gyro_xyz(self) -> tuple[float, float, float]:
        """Read gyroscope X, Y, Z data in degrees per second"""
        from waveshare_qmi import read_gyro_xyz as rgx

        return rgx()

    def read_xyz(self) -> tuple[float, float, float, float, float, float, int]:
        """Read accelerometer and gyroscope X, Y, Z data and time count"""
        from waveshare_qmi import read_xyz as rx

        return rx()

    def read_xyz_raw(self) -> tuple[int, int, int, int, int, int, int]:
        """Read raw accelerometer and gyroscope X, Y, Z data and time count"""
        from waveshare_qmi import read_xyz_raw as rxr

        return rxr()

    def set_wake_on_motion(self, enable: bool):
        """Enable or disable Wake-on-Motion feature"""
        from waveshare_qmi import set_wake_on_motion as swom

        swom(enable)
