/*
 * Waveshare Ultrasonic Sensor Native C Extension for MicroPython
 *
 */

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/mphal.h"
#include "ultrasonic_sensor.h"
#include <stdlib.h>
#include <string.h>

// Define STATIC if not already defined (MicroPython macro)
#ifndef STATIC
#define STATIC static
#endif

STATIC mp_obj_t waveshare_ultrasonic_sensor_init(void)
{
    ultrasonic_sensor_init();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_ultrasonic_sensor_init_obj, waveshare_ultrasonic_sensor_init);

STATIC mp_obj_t waveshare_ultrasonic_get_distance(void)
{
    float distance = ultrasonic_get_distance();
    return mp_obj_new_float(distance);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_ultrasonic_get_distance_obj, waveshare_ultrasonic_get_distance);

// Define module globals
STATIC const mp_rom_map_elem_t waveshare_ultrasonic_sensor_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_waveshare_ultrasonic_sensor)},
    // functions
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&waveshare_ultrasonic_sensor_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_distance), MP_ROM_PTR(&waveshare_ultrasonic_get_distance_obj)},
    // constants
    {MP_ROM_QSTR(MP_QSTR_ECHO_PIN), MP_ROM_INT(ECHO_PIN)},
    {MP_ROM_QSTR(MP_QSTR_TRIG_PIN), MP_ROM_INT(TRIG_PIN)},
};
STATIC MP_DEFINE_CONST_DICT(waveshare_ultrasonic_sensor_module_globals, waveshare_ultrasonic_sensor_module_globals_table);

const mp_obj_module_t waveshare_ultrasonic_sensor_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&waveshare_ultrasonic_sensor_module_globals,
};
MP_REGISTER_MODULE(MP_QSTR_waveshare_ultrasonic_sensor, waveshare_ultrasonic_sensor_user_cmodule);