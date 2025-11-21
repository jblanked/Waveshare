/*
 * Waveshare Qmi Native C Extension for MicroPython
 *
 */

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/mphal.h"
#include "qmi.h"
#include <stdlib.h>
#include <string.h>

// Define STATIC if not already defined (MicroPython macro)
#ifndef STATIC
#define STATIC static
#endif

// Function to initialize QMI8658
STATIC mp_obj_t waveshare_qmi_init(void)
{
    bool result = qmi_init();
    return mp_obj_new_bool(result);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_qmi_init_obj, waveshare_qmi_init);

// Function to set wake on motion
STATIC mp_obj_t waveshare_qmi_set_wake_on_motion(size_t n_args, const mp_obj_t *args)
{
    // Arguments: enable
    if (n_args != 1)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("set_wake_on_motion requires 1 argument: enable"));
    }
    bool enable = mp_obj_is_true(args[0]);
    qmi_set_wake_on_motion(enable);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(waveshare_qmi_set_wake_on_motion_obj, 1, 1, waveshare_qmi_set_wake_on_motion);

// Function ro read the temperature
STATIC mp_obj_t waveshare_qmi_read_temperature(void)
{
    float temperature = qmi_read_temperature();
    return mp_obj_new_float(temperature);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_qmi_read_temperature_obj, waveshare_qmi_read_temperature);

// function to read accelerometer data
STATIC mp_obj_t waveshare_qmi_read_acc_xyz(void)
{
    float acc_xyz[3];
    qmi_read_acc_xyz(acc_xyz);
    mp_obj_t tuple[3];
    tuple[0] = mp_obj_new_float(acc_xyz[0]);
    tuple[1] = mp_obj_new_float(acc_xyz[1]);
    tuple[2] = mp_obj_new_float(acc_xyz[2]);
    return mp_obj_new_tuple(3, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_qmi_read_acc_xyz_obj, waveshare_qmi_read_acc_xyz);

// function read ae data
STATIC mp_obj_t waveshare_qmi_read_ae(void)
{
    float quat[4];
    float velocity[3];
    qmi_read_ae(quat, velocity);
    mp_obj_t tuple[7];
    tuple[0] = mp_obj_new_float(quat[0]);
    tuple[1] = mp_obj_new_float(quat[1]);
    tuple[2] = mp_obj_new_float(quat[2]);
    tuple[3] = mp_obj_new_float(quat[3]);
    tuple[4] = mp_obj_new_float(velocity[0]);
    tuple[5] = mp_obj_new_float(velocity[1]);
    tuple[6] = mp_obj_new_float(velocity[2]);
    return mp_obj_new_tuple(7, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_qmi_read_ae_obj, waveshare_qmi_read_ae);

// function to read gyro data
STATIC mp_obj_t waveshare_qmi_read_gyro_xyz(void)
{
    float gyro_xyz[3];
    qmi_read_gyro_xyz(gyro_xyz);
    mp_obj_t tuple[3];
    tuple[0] = mp_obj_new_float(gyro_xyz[0]);
    tuple[1] = mp_obj_new_float(gyro_xyz[1]);
    tuple[2] = mp_obj_new_float(gyro_xyz[2]);
    return mp_obj_new_tuple(3, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_qmi_read_gyro_xyz_obj, waveshare_qmi_read_gyro_xyz);

// function to read xyz data
STATIC mp_obj_t waveshare_qmi_read_xyz(void)
{
    float acc[3];
    float gyro[3];
    unsigned int tim_count;
    qmi_read_xyz(acc, gyro, &tim_count);
    mp_obj_t tuple[7];
    tuple[0] = mp_obj_new_float(acc[0]);
    tuple[1] = mp_obj_new_float(acc[1]);
    tuple[2] = mp_obj_new_float(acc[2]);
    tuple[3] = mp_obj_new_float(gyro[0]);
    tuple[4] = mp_obj_new_float(gyro[1]);
    tuple[5] = mp_obj_new_float(gyro[2]);
    tuple[6] = mp_obj_new_int_from_uint(tim_count);
    return mp_obj_new_tuple(7, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_qmi_read_xyz_obj, waveshare_qmi_read_xyz);

// function to read raw xyz data
STATIC mp_obj_t waveshare_qmi_read_xyz_raw(void)
{
    short raw_acc_xyz[3];
    short raw_gyro_xyz[3];
    unsigned int tim_count;
    qmi_read_xyz_raw(raw_acc_xyz, raw_gyro_xyz, &tim_count);
    mp_obj_t tuple[7];
    tuple[0] = mp_obj_new_int(raw_acc_xyz[0]);
    tuple[1] = mp_obj_new_int(raw_acc_xyz[1]);
    tuple[2] = mp_obj_new_int(raw_acc_xyz[2]);
    tuple[3] = mp_obj_new_int(raw_gyro_xyz[0]);
    tuple[4] = mp_obj_new_int(raw_gyro_xyz[1]);
    tuple[5] = mp_obj_new_int(raw_gyro_xyz[2]);
    tuple[6] = mp_obj_new_int_from_uint(tim_count);
    return mp_obj_new_tuple(7, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_qmi_read_xyz_raw_obj, waveshare_qmi_read_xyz_raw);

// Define all functions in the module
STATIC const mp_rom_map_elem_t waveshare_qmi_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_waveshare_qmi)},
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&waveshare_qmi_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_set_wake_on_motion), MP_ROM_PTR(&waveshare_qmi_set_wake_on_motion_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_temperature), MP_ROM_PTR(&waveshare_qmi_read_temperature_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_acc_xyz), MP_ROM_PTR(&waveshare_qmi_read_acc_xyz_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_ae), MP_ROM_PTR(&waveshare_qmi_read_ae_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_gyro_xyz), MP_ROM_PTR(&waveshare_qmi_read_gyro_xyz_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_xyz), MP_ROM_PTR(&waveshare_qmi_read_xyz_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_xyz_raw), MP_ROM_PTR(&waveshare_qmi_read_xyz_raw_obj)},
};

STATIC MP_DEFINE_CONST_DICT(waveshare_qmi_module_globals, waveshare_qmi_module_globals_table);

// module definition
const mp_obj_module_t waveshare_qmi_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&waveshare_qmi_module_globals,
};

// Register the module with MicroPython
MP_REGISTER_MODULE(MP_QSTR_waveshare_qmi, waveshare_qmi_user_cmodule);