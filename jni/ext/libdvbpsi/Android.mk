
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libdvbpsi

LOCAL_CFLAGS += \
     -std=gnu99 -DHAVE_CONFIG_H -DDVBPSI_DIST

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)

LOCAL_SRC_FILES := \
    src/demux.c \
    src/descriptor.c \
    src/dvbpsi.c \
    src/psi.c \
    src/descriptors/dr_02.c \
    src/descriptors/dr_03.c \
    src/descriptors/dr_04.c \
    src/descriptors/dr_05.c \
    src/descriptors/dr_06.c \
    src/descriptors/dr_07.c \
    src/descriptors/dr_08.c \
    src/descriptors/dr_09.c \
    src/descriptors/dr_0a.c \
    src/descriptors/dr_0b.c \
    src/descriptors/dr_0c.c \
    src/descriptors/dr_0d.c \
    src/descriptors/dr_0e.c \
    src/descriptors/dr_0f.c \
    src/descriptors/dr_42.c \
    src/descriptors/dr_43.c \
    src/descriptors/dr_44.c \
    src/descriptors/dr_45.c \
    src/descriptors/dr_47.c \
    src/descriptors/dr_48.c \
    src/descriptors/dr_4d.c \
    src/descriptors/dr_4e.c \
    src/descriptors/dr_52.c \
    src/descriptors/dr_55.c \
    src/descriptors/dr_56.c \
    src/descriptors/dr_58.c \
    src/descriptors/dr_59.c \
    src/descriptors/dr_5a.c \
    src/descriptors/dr_69.c \
    src/descriptors/dr_8a.c \
    src/tables/bat.c \
    src/tables/cat.c \
    src/tables/eit.c \
    src/tables/nit.c \
    src/tables/pat.c \
    src/tables/pmt.c \
    src/tables/sdt.c \
    src/tables/sis.c \
    src/tables/tot.c

include $(BUILD_STATIC_LIBRARY)

