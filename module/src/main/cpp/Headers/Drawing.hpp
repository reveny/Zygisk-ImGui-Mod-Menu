//
// Created by Reveny on 1/6/2025.
//
#pragma once

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <functional>
#include <utility>
#include <unistd.h>

namespace Drawing
{
    extern std::function<void(int, int)> DrawFunction;
    extern EGLBoolean (*SwapBuffers)(EGLDisplay dpy, EGLSurface surf);
    extern void (*OriginalInput)(void *thiz, void *ex_ab, void *ex_ac);
    extern bool IsInitialized;
    extern int GlWidth;
    extern int GlHeight;

    void InitMenu(std::function<void(int, int)> drawFunction);
    void SetupMenu();
    void InternalDrawMenu(int width, int height);

    // Hooks
    void InputHook(void *thiz, void *ex_ab, void *ex_ac);
    EGLBoolean SwapBuffersHook(EGLDisplay dpy, EGLSurface surf);
}
