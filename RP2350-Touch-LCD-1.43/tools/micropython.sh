#!/bin/sh
# Script to build and install the MicroPython version of Waveshare 

echo "Initializing and preparing build environment..."

# set your locations
micropython_dir="/Users/user/pico/micropython/ports/rp2"
waveshare_dir="/Users/user/Desktop/Waveshare"

DIRECTORY="$waveshare_dir/RP2350-Touch-LCD-1.43/builds/MicroPython"

if [ ! -d "$DIRECTORY" ]; then
  mkdir -p "$DIRECTORY"
fi

rm -rf "$waveshare_dir"/RP2350-Touch-LCD-1.43/builds/MicroPython/Waveshare-RP2350-Touch-LCD-1.43.uf2

echo "Building MicroPython Waveshare firmware..."

# copy waveshare modules file to micropython modules directory
rm -rf "$micropython_dir"/modules/waveshare_modules.cmake
rm -rf "$micropython_dir"/modules/waveshare_battery
rm -rf "$micropython_dir"/modules/waveshare_lcd
rm -rf "$micropython_dir"/modules/waveshare_qmi
rm -rf "$micropython_dir"/modules/waveshare_touch
cp "$waveshare_dir"/RP2350-Touch-LCD-1.43/src/MicroPython/waveshare_modules.cmake "$micropython_dir"/modules/waveshare_modules.cmake
cp -r "$waveshare_dir"/RP2350-Touch-LCD-1.43/src/MicroPython/waveshare_battery "$micropython_dir"/modules/waveshare_battery
cp -r "$waveshare_dir"/RP2350-Touch-LCD-1.43/src/MicroPython/waveshare_lcd "$micropython_dir"/modules/waveshare_lcd
cp -r "$waveshare_dir"/RP2350-Touch-LCD-1.43/src/MicroPython/waveshare_qmi "$micropython_dir"/modules/waveshare_qmi
cp -r "$waveshare_dir"/RP2350-Touch-LCD-1.43/src/MicroPython/waveshare_touch "$micropython_dir"/modules/waveshare_touch

# move to the micropython rp2 port directory
cd "$micropython_dir"

echo "Starting build process..."

# Pico 2
make BOARD=RPI_PICO2 USER_C_MODULES="$micropython_dir"/modules/waveshare_modules.cmake
cp "$micropython_dir"/build-RPI_PICO2/firmware.uf2 "$waveshare_dir"/RP2350-Touch-LCD-1.43/builds/MicroPython/Waveshare-RP2350-Touch-LCD-1.43.uf2
echo "Pico 2 build complete."
