#!/bin/sh
# Script to build and install the MicroPython version of Waveshare 

echo "Initializing and preparing build environment..."

# set your locations
micropython_dir="/Users/user/pico/micropython/ports/rp2"
waveshare_dir="/Users/user/Desktop/Waveshare"

DIRECTORY="$waveshare_dir/PicoGo/builds/MicroPython"

if [ ! -d "$DIRECTORY" ]; then
  mkdir -p "$DIRECTORY"
fi

rm -rf "$waveshare_dir"/PicoGo/builds/MicroPython/Waveshare-PicoGo.uf2

echo "Building MicroPython Waveshare firmware..."

# copy waveshare modules file to micropython modules directory
rm -rf "$micropython_dir"/modules/waveshare_modules.cmake
rm -rf "$micropython_dir"/modules/waveshare_battery
rm -rf "$micropython_dir"/modules/waveshare_bluetooth
rm -rf "$micropython_dir"/modules/waveshare_infrared
rm -rf "$micropython_dir"/modules/waveshare_lcd
rm -rf "$micropython_dir"/modules/waveshare_motor
rm -rf "$micropython_dir"/modules/waveshare_tracking_sensor
rm -rf "$micropython_dir"/modules/waveshare_ultrasonic_sensor
cp "$waveshare_dir"/PicoGo/src/MicroPython/waveshare_modules.cmake "$micropython_dir"/modules/waveshare_modules.cmake
cp -r "$waveshare_dir"/PicoGo/src/MicroPython/waveshare_battery "$micropython_dir"/modules/waveshare_battery
cp -r "$waveshare_dir"/PicoGo/src/MicroPython/waveshare_bluetooth "$micropython_dir"/modules/waveshare_bluetooth
cp -r "$waveshare_dir"/PicoGo/src/MicroPython/waveshare_infrared "$micropython_dir"/modules/waveshare_infrared
cp -r "$waveshare_dir"/PicoGo/src/MicroPython/waveshare_lcd "$micropython_dir"/modules/waveshare_lcd
cp -r "$waveshare_dir"/PicoGo/src/MicroPython/waveshare_motor "$micropython_dir"/modules/waveshare_motor
cp -r "$waveshare_dir"/PicoGo/src/MicroPython/waveshare_tracking_sensor "$micropython_dir"/modules/waveshare_tracking_sensor
cp -r "$waveshare_dir"/PicoGo/src/MicroPython/waveshare_ultrasonic_sensor "$micropython_dir"/modules/waveshare_ultrasonic_sensor

# move to the micropython rp2 port directory
cd "$micropython_dir"

echo "Starting build process..."

# Pico 2
make BOARD=RPI_PICO2 USER_C_MODULES="$micropython_dir"/modules/waveshare_modules.cmake
cp "$micropython_dir"/build-RPI_PICO2/firmware.uf2 "$waveshare_dir"/PicoGo/builds/MicroPython/Waveshare-PicoGo.uf2
echo "Pico 2 build complete."
