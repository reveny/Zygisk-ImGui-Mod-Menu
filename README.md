# Zygisk-ImGui-Mod-Menu
Magisk Module that Injects a ImGui Mod Menu into any game

# Features
- [x] Any Android Version
- [x] Any Game Supported
- [x] Arm64 Support
- [ ] x86 Support

# Build and Installation
Android Studio is required: https://developer.android.com/studio  <br />

Setup and Installation: <br />
Open /module/src/main/cpp/modmenu.h and change game_package_name to the package name of the target game. <br />

Install the magisk module through magisk manager <br />
or use the android shell
   ```shell
  adb push imgui-modmenu-v1.0.0-debug.zip /data/local/tmp
  adb shell
  
  su
  magisk --install-module /data/local/tmp/imgui-modmenu-v1.0.0-debug.zip
  reboot
  ```

# Credits
ImGui by ocornut: https://github.com/ocornut/imgui <br />
ESP Drawing by rev: https://github.com/Rev/Among-Us-Android-ESP/blob/main/Drawing.h <br />
Unity-ImGUI-Android by octowolve: https://github.com/Octowolve/Unity-ImGUI-Android <br />
  
# Issues
In case you have issues with bootloops go into twrp and uninstall the module or use adb to uninstall the module
   ```shell
  adb shell
  su
  magisk --remove-modules
  Start up your device as normal
  ```
More information can be found here:
https://forum.xda-developers.com/t/help-need-to-uninstall-magisk-modules-stuck-in-a-bootloop-when-magisk-is-activated.4187645/

# Contact
Telegram: https://t.me/Reveny1


