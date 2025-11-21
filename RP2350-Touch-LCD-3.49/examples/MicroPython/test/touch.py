class Touch:
    """RP2350 Touch controller in gesture mode with interrupt handling"""

    def __init__(self):
        from waveshare_touch import init
        from machine import Pin

        # Initialize touch in gesture mode
        init()
        # set pin
        self.pin = Pin(11, Pin.IN, Pin.PULL_UP)
        # set callback
        self.pin.irq(handler=self.__touch_callback, trigger=Pin.IRQ_FALLING)

        self._last_point = (0, 0)

    def __del__(self):
        """Destructor to clean up resources"""
        from waveshare_touch import reset

        self.pin = None
        reset()
        self.reset()

    @property
    def point(self):
        """Get the last pressed point"""
        return self._last_point

    def __touch_callback(self, pin):
        """Touch interrupt callback function"""
        from waveshare_touch import get_touch_point

        self._last_point = get_touch_point()

    def reset(self):
        """Reset saved touch state"""
        from waveshare_touch import reset_state

        reset_state()
        self._last_point = (0, 0)
