WAVESHARE_QMI_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(WAVESHARE_QMI_MOD_DIR)/waveshare_qmi.c
SRC_USERMOD += $(WAVESHARE_QMI_MOD_DIR)/qmi.c
# We can add our module folder to include paths if needed
CFLAGS_USERMOD += -I$(WAVESHARE_QMI_MOD_DIR)