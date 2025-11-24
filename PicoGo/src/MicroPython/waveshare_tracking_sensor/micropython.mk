WAVESHARE_TRACKING_SENSOR_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(WAVESHARE_TRACKING_SENSOR_MOD_DIR)/waveshare_tracking_sensor.c
SRC_USERMOD += $(WAVESHARE_TRACKING_SENSOR_MOD_DIR)/tracking_sensor.c
# We can add our module folder to include paths if needed
CFLAGS_USERMOD += -I$(WAVESHARE_TRACKING_SENSOR_MOD_DIR)