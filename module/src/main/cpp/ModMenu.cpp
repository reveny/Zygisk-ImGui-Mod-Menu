//
// Created by Reveny on 1/6/2025.
//
#include <Headers/ModMenu.hpp>
#include <Headers/Drawing.hpp>
#include <Headers/Utility.hpp>

#include "imgui.h"

#define TARGET_LIBRARY "libil2cpp.so"

void ModMenu::DrawMenu(int width, int height) {
    ImGui::ShowDemoWindow();
}

void ModMenu::HackThread() {
    Drawing::InitMenu(DrawMenu);

    do {
        LOGI("Waiting for target library to load to load");
        sleep(1);
    } while (Utility::IsLibraryLoaded(TARGET_LIBRARY));
    LOGI("Target library loaded at 0x%lu", Utility::GetBaseAddress(TARGET_LIBRARY));

    //Hooks and Patches here

    LOGI("ModMenu initialized");
}