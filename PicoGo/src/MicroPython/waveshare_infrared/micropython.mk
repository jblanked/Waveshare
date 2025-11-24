WAVESHARE_INFRARED_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(WAVESHARE_INFRARED_MOD_DIR)/waveshare_infrared.c
SRC_USERMOD += $(WAVESHARE_INFRARED_MOD_DIR)/infrared.c
# We can add our module folder to include paths if needed
CFLAGS_USERMOD += -I$(WAVESHARE_INFRARED_MOD_DIR)