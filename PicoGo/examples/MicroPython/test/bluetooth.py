class Bluetooth:
    """Bluetooth class for Waveshare PicoGo Bluetooth Module"""

    def __init__(self):
        from waveshare_bluetooth import init

        # Initialize Bluetooth
        init()

    @property
    def buffer(self) -> bytes:
        """Get Bluetooth data buffer"""
        from waveshare_bluetooth import get_buffer

        return get_buffer()

    @property
    def is_available(self) -> bool:
        """Check if Bluetooth is available"""
        from waveshare_bluetooth import data_available

        return data_available()
