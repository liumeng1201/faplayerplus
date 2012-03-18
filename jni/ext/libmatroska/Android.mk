
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libmatroska

LOCAL_CFLAGS += -Wall -Wno-unknown-pragmas -fno-gnu-keywords -D_GNU_SOURCE -Wshadow

LOCAL_C_INCLUDES += $(EXTROOT)/libebml

LOCAL_SRC_FILES := \
    src/FileKax.cpp \
    src/KaxAttached.cpp \
    src/KaxAttachments.cpp \
    src/KaxBlock.cpp \
    src/KaxBlockData.cpp \
    src/KaxCluster.cpp \
    src/KaxContexts.cpp \
    src/KaxCues.cpp \
    src/KaxCuesData.cpp \
    src/KaxInfoData.cpp \
    src/KaxSeekHead.cpp \
    src/KaxSegment.cpp \
    src/KaxSemantic.cpp \
    src/KaxTracks.cpp \
    src/KaxVersion.cpp

include $(BUILD_STATIC_LIBRARY)

