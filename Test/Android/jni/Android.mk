LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Test
LOCAL_CFLAGS    := -std=c++11
LOCAL_CPP_FEATURES += exceptions

LOCAL_SRC_FILES := com_thirdpartyninjas_test_TestJni.cpp
LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)
