#pragma once

#include <android/log.h>

#define TAG "ModMenu"
#define LOGE(...) ((void)__android_log_print(6, TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(4,  TAG, __VA_ARGS__))