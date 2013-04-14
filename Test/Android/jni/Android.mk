LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_LOCAL_PATH := $(LOCAL_PATH)

LOCAL_MODULE := Test
LOCAL_CFLAGS := -std=c++11
LOCAL_CPP_FEATURES += exceptions

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Dependencies/boost/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Dependencies/JsonCpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Dependencies/libzip
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Dependencies/OpenALSoft/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Dependencies/pugixml
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Dependencies/spine-c/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Dependencies/stb_image
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Dependencies/stb_vorbis
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Dependencies/wav_reader
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Dependencies/Wm5

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Include

LOCAL_SRC_FILES := com_thirdpartyninjas_test_TestJni.cpp

LOCAL_LDLIBS := -llog -lGLESv2 -lz -lOpenSLES
LOCAL_STATIC_LIBRARIES := NinjaParty OpenAL

include $(BUILD_SHARED_LIBRARY)

# include NinjaParty
include $(MY_LOCAL_PATH)/../../../Build/Android/Android.mk
include $(MY_LOCAL_PATH)/../../../Dependencies/OpenALSoft/Android.mk
