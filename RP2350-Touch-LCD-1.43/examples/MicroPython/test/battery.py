class Battery:
    """Battery management class"""

    def __init__(self):
        """Initialize battery management"""
        from waveshare_battery import init

        init()

    @property
    def percentage(self):
        """Get the current battery percentage"""
        from waveshare_battery import get_percentage

        return get_percentage()

    @property
    def voltage(self):
        """Get the current battery voltage in millivolts"""
        from waveshare_battery import get_voltage

        return get_voltage()

    def read(self):
        """read raw battery ADC value"""
        from waveshare_battery import read

        return read()
