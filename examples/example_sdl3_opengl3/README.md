
# 如何构建

## 在 Windows 上使用 Visual Studio IDE

使用提供的项目文件（.vcxproj）。如有必要，将其添加到解决方案（imgui_examples.sln）中。

## 在 Windows 上使用 Visual Studio 命令行

使用 build_win32.bat，或直接执行：
```
set SDL3_DIR=path_to_your_sdl3_folder
cl /Zi /MD /utf-8 /I.. /I..\.. /I%SDL3_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl3.cpp ..\..\backends\imgui_impl_opengl3.cpp ..\..\imgui*.cpp /FeDebug/example_sdl3_opengl3.exe /FoDebug/ /link /libpath:%SDL3_DIR%\lib\x86 SDL3.lib opengl32.lib /subsystem:console
#                 ^^ 包含路径     ^^ 源文件                                                                                    ^^ 输出 exe                    ^^ 输出目录   ^^ 库
# 或用于 64 位：
cl /Zi /MD /utf-8 /I.. /I..\.. /I%SDL3_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl3.cpp ..\..\backends\imgui_impl_opengl3.cpp ..\..\imgui*.cpp /FeDebug/example_sdl3_opengl3.exe /FoDebug/ /link /libpath:%SDL3_DIR%\lib\x64 SDL3.lib SDL2mainopengl32.lib /subsystem:console
```

## Linux 及类似 Unix 系统

使用我们的 Makefile，或直接执行：
```
c++ `sdl3-config --cflags` -I .. -I ../.. -I ../../backends
  main.cpp ../../backends/imgui_impl_sdl3.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp
  `sdl3-config --libs` -lGL -ldl
```

## macOS

使用我们的 Makefile，或直接执行：
```
brew install sdl3
c++ `sdl3-config --cflags` -I .. -I ../.. -I ../../backends
  main.cpp ../../backends/imgui_impl_sdl3.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp
  `sdl3-config --libs` -framework OpenGl -framework CoreFoundation
```

## Emscripten

截至 2023-05-30，Emscripten 尚不支持 SDL3。
