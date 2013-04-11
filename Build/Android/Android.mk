LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := NinjaParty
LOCAL_CPPFLAGS    := -std=c++11
LOCAL_CPP_FEATURES += exceptions


include $(BUILD_STATIC_LIBRARY)
