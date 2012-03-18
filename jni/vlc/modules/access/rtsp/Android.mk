
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := realrtsp_plugin

LOCAL_CFLAGS += \
    -std=c99 \
    -DHAVE_CONFIG_H \
    -DMODULE_STRING=\"realrtsp\" \
    -DMODULE_NAME=realrtsp

LOCAL_C_INCLUDES += \
    $(VLCROOT) \
    $(VLCROOT)/include \
    $(VLCROOT)/src

LOCAL_SRC_FILES := \
    access.c \
    real.c \
    real_asmrp.c \
    real_rmff.c \
    real_sdpplin.c \
    rtsp.c

include $(BUILD_STATIC_LIBRARY)

