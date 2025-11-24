/*
 * Waveshare Bluetooth Native C Extension for MicroPython
 *
 */

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/mphal.h"
#include "bluetooth.h"
#include <stdlib.h>
#include <string.h>

// Define STATIC if not already defined (MicroPython macro)
#ifndef STATIC
#define STATIC static
#endif

static mp_obj_t waveshare_bluetooth_init(void)
{
    bluetooth_init();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_bluetooth_init_obj, waveshare_bluetooth_init);

static mp_obj_t waveshare_bluetooth_data_available(void)
{
    bool available = bluetooth_data_available();
    return mp_obj_new_bool(available);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_bluetooth_data_available_obj, waveshare_bluetooth_data_available);

static mp_obj_t waveshare_bluetooth_get_buffer(void)
{
    char buffer[UART_BUFFER_SIZE];
    int len = bluetooth_get_buffer(buffer, UART_BUFFER_SIZE);
    if (len < 0)
    {
        return mp_const_none;
    }
    return mp_obj_new_bytes((const byte *)buffer, len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_bluetooth_get_buffer_obj, waveshare_bluetooth_get_buffer);

// Define all functions in the module
STATIC const mp_rom_map_elem_t waveshare_bluetooth_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_waveshare_bluetooth)},
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&waveshare_bluetooth_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_data_available), MP_ROM_PTR(&waveshare_bluetooth_data_available_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_buffer), MP_ROM_PTR(&waveshare_bluetooth_get_buffer_obj)},
};
STATIC MP_DEFINE_CONST_DICT(waveshare_bluetooth_module_globals, waveshare_bluetooth_module_globals_table);

const mp_obj_module_t waveshare_bluetooth_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&waveshare_bluetooth_module_globals,
};
MP_REGISTER_MODULE(MP_QSTR_waveshare_bluetooth, waveshare_bluetooth_user_cmodule);