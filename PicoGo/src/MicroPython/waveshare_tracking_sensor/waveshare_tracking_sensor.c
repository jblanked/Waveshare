/*
 * Waveshare Tracking Sensor Native C Extension for MicroPython
 *
 */

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/mphal.h"
#include "tracking_sensor.h"
#include <stdlib.h>
#include <string.h>

// Define STATIC if not already defined (MicroPython macro)
#ifndef STATIC
#define STATIC static
#endif

STATIC mp_obj_t waveshare_tracking_sensor_init(void)
{
    tracking_sensor_init();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_tracking_sensor_init_obj, waveshare_tracking_sensor_init);

STATIC mp_obj_t waveshare_tracking_sensor_analog_read(void)
{
    uint16_t buffer[NUM_SENSORS];
    tracking_sensor_analog_read(buffer);
    mp_obj_t tuple[NUM_SENSORS];
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        tuple[i] = mp_obj_new_int(buffer[i]);
    }
    return mp_obj_new_list(NUM_SENSORS, tuple); // return as a list
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_tracking_sensor_analog_read_obj, waveshare_tracking_sensor_analog_read);

STATIC mp_obj_t waveshare_tracking_sensor_calibrate(void)
{
    tracking_sensor_calibrate();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_tracking_sensor_calibrate_obj, waveshare_tracking_sensor_calibrate);

STATIC mp_obj_t waveshare_tracking_sensor_get_calibrated_min(void)
{
    uint16_t buffer[NUM_SENSORS];
    tracking_sensor_get_calibrated_min(buffer);
    mp_obj_t tuple[NUM_SENSORS];
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        tuple[i] = mp_obj_new_int(buffer[i]);
    }
    return mp_obj_new_list(NUM_SENSORS, tuple); // return as a list
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_tracking_sensor_get_calibrated_min_obj, waveshare_tracking_sensor_get_calibrated_min);

STATIC mp_obj_t waveshare_tracking_sensor_get_calibrated_max(void)
{
    uint16_t buffer[NUM_SENSORS];
    tracking_sensor_get_calibrated_max(buffer);
    mp_obj_t tuple[NUM_SENSORS];
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        tuple[i] = mp_obj_new_int(buffer[i]);
    }
    return mp_obj_new_list(NUM_SENSORS, tuple); // return as a list
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_tracking_sensor_get_calibrated_max_obj, waveshare_tracking_sensor_get_calibrated_max);

STATIC mp_obj_t waveshare_tracking_sensor_fixed_calibration(void)
{
    tracking_sensor_fixed_calibration();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_tracking_sensor_fixed_calibration_obj, waveshare_tracking_sensor_fixed_calibration);

STATIC mp_obj_t waveshare_tracking_sensor_read_calibrated(void)
{
    uint16_t buffer[NUM_SENSORS];
    tracking_sensor_read_calibrated(buffer);
    mp_obj_t tuple[NUM_SENSORS];
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        tuple[i] = mp_obj_new_int(buffer[i]);
    }
    return mp_obj_new_list(NUM_SENSORS, tuple); // return as a list
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_tracking_sensor_read_calibrated_obj, waveshare_tracking_sensor_read_calibrated);

STATIC mp_obj_t waveshare_tracking_sensor_read_line(mp_obj_t position_obj, mp_obj_t white_line_obj)
{
    uint16_t position;
    uint16_t sensor_values[NUM_SENSORS];
    bool white_line = mp_obj_is_true(white_line_obj);

    tracking_sensor_read_line(&position, sensor_values, white_line);

    mp_obj_t sensor_tuple[NUM_SENSORS];
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        sensor_tuple[i] = mp_obj_new_int(sensor_values[i]);
    }

    mp_obj_t result_tuple[2];
    result_tuple[0] = mp_obj_new_int(position);
    result_tuple[1] = mp_obj_new_list(NUM_SENSORS, sensor_tuple);

    return mp_obj_new_tuple(2, result_tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(waveshare_tracking_sensor_read_line_obj, waveshare_tracking_sensor_read_line);

STATIC const mp_rom_map_elem_t waveshare_tracking_sensor_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_waveshare_tracking_sensor)},
    // functions
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&waveshare_tracking_sensor_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_analog_read), MP_ROM_PTR(&waveshare_tracking_sensor_analog_read_obj)},
    {MP_ROM_QSTR(MP_QSTR_calibrate), MP_ROM_PTR(&waveshare_tracking_sensor_calibrate_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_calibrated_min), MP_ROM_PTR(&waveshare_tracking_sensor_get_calibrated_min_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_calibrated_max), MP_ROM_PTR(&waveshare_tracking_sensor_get_calibrated_max_obj)},
    {MP_ROM_QSTR(MP_QSTR_fixed_calibration), MP_ROM_PTR(&waveshare_tracking_sensor_fixed_calibration_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_calibrated), MP_ROM_PTR(&waveshare_tracking_sensor_read_calibrated_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_line), MP_ROM_PTR(&waveshare_tracking_sensor_read_line_obj)},
    // constants
    {MP_ROM_QSTR(MP_QSTR_NUM_SENSORS), MP_ROM_INT(NUM_SENSORS)},
};
STATIC MP_DEFINE_CONST_DICT(waveshare_tracking_sensor_module_globals, waveshare_tracking_sensor_module_globals_table);

const mp_obj_module_t waveshare_tracking_sensor_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&waveshare_tracking_sensor_module_globals,
};
MP_REGISTER_MODULE(MP_QSTR_waveshare_tracking_sensor, waveshare_tracking_sensor_user_cmodule);