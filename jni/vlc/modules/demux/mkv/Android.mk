
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libmkv_plugin

LOCAL_CFLAGS += \
    -DHAVE_CONFIG_H \
    -DMODULE_STRING=\"mkv\" \
    -DMODULE_NAME=mkv

LOCAL_C_INCLUDES += \
    $(VLCROOT) \
    $(VLCROOT)/include \
    $(EXTROOT)/libebml \
    $(EXTROOT)/libmatroska

LOCAL_SRC_FILES := \
	chapter_command.cpp \
	chapters.cpp \
	demux.cpp \
	Ebml_parser.cpp \
	matroska_segment.cpp \
	matroska_segment_parse.cpp \
	mkv.cpp \
	stream_io_callback.cpp \
	util.cpp \
	virtual_segment.cpp

include $(BUILD_STATIC_LIBRARY)

