/*
 * Waveshare Motor Native C Extension for MicroPython
 *
 */

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/mphal.h"
#include "motor.h"
#include <stdlib.h>
#include <string.h>

// Define STATIC if not already defined (MicroPython macro)
#ifndef STATIC
#define STATIC static
#endif

STATIC mp_obj_t waveshare_motor_init(void)
{
    motor_init();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_motor_init_obj, waveshare_motor_init);

STATIC mp_obj_t waveshare_motor_forward(mp_obj_t speed_obj)
{
    uint8_t speed = mp_obj_get_int(speed_obj);
    motor_forward(speed);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_motor_forward_obj, waveshare_motor_forward);

STATIC mp_obj_t waveshare_motor_backward(mp_obj_t speed_obj)
{
    uint8_t speed = mp_obj_get_int(speed_obj);
    motor_backward(speed);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_motor_backward_obj, waveshare_motor_backward);

STATIC mp_obj_t waveshare_motor_left(mp_obj_t speed_obj)
{
    uint8_t speed = mp_obj_get_int(speed_obj);
    motor_left(speed);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_motor_left_obj, waveshare_motor_left);

STATIC mp_obj_t waveshare_motor_right(mp_obj_t speed_obj)
{
    uint8_t speed = mp_obj_get_int(speed_obj);
    motor_right(speed);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_motor_right_obj, waveshare_motor_right);

STATIC mp_obj_t waveshare_motor_stop(void)
{
    motor_stop();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_motor_stop_obj, waveshare_motor_stop);

STATIC mp_obj_t waveshare_motor_set(mp_obj_t left_obj, mp_obj_t right_obj)
{
    int left = mp_obj_get_int(left_obj);
    int right = mp_obj_get_int(right_obj);
    motor_set(left, right);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(waveshare_motor_set_obj, waveshare_motor_set);

// Define module globals
STATIC const mp_rom_map_elem_t waveshare_motor_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_waveshare_motor)},
    // functions
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&waveshare_motor_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_forward), MP_ROM_PTR(&waveshare_motor_forward_obj)},
    {MP_ROM_QSTR(MP_QSTR_backward), MP_ROM_PTR(&waveshare_motor_backward_obj)},
    {MP_ROM_QSTR(MP_QSTR_left), MP_ROM_PTR(&waveshare_motor_left_obj)},
    {MP_ROM_QSTR(MP_QSTR_right), MP_ROM_PTR(&waveshare_motor_right_obj)},
    {MP_ROM_QSTR(MP_QSTR_stop), MP_ROM_PTR(&waveshare_motor_stop_obj)},
    {MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&waveshare_motor_set_obj)},
};
STATIC MP_DEFINE_CONST_DICT(waveshare_motor_module_globals, waveshare_motor_module_globals_table);

const mp_obj_module_t waveshare_motor_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&waveshare_motor_module_globals,
};
MP_REGISTER_MODULE(MP_QSTR_waveshare_motor, waveshare_motor_user_cmodule);