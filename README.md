# Zygisk-ImGui-Mod-Menu
Magisk Module that Injects a ImGui Mod Menu into any game.

# Features
- [x] Any Android Version
- [x] Any Game Supported
- [x] Arm64 Support

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

## Warning
This project is still in development and may not work as expected. <br />

# Credits
ImGui by ocornut: https://github.com/ocornut/imgui <br />
ESP Drawing by rev: https://github.com/Rev/Among-Us-Android-ESP/blob/main/Drawing.h <br />
Unity-ImGUI-Android by octowolve: https://github.com/Octowolve/Unity-ImGUI-Android <br />

## Contact
For any questions, collaboration requests, or updates, feel free to reach out via:

Telegram Channel: [Join Channel](https://t.me/reveny1)  
Telegram Contact: [Contact Me](https://t.me/revenyy)  
Website: [My Website](https://reveny.me)  
Email: [contact@reveny.me](mailto:contact@reveny.me)

## Support Me
If you'd like to, you can support me on my [Website](https://reveny.me/donate.html)
