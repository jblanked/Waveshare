WAVESHARE_MOTOR_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(WAVESHARE_MOTOR_MOD_DIR)/waveshare_motor.c
SRC_USERMOD += $(WAVESHARE_MOTOR_MOD_DIR)/motor.c
# We can add our module folder to include paths if needed
CFLAGS_USERMOD += -I$(WAVESHARE_MOTOR_MOD_DIR)