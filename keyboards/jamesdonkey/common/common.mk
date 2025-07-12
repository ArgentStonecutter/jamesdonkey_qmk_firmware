OPT_DEFS += -DFACTORY_TEST_ENABLE #-DAPDAPTIVE_NKRO_ENABLE

VENDOR_COMMON_DIR = common
SRC += \
    $(VENDOR_COMMON_DIR)/task.c \
    $(VENDOR_COMMON_DIR)/common.c \
    $(VENDOR_COMMON_DIR)/factory_test.c

VPATH += $(TOP_DIR)/keyboards/jamesdonkey/$(VENDOR_COMMON_DIR)

