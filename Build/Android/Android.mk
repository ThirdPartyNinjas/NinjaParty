LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

D_PATH := ../../Dependencies

LOCAL_MODULE := NinjaParty
LOCAL_CPPFLAGS := -std=c++11 -pthread
LOCAL_CPP_FEATURES += exceptions rtti

# boost
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(D_PATH)/boost/include

# JsonCpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(D_PATH)/JsonCpp
LOCAL_SRC_FILES := $(D_PATH)/JsonCpp/jsoncpp.cpp

# libzip
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(D_PATH)/libzip
LOCAL_SRC_FILES +=\
	$(D_PATH)/libzip/zip_add.c \
	$(D_PATH)/libzip/zip_add_dir.c \
	$(D_PATH)/libzip/zip_close.c \
	$(D_PATH)/libzip/zip_delete.c \
	$(D_PATH)/libzip/zip_dirent.c \
	$(D_PATH)/libzip/zip_entry_free.c \
	$(D_PATH)/libzip/zip_entry_new.c \
	$(D_PATH)/libzip/zip_err_str.c \
	$(D_PATH)/libzip/zip_error.c \
	$(D_PATH)/libzip/zip_error_clear.c \
	$(D_PATH)/libzip/zip_error_get.c \
	$(D_PATH)/libzip/zip_error_get_sys_type.c \
	$(D_PATH)/libzip/zip_error_strerror.c \
	$(D_PATH)/libzip/zip_error_to_str.c \
	$(D_PATH)/libzip/zip_fclose.c \
	$(D_PATH)/libzip/zip_file_error_clear.c \
	$(D_PATH)/libzip/zip_file_error_get.c \
	$(D_PATH)/libzip/zip_file_get_offset.c \
	$(D_PATH)/libzip/zip_file_strerror.c \
	$(D_PATH)/libzip/zip_filerange_crc.c \
	$(D_PATH)/libzip/zip_fopen.c \
	$(D_PATH)/libzip/zip_fopen_index.c \
	$(D_PATH)/libzip/zip_fread.c \
	$(D_PATH)/libzip/zip_free.c \
	$(D_PATH)/libzip/zip_get_archive_comment.c \
	$(D_PATH)/libzip/zip_get_archive_flag.c \
	$(D_PATH)/libzip/zip_get_file_comment.c \
	$(D_PATH)/libzip/zip_get_num_files.c \
	$(D_PATH)/libzip/zip_get_name.c \
	$(D_PATH)/libzip/zip_memdup.c \
	$(D_PATH)/libzip/zip_name_locate.c \
	$(D_PATH)/libzip/zip_new.c \
	$(D_PATH)/libzip/zip_open.c \
	$(D_PATH)/libzip/zip_rename.c \
	$(D_PATH)/libzip/zip_replace.c \
	$(D_PATH)/libzip/zip_set_archive_comment.c \
	$(D_PATH)/libzip/zip_set_archive_flag.c \
	$(D_PATH)/libzip/zip_set_file_comment.c \
	$(D_PATH)/libzip/zip_source_buffer.c \
	$(D_PATH)/libzip/zip_source_file.c \
	$(D_PATH)/libzip/zip_source_filep.c \
	$(D_PATH)/libzip/zip_source_free.c \
	$(D_PATH)/libzip/zip_source_function.c \
	$(D_PATH)/libzip/zip_source_zip.c \
	$(D_PATH)/libzip/zip_set_name.c \
	$(D_PATH)/libzip/zip_stat.c \
	$(D_PATH)/libzip/zip_stat_index.c \
	$(D_PATH)/libzip/zip_stat_init.c \
	$(D_PATH)/libzip/zip_strerror.c \
	$(D_PATH)/libzip/zip_unchange.c \
	$(D_PATH)/libzip/zip_unchange_all.c \
	$(D_PATH)/libzip/zip_unchange_archive.c \
	$(D_PATH)/libzip/zip_unchange_data.c

# OpenALSoft
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(D_PATH)/OpenALSoft/include

# pugixml
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(D_PATH)/pugixml
LOCAL_SRC_FILES += $(D_PATH)/pugixml/pugixml.cpp

# spine-c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(D_PATH)/spine-c/include
LOCAL_SRC_FILES +=\
	$(D_PATH)/spine-c/src/spine/Animation.c \
	$(D_PATH)/spine-c/src/spine/AnimationState.c \
	$(D_PATH)/spine-c/src/spine/AnimationStateData.c \
	$(D_PATH)/spine-c/src/spine/Atlas.c \
	$(D_PATH)/spine-c/src/spine/AtlasAttachmentLoader.c \
	$(D_PATH)/spine-c/src/spine/Attachment.c \
	$(D_PATH)/spine-c/src/spine/AttachmentLoader.c \
	$(D_PATH)/spine-c/src/spine/Bone.c \
	$(D_PATH)/spine-c/src/spine/BoneData.c \
	$(D_PATH)/spine-c/src/spine/extension.c \
	$(D_PATH)/spine-c/src/spine/Json.c \
	$(D_PATH)/spine-c/src/spine/RegionAttachment.c \
	$(D_PATH)/spine-c/src/spine/Skeleton.c \
	$(D_PATH)/spine-c/src/spine/SkeletonData.c \
	$(D_PATH)/spine-c/src/spine/SkeletonJson.c \
	$(D_PATH)/spine-c/src/spine/Skin.c \
	$(D_PATH)/spine-c/src/spine/Slot.c \
	$(D_PATH)/spine-c/src/spine/SlotData.c

# stb_image
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(D_PATH)/stb_image
LOCAL_SRC_FILES += $(D_PATH)/stb_image/stb_image.c

# stb_vorbis
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(D_PATH)/stb_vorbis
LOCAL_SRC_FILES += $(D_PATH)/stb_vorbis/stb_vorbis.c

# Wm5
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(D_PATH)/Wm5
LOCAL_SRC_FILES +=\
	$(D_PATH)/Wm5/Wm5Assert.cpp \
	$(D_PATH)/Wm5/Wm5Math.cpp \
	$(D_PATH)/Wm5/Wm5Matrix3.cpp \
	$(D_PATH)/Wm5/Wm5Vector2.cpp \
	$(D_PATH)/Wm5/Wm5Vector3.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Include

S_PATH := ../../Source

# Assets
LOCAL_SRC_FILES +=\
	$(S_PATH)/Assets/AssetManager.cpp \
	$(S_PATH)/Assets/GetRootPath_Empty.cpp \
	$(S_PATH)/Assets/Path.cpp

# Audio
LOCAL_SRC_FILES +=\
	$(S_PATH)/Audio/AudioRenderer.cpp \
	$(S_PATH)/Audio/Song.cpp \
	$(S_PATH)/Audio/SoundEffect.cpp \
	$(S_PATH)/Audio/SoundInstance.cpp \
	$(S_PATH)/Audio/Vorbis.cpp \
	$(S_PATH)/Audio/Wav.cpp
	
# Graphics
LOCAL_SRC_FILES +=\
	$(S_PATH)/Graphics/Camera.cpp \
	$(S_PATH)/Graphics/Color.cpp \
	$(S_PATH)/Graphics/DeminaAnimationPlayer.cpp \
	$(S_PATH)/Graphics/Font.cpp \
	$(S_PATH)/Graphics/LoadTexture_stb.cpp \
	$(S_PATH)/Graphics/RenderTexture.cpp \
	$(S_PATH)/Graphics/SpineAnimationPlayer.cpp \
	$(S_PATH)/Graphics/SpriteAnimationPlayer.cpp \
	$(S_PATH)/Graphics/SpriteBatch.cpp \
	$(S_PATH)/Graphics/SpriteShader.cpp \
	$(S_PATH)/Graphics/Texture.cpp \
	$(S_PATH)/Graphics/TextureDictionary.cpp

# Game
LOCAL_SRC_FILES +=\
	$(S_PATH)/Game/Game.cpp \
	$(S_PATH)/Game/GameScreen.cpp \
	$(S_PATH)/Game/ScreenManager.cpp \
	$(S_PATH)/Game/TaskManager.cpp

# Math
LOCAL_SRC_FILES +=\
	$(S_PATH)/Math/Matrix3.cpp \
	$(S_PATH)/Math/Random.cpp \
	$(S_PATH)/Math/Tween.cpp \
	$(S_PATH)/Math/Vector2.cpp

include $(BUILD_STATIC_LIBRARY)
