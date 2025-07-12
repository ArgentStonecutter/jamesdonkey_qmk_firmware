SIDE_LIGHT_DIR = common/side_rgb
SRC += \
     $(SIDE_LIGHT_DIR)/side_rgb.c \
     $(SIDE_LIGHT_DIR)/chyt_3528_spi.c

VPATH += $(TOP_DIR)/keyboards/jamesdonkey/$(SIDE_LIGHT_DIR)
