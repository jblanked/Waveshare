#pragma once

#include "stdio.h"
#include <stdlib.h>
#include <stdint.h>
#include "pico/stdlib.h"

#define DSR_PIN (2)
#define DSL_PIN (3)
#define IR_PIN (5)

#define IR_REMOTE_UP 0x18    // 2 key
#define IR_REMOTE_LEFT 0x08  // 4 key
#define IR_REMOTE_OK 0x1C    // 5 key
#define IR_REMOTE_RIGHT 0x5A // 6 key
#define IR_REMOTE_DOWN 0x52  // 8 key
//
#define IR_REMOTE_VOLUME_UP 0x15    // + key
#define IR_REMOTE_VOLUME_DOWN 0x07  // - key
#define IR_REMOTE_EQ 0x09           // EQ key
#define IR_REMOTE_NEXT 0x40         // >>| key
#define IR_REMOTE_PREV 0x44         // |<< key
#define IR_REMOTE_PLAY_PAUSE 0x43   // play/pause key
#define IR_REMOTE_CHANNEL_DOWN 0x45 // CH- key
#define IR_REMOTE_CHANNEL 0x46      // CH key
#define IR_REMOTE_CHANNEL_UP 0x47   // CH+ key
#define IR_REMOTE_100_PLUS 0x19     // 100+ key
#define IR_REMOTE_200_PLUS 0x0D     // 200+ key
//
#define IR_REMOTE_0 0x16 // 0 key
#define IR_REMOTE_1 0x0C // 1 key
#define IR_REMOTE_3 0x5E // 3 key
#define IR_REMOTE_7 0x42 // 7 key
#define IR_REMOTE_9 0x4A // 9 key

#ifdef __cplusplus
extern "C"
{
#endif
    void infrared_init(void);
    uint8_t infrared_get_remote_key(void);
    uint8_t infrared_get_dsr_status(void);
    uint8_t infrared_get_dsl_status(void);
#ifdef __cplusplus
}
#endif