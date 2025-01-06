//
// Created by Reveny on 1/6/2025.
//
#include <Headers/Drawing.hpp>
#include <Headers/Logger.hpp>
#include <Headers/Utility.hpp>

#include "dobby.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"
#include "backends/android_native_app_glue.h"
#include "KittyMemory.h"
#include "Include/Roboto-Regular.h"

namespace Drawing
{
    std::function<void(int, int)> DrawFunction = nullptr;
    EGLBoolean (*SwapBuffers)(EGLDisplay dpy, EGLSurface surf) = nullptr;
    void (*OriginalInput)(void *thiz, void *ex_ab, void *ex_ac) = nullptr;
    bool IsInitialized = false;
    int GlWidth = 0;
    int GlHeight = 0;
}

void Drawing::InitMenu(std::function<void(int, int)> drawFunction) {
    DrawFunction = std::move(drawFunction);
    do {
        sleep(1);
    } while (!Utility::IsLibraryLoaded("libEGL.so"));

    void* eglSwapBuffers = DobbySymbolResolver("libEGL.so", "eglSwapBuffers");
    KittyMemory::ProtectAddr(eglSwapBuffers, sizeof(eglSwapBuffers), PROT_READ | PROT_WRITE | PROT_EXEC);
    DobbyHook(eglSwapBuffers, (void *)SwapBuffersHook, (void **)&SwapBuffers);

    void* inputMethod = DobbySymbolResolver("/system/lib/libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
    KittyMemory::ProtectAddr(inputMethod, sizeof(inputMethod), PROT_READ | PROT_WRITE | PROT_EXEC);
    DobbyHook(inputMethod, (void *)InputHook, (void **)&OriginalInput);

    LOGI("Drawing initialized");
}

void Drawing::SetupMenu() {
    if (IsInitialized) return;

    auto ctx = ImGui::CreateContext();
    if (!ctx) {
        LOGI("Failed to create context");
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)GlWidth, (float)GlHeight);
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.IniFilename = nullptr;

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init();
    ImGui_ImplOpenGL3_Init("#version 300 es");

    int systemScale = (1 / GlWidth) * GlWidth;
    ImFontConfig font_cfg;
    font_cfg.SizePixels = static_cast<float>(systemScale * 22);
    io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, systemScale * 30, 40.0f);

    ImGui::GetStyle().ScaleAllSizes(2);

    IsInitialized = true;
    LOGI("ImGUI Setup done.");
}

void Drawing::InternalDrawMenu(int width, int height) {
    if (!IsInitialized) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(width, height);
    ImGui::NewFrame();

    DrawFunction(width, height);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

EGLBoolean Drawing::SwapBuffersHook(EGLDisplay dpy, EGLSurface surf) {
    EGLint w, h;
    eglQuerySurface(dpy, surf, EGL_WIDTH, &w);
    eglQuerySurface(dpy, surf, EGL_HEIGHT, &h);
    GlWidth = w;
    GlHeight = h;

    SetupMenu();
    InternalDrawMenu(w, h);

    return SwapBuffers(dpy, surf);
}

void Drawing::InputHook(void *thiz, void *ex_ab, void *ex_ac) {
    OriginalInput(thiz, ex_ab, ex_ac);

    // Handle the input event with ImGui
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
}