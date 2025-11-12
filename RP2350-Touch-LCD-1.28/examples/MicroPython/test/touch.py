class Touch:
    """RP2350 Touch controller in gesture mode with interrupt handling"""

    def __init__(self):
        from waveshare_touch import init, TOUCH_GESTURE_MODE, TOUCH_GESTURE_NONE
        from machine import Pin

        # Initialize touch in gesture mode
        init(TOUCH_GESTURE_MODE)
        # set pin
        self.pin = Pin(21, Pin.IN, Pin.PULL_UP)
        # set callback
        self.pin.irq(handler=self.__touch_callback, trigger=Pin.IRQ_FALLING)

        self._last_point = (0, 0)
        self._last_gesture = TOUCH_GESTURE_NONE

    def __del__(self):
        """Destructor to clean up resources"""
        from waveshare_touch import reset

        self.pin = None
        reset()
        self.reset()

    @property
    def gesture(self):
        """Get the last detected gesture"""
        return self._last_gesture

    @property
    def point(self):
        """Get the last pressed point"""
        return self._last_point

    def __touch_callback(self, pin):
        """Touch interrupt callback function"""
        from waveshare_touch import (
            get_gesture,
            TOUCH_GESTURE_NONE,
            get_touch_point,
        )

        self._last_gesture = get_gesture()
        if self._last_gesture != TOUCH_GESTURE_NONE:
            self._last_point = get_touch_point()

    def reset(self):
        """Reset saved touch state"""
        from waveshare_touch import TOUCH_GESTURE_NONE

        self._last_gesture = TOUCH_GESTURE_NONE
        self._last_point = (0, 0)
