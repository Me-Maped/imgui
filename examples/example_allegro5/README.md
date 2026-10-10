
# 配置

Dear ImGui 默认输出 16 位顶点索引。
Allegro 本身不支持它们，因此我们有两种解决方案：在 imgui_impl_allegro5.cpp 中手动转换索引，或者以 32 位索引编译 dear imgui。
你可以修改 Dear ImGui 自带的 imconfig.h（更简单），或者设置 C++ 预处理器选项 IMGUI_USER_CONFIG 指向某个文件名。
我们提供了 `imconfig_allegro5.h`，它启用 32 位索引。
请注意，此后端同时支持 16 位和 32 位索引，但 32 位索引会略快一些，因为它们不需要手动转换。

# 如何构建

### 在 Ubuntu 14.04+ 和 macOS 上

```bash
g++ -DIMGUI_USER_CONFIG=\"examples/example_allegro5/imconfig_allegro5.h\" -I .. -I ../.. -I ../../backends main.cpp ../../backends/imgui_impl_allegro5.cpp ../../imgui*.cpp -lallegro -lallegro_main -lallegro_primitives -o allegro5_example
```

在 macOS 上，使用 homebrew 安装 Allegro：`brew install allegro`。

### 在 Windows 上使用 Visual Studio 的命令行

你可以使用 vcpkg 安装 Allegro：
```
git clone https://github.com/Microsoft/vcpkg
cd vcpkg
bootstrap-vcpkg.bat
vcpkg install allegro5 --triplet=x86-windows    ; for win32
vcpkg install allegro5 --triplet=x64-windows    ; for win64
vcpkg integrate install                         ; register include / libs in Visual Studio
```

构建：
```
set ALLEGRODIR=path_to_your_allegro5_folder
cl /Zi /MD /utf-8 /I %ALLEGRODIR%\include /DIMGUI_USER_CONFIG=\"examples/example_allegro5/imconfig_allegro5.h\" /I .. /I ..\.. /I ..\..\backends main.cpp ..\..\backends\imgui_impl_allegro5.cpp ..\..\imgui*.cpp /link /LIBPATH:%ALLEGRODIR%\lib allegro-5.0.10-monolith-md.lib user32.lib
```
