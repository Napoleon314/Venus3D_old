LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := VePower

ifeq ($(Configuration),Debug)
LOCAL_CFLAGS := -O0 -g -Wall -Werror -DVE_DEBUG
endif

ifeq ($(Configuration),Release)
LOCAL_CFLAGS := -O3 -Wall -Werror -DVE_RELEASE
endif

LOCAL_CFLAGS += -DVE_PLATFORM_ANDROID -DVE_STATIC_LIB
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS) -std=c++1y -fno-rtti

LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%.cpp,%.cpp,$(wildcard $(LOCAL_PATH)/*.cpp))
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_STATIC_LIBRARIES := lualib

include $(BUILD_STATIC_LIBRARY)

$(call import-module,Externals/lualib)
