WAVESHARE_BLUETOOTH_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(WAVESHARE_BLUETOOTH_MOD_DIR)/waveshare_bluetooth.c
SRC_USERMOD += $(WAVESHARE_BLUETOOTH_MOD_DIR)/bluetooth.c
# We can add our module folder to include paths if needed
CFLAGS_USERMOD += -I$(WAVESHARE_BLUETOOTH_MOD_DIR)