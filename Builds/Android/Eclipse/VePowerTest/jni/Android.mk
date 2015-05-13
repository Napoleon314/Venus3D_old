LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := VePowerTest

ifeq ($(Configuration),Debug)
LOCAL_CFLAGS := -O0 -g -Wall -Werror -DVE_DEBUG -DVE_PLATFORM_ANDROID -DVE_STATIC_LIB
endif

ifeq ($(Configuration),Release)
LOCAL_CFLAGS := -O3 -Wall -Werror -DVE_RELEASE -DVE_PLATFORM_ANDROID -DVE_STATIC_LIB
endif

LOCAL_CFLAGS += -DVE_PLATFORM_ANDROID -DVE_STATIC_LIB
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS) -std=c++1y -fno-rtti

LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%.c,%.c,$(wildcard $(LOCAL_PATH)/*.c))
LOCAL_SRC_FILES += $(patsubst $(LOCAL_PATH)/%.cpp,%.cpp,$(wildcard $(LOCAL_PATH)/*.cpp))
LOCAL_SRC_FILES += $(patsubst $(LOCAL_PATH)/../../../../../Tests/VePowerTest/%.cpp,../../../../../Tests/VePowerTest/%.cpp,$(wildcard $(LOCAL_PATH)/../../../../../Tests/VePowerTest/*.cpp))

LOCAL_LDLIBS    := -llog -landroid
LOCAL_STATIC_LIBRARIES := VePower

include $(BUILD_SHARED_LIBRARY)

$(call import-module,Source/VePower)
