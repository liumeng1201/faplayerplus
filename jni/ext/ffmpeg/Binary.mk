
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := s_ffmpeg

LOCAL_CFLAGS += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

LOCAL_SRC_FILES := \
    ffmpeg.c \
    cmdutils.c

LOCAL_CFLAGS += -DHAVE_NEON=$(BUILD_WITH_NEON)

LOCAL_SHARED_LIBRARIES += vlccore

LOCAL_LDLIBS += -lz

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := t_ffmpeg

LOCAL_CFLAGS += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

LOCAL_SRC_FILES := \
    ffmpeg.c \
    cmdutils.c

LOCAL_CFLAGS += -DHAVE_NEON=$(BUILD_WITH_NEON)

LOCAL_STATIC_LIBRARIES += libavformat libavfilter libavcodec libavutil libswscale libavdevice

LOCAL_LDLIBS += -lz

include $(BUILD_EXECUTABLE)

