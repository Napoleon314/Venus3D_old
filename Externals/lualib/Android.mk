LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := lualib

ifeq ($(Configuration),Debug)
LOCAL_CFLAGS := -O0 -g -Wall -Werror
endif

ifeq ($(Configuration),Release)
LOCAL_CFLAGS := -O3 -Wall -Werror
endif

LOCAL_CFLAGS += -DLUA_USE_POSIX -DVE_PLATFORM_ANDROID

LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%.c,%.c,$(wildcard $(LOCAL_PATH)/*.c))
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)
