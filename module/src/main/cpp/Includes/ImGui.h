//
// Created by Reveny on 2022/12/25.
//

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <asm-generic/mman.h>
#include <sys/mman.h>

#include "ImGui/imgui.h"
#include "Includes/Roboto-Regular.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/android_native_app_glue.h"

#include "Utils.h"
#include "Dobby/dobby.h"

#include "Includes/obfuscate.h"
#include "Includes/Logger.h"

void menuStyle();
void (*menuAddress)();

using swapbuffers_orig = EGLBoolean (*)(EGLDisplay dpy, EGLSurface surf);
EGLBoolean swapbuffers_hook(EGLDisplay dpy, EGLSurface surf);
using inject_event_orig = int (*)(JNIEnv *env, jobject thiz, jobject inputEvent);

swapbuffers_orig o_swapbuffers = nullptr;
inject_event_orig o_inject_event = nullptr;
bool isInitialized = false;
int glWidth = 0;
int glHeight = 0;

int inject_event_hook(JNIEnv *env, jobject __this, jobject input_event) {
    jclass motion_event = env->FindClass((OBFUSCATE("android/view/MotionEvent")));

    if (!motion_event) {
        LOGI(OBFUSCATE("Can't find MotionEvent!"));
        return o_inject_event(env, __this, input_event);
    }

    if (env->IsInstanceOf(input_event, motion_event)) {
        jmethodID get_action = env->GetMethodID(motion_event, ("getActionMasked"), ("()I"));
        jmethodID get_x = env->GetMethodID(motion_event, ("getX"), ("()F"));
        jmethodID get_y = env->GetMethodID(motion_event, ("getY"), ("()F"));
        jmethodID get_pointer_count = env->GetMethodID(motion_event, ("getPointerCount"), ("()I"));

        ImGui_ImplAndroid_HandleInputEvent(env->CallIntMethod(input_event, get_action), env->CallFloatMethod(input_event, get_x), env->CallFloatMethod(input_event, get_y), env->CallIntMethod(input_event, get_pointer_count));

        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse) {
            return true;
        }
    }

    jclass key_event = env->FindClass(OBFUSCATE("android/view/KeyEvent"));

    if (!key_event) {
        LOGI("Can't find KeyEvent!");
        return o_inject_event(env, __this, input_event);
    }

    if (env->IsInstanceOf(input_event, key_event)) {
        jmethodID get_unicode_char = env->GetMethodID(key_event, ("getUnicodeChar"), ("()I"));
        jmethodID get_meta_state = env->GetMethodID(key_event, ("getMetaState"), ("()I"));

        ImGuiIO &io = ImGui::GetIO();
        io.AddInputCharacter(env->CallIntMethod(input_event, get_unicode_char, env->CallIntMethod(input_event, get_meta_state)));
    }
    return o_inject_event(env, __this, input_event);
}

//This menu_addr is used to allow for multiple game support in the future
void *initModMenu(void *menu_addr) {
    menuAddress = (void (*)())menu_addr;
    do {
        sleep(1);
    } while (!isLibraryLoaded(OBFUSCATE("libEGL.so")));

    auto swapBuffers = ((uintptr_t) DobbySymbolResolver(OBFUSCATE("libEGL.so"), OBFUSCATE("eglSwapBuffers")));
    mprotect((void *) swapBuffers, 8, PROT_READ | PROT_WRITE | PROT_EXEC);
    DobbyHook((void *) swapBuffers, (void *) swapbuffers_hook, (void **) &o_swapbuffers);

    //Taken from https://github.com/Octowolve/Unity-ImGUI-Android/blob/main/src/hooks.cpp
    uintptr_t inject_event = find_pattern_in_module(OBFUSCATE("libunity.so"), OBFUSCATE("FF 83 01 D1 F7 5B 03 A9 F5 53 04 A9 F3 7B 05 A9 F5 03 02 AA F9 35 FF 97"));
    if(inject_event != 0) {
        DobbyHook((void *)inject_event, (void *)inject_event_hook, (void **)&o_inject_event);
    }

    LOGI(OBFUSCATE("ImGUI Hooks initialized"));
    return nullptr;
}

void setupMenu() {
    if (isInitialized) return;

    auto ctx = ImGui::CreateContext();
    if (!ctx) {
        LOGI(OBFUSCATE("Failed to create context"));
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.IniFilename = nullptr;

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init();
    ImGui_ImplOpenGL3_Init("#version 300 es");

    int systemScale = (1.0 / glWidth) * glWidth;
    ImFontConfig font_cfg;
    font_cfg.SizePixels = systemScale * 22.0f;
    io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, systemScale * 30.0, 40.0f);

    ImGui::GetStyle().ScaleAllSizes(7);

    isInitialized = true;
    LOGI("setup done.");
}
void internalDrawMenu(int width, int height) {
    if(!isInitialized) return;

    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(width, height);
    ImGui::NewFrame();

    menuAddress();

    ImGui::Render();

    //Disable for UE
    ImGui::Render();
    ImGui::EndFrame();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //Disable for UE
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.0, 0.0, 0, 0.0);
}

EGLBoolean swapbuffers_hook(EGLDisplay dpy, EGLSurface surf) {
    EGLint w, h;
    eglQuerySurface(dpy, surf, EGL_WIDTH, &w);
    eglQuerySurface(dpy, surf, EGL_HEIGHT, &h);
    glWidth = w;
    glHeight = h;

    setupMenu();
    internalDrawMenu(w, h);

    return o_swapbuffers(dpy, surf);
}
