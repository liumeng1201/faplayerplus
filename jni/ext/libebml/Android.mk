
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libebml

LOCAL_CFLAGS += -Wall -Wno-unknown-pragmas -fno-gnu-keywords -Wshadow

LOCAL_SRC_FILES := \
    src/EbmlBinary.cpp \
    src/EbmlContexts.cpp \
    src/EbmlCrc32.cpp \
    src/EbmlDate.cpp \
    src/EbmlDummy.cpp \
    src/EbmlElement.cpp \
    src/EbmlFloat.cpp \
    src/EbmlHead.cpp \
    src/EbmlMaster.cpp \
    src/EbmlSInteger.cpp \
    src/EbmlStream.cpp \
    src/EbmlString.cpp \
    src/EbmlSubHead.cpp \
    src/EbmlUInteger.cpp \
    src/EbmlUnicodeString.cpp \
    src/EbmlVersion.cpp \
    src/EbmlVoid.cpp \
    src/IOCallback.cpp \
    src/MemIOCallback.cpp \
    src/StdIOCallback.cpp

include $(BUILD_STATIC_LIBRARY)

