//
// Created by Reveny on 2023/1/13.
//
#include <cstring>
#include <pthread.h>
#include <dlfcn.h>
#include <string>
#include <android/log.h>
#include <thread>

#include <Headers/Logger.hpp>
#include <Include/zygisk.hpp>

namespace ModMenu
{
    void DrawMenu(int width, int height);
    void HackThread();
}