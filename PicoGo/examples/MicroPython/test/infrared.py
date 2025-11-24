from micropython import const

IR_PIN = const(5)  # GPIO5
#
IR_REMOTE_UP = const(0x18)  # 2 key
IR_REMOTE_LEFT = const(0x08)  # 4 key
IR_REMOTE_OK = const(0x1C)  # 5 key
IR_REMOTE_RIGHT = const(0x5A)  # 6 key
IR_REMOTE_DOWN = const(0x52)  # 8 key
#
IR_REMOTE_VOLUME_UP = const(0x15)  # + key
IR_REMOTE_VOLUME_DOWN = const(0x07)  # - key
IR_REMOTE_EQ = const(0x09)  # EQ key
IR_REMOTE_NEXT = const(0x40)  # >>| key
IR_REMOTE_PREV = const(0x44)  # |<< key
IR_REMOTE_PLAY_PAUSE = const(0x43)  # play/pause key
IR_REMOTE_CHANNEL_DOWN = const(0x45)  # CH- key
IR_REMOTE_CHANNEL = const(0x46)  # CH key
IR_REMOTE_CHANNEL_UP = const(0x47)  # CH+ key
IR_REMOTE_100_PLUS = const(0x19)  # 100+ key
IR_REMOTE_200_PLUS = const(0x0D)  # 200+ key
#
IR_REMOTE_0 = const(0x16)  # 0 key
IR_REMOTE_1 = const(0x0C)  # 1 key
IR_REMOTE_3 = const(0x5E)  # 3 key
IR_REMOTE_7 = const(0x42)  # 7 key
IR_REMOTE_9 = const(0x4A)  # 9 key


class Infrared:
    """Waveshare Infrared Native C Extension for MicroPython"""

    def __init__(self):
        from waveshare_infrared import init
        from machine import Pin

        self._last_key = 0

        # set pin
        self.pin = Pin(IR_PIN, Pin.IN, Pin.PULL_UP)

        # set callback
        self.pin.irq(handler=self.__callback, trigger=Pin.IRQ_FALLING)

        # Initialize Infrared
        init()

    def __callback(self, pin):
        """Touch interrupt callback function"""
        import utime

        IR = self.pin
        if IR.value() == 0:
            count = 0
            while (IR.value() == 0) and (count < 100):  # 9ms
                count += 1
                utime.sleep_us(100)
            if count < 10:
                return None
            count = 0
            while (IR.value() == 1) and (count < 50):  # 4.5ms
                count += 1
                utime.sleep_us(100)

            idx = 0
            cnt = 0
            data = [0, 0, 0, 0]
            for i in range(0, 32):
                count = 0
                while (IR.value() == 0) and (count < 10):  # 0.56ms
                    count += 1
                    utime.sleep_us(100)

                count = 0
                while (IR.value() == 1) and (count < 20):  # 0: 0.56mx
                    count += 1  # 1: 1.69ms
                    utime.sleep_us(100)

                if count > 7:
                    data[idx] |= 1 << cnt
                if cnt == 7:
                    cnt = 0
                    idx += 1
                else:
                    cnt += 1

            if data[0] + data[1] == 0xFF and data[2] + data[3] == 0xFF:  # check
                self._last_key = data[2]
            else:
                self._last_key = 0

    @property
    def dsl(self) -> bool:
        """Get the DSL status"""
        from waveshare_infrared import get_dsl_status

        return get_dsl_status()

    @property
    def dsr(self) -> bool:
        """Get the DSR status"""
        from waveshare_infrared import get_dsr_status

        return get_dsr_status()

    @property
    def remote_key(self) -> int:
        """Get the last received remote key"""
        temp = self._last_key
        self._last_key = 0
        return temp
