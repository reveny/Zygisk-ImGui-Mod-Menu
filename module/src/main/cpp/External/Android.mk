LOCAL_PATH := $(call my-dir)

# libimgui.a
include $(CLEAR_VARS)
LOCAL_MODULE := libimgui
LOCAL_C_INCLUDES := $(LOCAL_PATH)/ImGui
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_SRC_FILES := ImGui/backends/imgui_impl_opengl3.cpp \
                   ImGui/backends/imgui_impl_android.cpp \
                   ImGui/imgui.cpp \
                   ImGui/imgui_demo.cpp \
                   ImGui/imgui_draw.cpp \
                   ImGui/imgui_tables.cpp \
                   ImGui/imgui_widgets.cpp \

include $(BUILD_STATIC_LIBRARY)

# libkitty.a
include $(CLEAR_VARS)
LOCAL_MODULE := libkitty
LOCAL_C_INCLUDES := $(LOCAL_PATH)/KittyMemory
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_SRC_FILES := KittyMemory/KittyMemory.cpp \
                   KittyMemory/MemoryPatch.cpp \
                   KittyMemory/MemoryBackup.cpp \
                   KittyMemory/KittyUtils.cpp \

include $(BUILD_STATIC_LIBRARY)

# libdobby.a
include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_C_INCLUDES := $(LOCAL_PATH)/Dobby
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_SRC_FILES := Dobby/libs/$(TARGET_ARCH_ABI)/libdobby.a

include $(PREBUILT_STATIC_LIBRARY)

CWD := $(LOCAL_PATH)