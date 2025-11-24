/*
 * Waveshare Infrared Native C Extension for MicroPython
 *
 */

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/mphal.h"
#include "infrared.h"
#include <stdlib.h>
#include <string.h>

// Define STATIC if not already defined (MicroPython macro)
#ifndef STATIC
#define STATIC static
#endif

STATIC mp_obj_t waveshare_infrared_init(void)
{
    infrared_init();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_infrared_init_obj, waveshare_infrared_init);

STATIC mp_obj_t waveshare_infrared_get_remote_key(void)
{
    uint8_t key = infrared_get_remote_key();
    return mp_obj_new_int(key);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_infrared_get_remote_key_obj, waveshare_infrared_get_remote_key);

STATIC mp_obj_t waveshare_infrared_get_dsr_status(void)
{
    uint8_t status = infrared_get_dsr_status();
    return mp_obj_new_int(status);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_infrared_get_dsr_status_obj, waveshare_infrared_get_dsr_status);

STATIC mp_obj_t waveshare_infrared_get_dsl_status(void)
{
    uint8_t status = infrared_get_dsl_status();
    return mp_obj_new_int(status);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_infrared_get_dsl_status_obj, waveshare_infrared_get_dsl_status);

// Define module globals
STATIC const mp_rom_map_elem_t waveshare_infrared_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_waveshare_infrared)},

    // functions
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&waveshare_infrared_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_remote_key), MP_ROM_PTR(&waveshare_infrared_get_remote_key_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_dsr_status), MP_ROM_PTR(&waveshare_infrared_get_dsr_status_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_dsl_status), MP_ROM_PTR(&waveshare_infrared_get_dsl_status_obj)},

    // constants
    {MP_ROM_QSTR(MP_QSTR_DSR_PIN), MP_ROM_INT(DSR_PIN)},
    {MP_ROM_QSTR(MP_QSTR_DSL_PIN), MP_ROM_INT(DSL_PIN)},
    {MP_ROM_QSTR(MP_QSTR_IR_PIN), MP_ROM_INT(IR_PIN)},

    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_UP), MP_ROM_INT(IR_REMOTE_UP)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_LEFT), MP_ROM_INT(IR_REMOTE_LEFT)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_OK), MP_ROM_INT(IR_REMOTE_OK)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_RIGHT), MP_ROM_INT(IR_REMOTE_RIGHT)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_DOWN), MP_ROM_INT(IR_REMOTE_DOWN)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_VOLUME_UP), MP_ROM_INT(IR_REMOTE_VOLUME_UP)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_VOLUME_DOWN), MP_ROM_INT(IR_REMOTE_VOLUME_DOWN)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_EQ), MP_ROM_INT(IR_REMOTE_EQ)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_NEXT), MP_ROM_INT(IR_REMOTE_NEXT)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_PREV), MP_ROM_INT(IR_REMOTE_PREV)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_PLAY_PAUSE), MP_ROM_INT(IR_REMOTE_PLAY_PAUSE)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_CHANNEL_DOWN), MP_ROM_INT(IR_REMOTE_CHANNEL_DOWN)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_CHANNEL), MP_ROM_INT(IR_REMOTE_CHANNEL)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_CHANNEL_UP), MP_ROM_INT(IR_REMOTE_CHANNEL_UP)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_100_PLUS), MP_ROM_INT(IR_REMOTE_100_PLUS)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_200_PLUS), MP_ROM_INT(IR_REMOTE_200_PLUS)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_0), MP_ROM_INT(IR_REMOTE_0)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_1), MP_ROM_INT(IR_REMOTE_1)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_3), MP_ROM_INT(IR_REMOTE_3)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_7), MP_ROM_INT(IR_REMOTE_7)},
    {MP_ROM_QSTR(MP_QSTR_IR_REMOTE_9), MP_ROM_INT(IR_REMOTE_9)},
};
STATIC MP_DEFINE_CONST_DICT(waveshare_infrared_module_globals, waveshare_infrared_module_globals_table);

const mp_obj_module_t waveshare_infrared_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&waveshare_infrared_module_globals,
};
MP_REGISTER_MODULE(MP_QSTR_waveshare_infrared, waveshare_infrared_user_cmodule);