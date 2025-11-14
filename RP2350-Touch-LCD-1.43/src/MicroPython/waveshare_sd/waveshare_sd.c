/*
 * Waveshare SD Native C Extension for MicroPython
 *
 */

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/mphal.h"
#include "fat32.h"
#include "py/mperrno.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

// Define STATIC if not already defined (MicroPython macro)
#ifndef STATIC
#define STATIC static
#endif

#define PRINT(...) mp_printf(&mp_plat_print, __VA_ARGS__)

// Function to initialize the SD card
STATIC mp_obj_t waveshare_sd_init(void)
{
    fat32_init();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_sd_init_obj, waveshare_sd_init);

// Function to create a file on the SD card
STATIC mp_obj_t waveshare_sd_create_file(mp_obj_t filepath_obj)
{
    const char *filePath = mp_obj_str_get_str(filepath_obj);
    fat32_file_t file;
    bool status = true;
    if (fat32_open(&file, filePath) != FAT32_OK)
    {
        if (fat32_create(&file, filePath) != FAT32_OK)
        {
            printf("Failed to create file.\n");
            status = false;
        }
    }
    fat32_close(&file);
    return mp_obj_new_bool(status);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_sd_create_file_obj, waveshare_sd_create_file);

// Function to create a directory on the SD card
STATIC mp_obj_t waveshare_sd_create_directory(mp_obj_t directory_path_obj)
{
    const char *dirPath = mp_obj_str_get_str(directory_path_obj);
    fat32_file_t dir;
    if (fat32_open(&dir, dirPath) == FAT32_OK)
    {
        // Directory already exists, check if it's actually a directory
        if (dir.attributes & FAT32_ATTR_DIRECTORY)
        {
            fat32_close(&dir);
            return mp_const_true;
        }
        else
        {
            fat32_close(&dir);
            PRINT("Path exists but is not a directory.\n");
            mp_raise_OSError(MP_EEXIST);
        }
    }
    // Directory doesn't exist, try to create it
    fat32_error_t err = fat32_dir_create(&dir, dirPath);
    if (err != FAT32_OK)
    {
        PRINT("Failed to create directory: %s\n", fat32_error_string(err));
        fat32_close(&dir);
        mp_raise_OSError(MP_EIO);
    }
    fat32_close(&dir);
    return mp_const_true;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_sd_create_directory_obj, waveshare_sd_create_directory);

// Function to check if a file or directory exists
STATIC mp_obj_t waveshare_sd_exists(mp_obj_t path_obj)
{
    const char *path = mp_obj_str_get_str(path_obj);
    fat32_file_t file;
    if (fat32_open(&file, path) == FAT32_OK)
    {
        fat32_close(&file);
        return mp_const_true;
    }
    return mp_const_false;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_sd_exists_obj, waveshare_sd_exists);

// Function to get the file size
STATIC mp_obj_t waveshare_sd_get_file_size(mp_obj_t filepath_obj)
{
    const char *filePath = mp_obj_str_get_str(filepath_obj);
    fat32_file_t file;
    if (fat32_open(&file, filePath) != FAT32_OK)
    {
        PRINT("Failed to open file for reading.\n");
        return mp_obj_new_int(0);
    }
    const uint32_t size = fat32_size(&file);
    fat32_close(&file);
    return mp_obj_new_int_from_uint(size);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_sd_get_file_size_obj, waveshare_sd_get_file_size);

// Function to get the free space of the SD card
STATIC mp_obj_t waveshare_sd_get_free_space(void)
{
    uint64_t free_space = 0;
    if (fat32_get_free_space(&free_space) != FAT32_OK)
    {
        PRINT("Failed to get free space.\n");
        return mp_obj_new_int(0);
    }
    return mp_obj_new_int_from_ull(free_space);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_sd_get_free_space_obj, waveshare_sd_get_free_space);

// Function to get the total space of the SD card
STATIC mp_obj_t waveshare_sd_get_total_space(void)
{
    uint64_t total_space = 0;
    if (fat32_get_total_space(&total_space) != FAT32_OK)
    {
        PRINT("Failed to get total space.\n");
        return mp_obj_new_int(0);
    }
    return mp_obj_new_int_from_ull(total_space);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_sd_get_total_space_obj, waveshare_sd_get_total_space);

// Functino to check if SD card is initialized
STATIC mp_obj_t waveshare_sd_is_initialized(void)
{
    return mp_obj_new_bool(fat32_is_ready());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(waveshare_sd_is_initialized_obj, waveshare_sd_is_initialized);

// Function to check if path is a directory
STATIC mp_obj_t waveshare_sd_is_directory(mp_obj_t path_obj)
{
    const char *dirPath = mp_obj_str_get_str(path_obj);
    fat32_file_t dir;
    if (fat32_open(&dir, dirPath) == FAT32_OK)
    {
        // Directory already exists, check if it's actually a directory
        mp_obj_t result = (dir.attributes & FAT32_ATTR_DIRECTORY) ? mp_const_true : mp_const_false;
        fat32_close(&dir);
        return result;
    }
    return mp_const_false;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_sd_is_directory_obj, waveshare_sd_is_directory);

// Function to check if path is a file
STATIC mp_obj_t waveshare_sd_is_file(mp_obj_t path_obj)
{
    const char *path = mp_obj_str_get_str(path_obj);
    fat32_file_t file;
    if (fat32_open(&file, path) == FAT32_OK)
    {
        // File exists, check if it's actually a file
        mp_obj_t result = (file.attributes & FAT32_ATTR_DIRECTORY) ? mp_const_false : mp_const_true;
        fat32_close(&file);
        return result;
    }
    return mp_const_false;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_sd_is_file_obj, waveshare_sd_is_file);

// Function to read a file
STATIC mp_obj_t waveshare_sd_read(size_t n_args, const mp_obj_t *args)
{
    // Arguments: File path, index (optional), count (optional)
    if (n_args < 1 || n_args > 3)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("read requires at least 1 argument: file_path, [index], [count]"));
    }
    uint32_t index = 0;
    uint32_t count = 0;
    if (n_args == 2)
    {
        index = mp_obj_get_int(args[1]);
    }
    if (n_args == 3)
    {
        count = mp_obj_get_int(args[2]);
    }
    const char *filePath = mp_obj_str_get_str(args[0]);
    fat32_file_t file;
    fat32_error_t err = fat32_open(&file, filePath);
    if (err != FAT32_OK)
    {
        PRINT("Failed to open file for reading: %s\n", fat32_error_string(err));
        mp_raise_OSError(MP_ENOENT);
    }
    if (index > 0)
    {
        if (fat32_seek(&file, index) != FAT32_OK)
        {
            fat32_close(&file);
            PRINT("Failed to seek to index %lu.\n", index);
            mp_raise_OSError(MP_EIO);
        }
    }
    const uint32_t size_of_buffer = count != 0 ? count : fat32_size(&file);
    uint8_t buffer[size_of_buffer];
    size_t bytes_read;
    const bool status = fat32_read(&file, buffer, size_of_buffer, &bytes_read) == FAT32_OK;
    fat32_close(&file);
    mp_obj_t result = status ? mp_obj_new_bytes(buffer, bytes_read) : mp_const_none;
    return result;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(waveshare_sd_read_obj, 1, 3, waveshare_sd_read);

// Function to remove a file
STATIC mp_obj_t waveshare_sd_remove(mp_obj_t filepath_obj)
{
    const char *filePath = mp_obj_str_get_str(filepath_obj);
    fat32_file_t file;
    if (fat32_open(&file, filePath) != FAT32_OK)
    {
        printf("File does not exist.\n");
        return mp_const_true;
    }
    const bool status = fat32_delete(filePath) == FAT32_OK;
    fat32_close(&file);
    return mp_obj_new_bool(status);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(waveshare_sd_remove_obj, waveshare_sd_remove);

// Function to rename a file
STATIC mp_obj_t waveshare_sd_rename(size_t n_args, const mp_obj_t *args)
{
    // Arguments: old_path, new_path
    if (n_args != 2)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("rename requires 2 arguments: old_path, new_path"));
    }
    const char *oldPath = mp_obj_str_get_str(args[0]);
    const char *newPath = mp_obj_str_get_str(args[1]);
    fat32_file_t file;
    if (fat32_open(&file, oldPath) != FAT32_OK)
    {
        printf("Failed to open file for renaming.\n");
        mp_raise_OSError(MP_ENOENT);
    }
    const bool status = fat32_rename(oldPath, newPath) == FAT32_OK;
    fat32_close(&file);
    return mp_obj_new_bool(status);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(waveshare_sd_rename_obj, 2, 2, waveshare_sd_rename);

// Function to write to a file
STATIC mp_obj_t waveshare_sd_write(size_t n_args, const mp_obj_t *args)
{
    // Arguments: file_path, data, overwrite
    if (n_args != 3)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("write requires 3 arguments: file_path, data, overwrite"));
    }
    const char *filePath = mp_obj_str_get_str(args[0]);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[1], &bufinfo, MP_BUFFER_READ);
    const bool overwrite = mp_obj_is_true(args[2]);
    fat32_file_t file;
    if (fat32_open(&file, filePath) != FAT32_OK)
    {
        if (fat32_create(&file, filePath) != FAT32_OK)
        {
            PRINT("Failed to create file.\n");
            mp_raise_OSError(MP_EIO);
        }
    }
    else
    {
        if (overwrite)
        {
            if (fat32_delete(filePath) != FAT32_OK)
            {
                printf("Failed to delete existing file.\n");
                mp_raise_OSError(MP_EIO);
            }
            if (fat32_create(&file, filePath) != FAT32_OK)
            {
                printf("Failed to create new file.\n");
                mp_raise_OSError(MP_EIO);
            }
            if (fat32_open(&file, filePath) != FAT32_OK)
            {
                printf("Failed to open new file.\n");
                mp_raise_OSError(MP_ENOENT);
            }
        }
    }
    size_t bytes_written;
    const bool status = fat32_write(&file, bufinfo.buf, bufinfo.len, &bytes_written) == FAT32_OK;
    fat32_close(&file);
    return mp_obj_new_bool(status && bytes_written == bufinfo.len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(waveshare_sd_write_obj, 3, 3, waveshare_sd_write);

// Define module globals
STATIC const mp_rom_map_elem_t waveshare_sd_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_waveshare_sd)},
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&waveshare_sd_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_create_file), MP_ROM_PTR(&waveshare_sd_create_file_obj)},
    {MP_ROM_QSTR(MP_QSTR_create_directory), MP_ROM_PTR(&waveshare_sd_create_directory_obj)},
    {MP_ROM_QSTR(MP_QSTR_exists), MP_ROM_PTR(&waveshare_sd_exists_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_file_size), MP_ROM_PTR(&waveshare_sd_get_file_size_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_free_space), MP_ROM_PTR(&waveshare_sd_get_free_space_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_total_space), MP_ROM_PTR(&waveshare_sd_get_total_space_obj)},
    {MP_ROM_QSTR(MP_QSTR_is_directory), MP_ROM_PTR(&waveshare_sd_is_directory_obj)},
    {MP_ROM_QSTR(MP_QSTR_is_initialized), MP_ROM_PTR(&waveshare_sd_is_initialized_obj)},
    {MP_ROM_QSTR(MP_QSTR_is_file), MP_ROM_PTR(&waveshare_sd_is_file_obj)},
    {MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&waveshare_sd_read_obj)},
    {MP_ROM_QSTR(MP_QSTR_remove), MP_ROM_PTR(&waveshare_sd_remove_obj)},
    {MP_ROM_QSTR(MP_QSTR_rename), MP_ROM_PTR(&waveshare_sd_rename_obj)},
    {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&waveshare_sd_write_obj)},
};
STATIC MP_DEFINE_CONST_DICT(waveshare_sd_module_globals, waveshare_sd_module_globals_table);

// Define module
const mp_obj_module_t waveshare_sd_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&waveshare_sd_module_globals,
};

// Register the module to make it available in Python
MP_REGISTER_MODULE(MP_QSTR_waveshare_sd, waveshare_sd_user_cmodule);