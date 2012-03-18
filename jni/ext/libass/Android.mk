
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libass

LOCAL_CFLAGS += \
    -DHAVE_CONFIG_H

LOCAL_CFLAGS += \
    -DICONV_CONST=

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)

LOCAL_C_INCLUDES += \
    $(EXTROOT)/iconv/include \
    $(EXTROOT)/freetype/include

LOCAL_SRC_FILES := \
    libass/ass.c \
    libass/ass_bitmap.c \
    libass/ass_cache.c \
    libass/ass_drawing.c \
    libass/ass_font.c \
    libass/ass_fontconfig.c \
    libass/ass_library.c \
    libass/ass_parse.c \
    libass/ass_render.c \
    libass/ass_render_api.c \
    libass/ass_strtod.c \
    libass/ass_utils.c

include $(BUILD_STATIC_LIBRARY)

