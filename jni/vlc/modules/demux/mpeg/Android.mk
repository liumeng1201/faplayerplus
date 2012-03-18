
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := mpgv_plugin

LOCAL_CFLAGS += \
    -std=c99 \
    -DHAVE_CONFIG_H \
    -DMODULE_STRING=\"mpgv\" \
    -DMODULE_NAME=mpgv

LOCAL_C_INCLUDES += \
    $(VLCROOT) \
    $(VLCROOT)/include

LOCAL_SRC_FILES := \
    mpgv.c

include $(BUILD_STATIC_LIBRARY)

