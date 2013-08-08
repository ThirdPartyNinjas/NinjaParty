LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := OpenAL

LOCAL_CFLAGS := -DAL_ALEXT_PROTOTYPES

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/OpenAL32/Include

LOCAL_SRC_FILES :=\
	OpenAL32/alAuxEffectSlot.c \
	OpenAL32/alBuffer.c \
	OpenAL32/alEffect.c \
	OpenAL32/alError.c \
	OpenAL32/alExtension.c \
	OpenAL32/alFilter.c \
	OpenAL32/alListener.c \
	OpenAL32/alSource.c \
	OpenAL32/alState.c \
	OpenAL32/alThunk.c

LOCAL_SRC_FILES +=\
	Alc/ALc.c \
	Alc/Alu.c \
	Alc/alcConfig.c \
	Alc/alcDedicated.c \
	Alc/alcEcho.c \
	Alc/alcModulator.c \
	Alc/alcReverb.c \
	Alc/alcRing.c \
	Alc/alcThread.c \
	Alc/bs2b.c \
	Alc/helpers.c \
	Alc/hrtf.c \
	Alc/mixer.c \
	Alc/panning.c

LOCAL_SRC_FILES +=\
	Alc/backends/loopback.c \
	Alc/backends/null.c \
	Alc/backends/opensl.c \
	Alc/backends/android.c \
	Alc/backends/wave.c
	
#LOCAL_LDLIBS += -lOpenSLES

include $(BUILD_STATIC_LIBRARY)
