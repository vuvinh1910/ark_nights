LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE	:= libdobby
LOCAL_SRC_FILES	:= Includes/Dobby/libraries/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE	:= $(TARGET_ARCH_ABI)
LOCAL_MODULE_FILENAME	:= $(TARGET_ARCH_ABI)

LOCAL_CFLAGS	:= -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS	+= -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS	:= -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS	+= -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS	+= -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE	:= arm
LOCAL_LDLIBS	:= -llog -landroid -lEGL -lGLESv3 -lGLESv2 -lGLESv1_CM -lz

LOCAL_C_INCLUDES	+= $(LOCAL_PATH)/Includes
LOCAL_C_INCLUDES	+= $(LOCAL_PATH)/ImGui
LOCAL_C_INCLUDES	+= $(LOCAL_PATH)/ImGui/backends
LOCAL_C_INCLUDES	+= $(LOCAL_PATH)/TuanMeta

LOCAL_SRC_FILES := Main.cpp \
	ImGui/imgui.cpp \
	ImGui/imgui_draw.cpp \
	ImGui/imgui_tables.cpp \
	ImGui/imgui_widgets.cpp \
	ImGui/backends/imgui_impl_opengl3.cpp \
	ImGui/backends/imgui_impl_android.cpp \
	Includes/KittyMemory/KittyMemory.cpp \
	Includes/KittyMemory/MemoryPatch.cpp \
	Includes/KittyMemory/MemoryBackup.cpp \
	Includes/KittyMemory/KittyUtils.cpp \
	Includes/KittyMemory/KittyScanner.cpp \
	Includes/KittyMemory/KittyArm64.cpp \
	Includes/xdl/xdl.c \
	Includes/xdl/xdl_iterate.c \
	Includes/xdl/xdl_linker.c \
	Includes/xdl/xdl_lzma.c \
	Includes/xdl/xdl_util.c \
	Includes/oxorany/oxorany.cpp \
	TuanMeta/Utils/Tools.cpp \
	TuanMeta/Utils/DebugLog.cpp \
	TuanMeta/IL2CppSDKGenerator/Il2Cpp.cpp \

LOCAL_STATIC_LIBRARIES := libdobby
LOCAL_CPP_FEATURES	:= exceptions

include $(BUILD_SHARED_LIBRARY)
