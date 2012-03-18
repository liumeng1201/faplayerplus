LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_MODULE := compat

LOCAL_CFLAGS += \
    -std=c99 \
    -DHAVE_CONFIG_H

LOCAL_C_INCLUDES += \
    $(VLCROOT) \
    $(VLCROOT)/include \
    $(VLCROOT)/src

LOCAL_SRC_FILES := \
    getdelim.c \
    pthread-cancel.c \
    swab.c \
    tdestroy.c

include $(BUILD_STATIC_LIBRARY)

