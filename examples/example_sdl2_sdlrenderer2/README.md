
# 如何构建

- 在 Windows 上使用 Visual Studio 命令行

```
set SDL2_DIR=path_to_your_sdl2_folder
cl /Zi /MD /utf-8 /I.. /I..\.. /I%SDL2_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl2.cpp ..\..\backends\imgui_impl_sdlrenderer2.cpp ..\..\imgui*.cpp /FeDebug/example_sdl2_sdlrenderer.exe /FoDebug/ /link /libpath:%SDL2_DIR%\lib\x86 SDL2.lib SDL2main.lib /subsystem:console
#                 ^^ 包含路径                  ^^ 源文件                                                                                         ^^ 输出 exe                         ^^ 输出目录  ^^ 库
# 或用于 64 位：
cl /Zi /MD /utf-8 /I.. /I..\.. /I%SDL2_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl2.cpp ..\..\backends\imgui_impl_sdlrenderer2.cpp ..\..\imgui*.cpp /FeDebug/example_sdl2_sdlrenderer.exe /FoDebug/ /link /libpath:%SDL2_DIR%\lib\x64 SDL2.lib SDL2main.lib /subsystem:console
```

- 在 Linux 及类似 Unix 系统上

```
c++ `sdl2-config --cflags` -I .. -I ../.. main.cpp ../../backends/imgui_impl_sdl2.cpp ../../backends/imgui_impl_sdlrenderer2.cpp ../../imgui*.cpp `sdl2-config --libs` -lGL
```

- 在 Mac OS X 上

```
brew install sdl2
c++ `sdl2-config --cflags` -I .. -I ../.. main.cpp ../../backends/imgui_impl_sdl2.cpp ../../backends/imgui_impl_sdlrenderer2.cpp ../../imgui*.cpp `sdl2-config --libs` -framework OpenGl
```
