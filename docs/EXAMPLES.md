_(你可以在 https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md 浏览此文件，或使用任意 Markdown 查看器查看)_

## Dear ImGui: 示例

**[examples/](https://github.com/ocornut/imgui/blob/master/examples) 文件夹包含适用于各种平台和图形 API 的示例应用程序（独立、可直接构建）。** 它们都使用 [backends/](https://github.com/ocornut/imgui/blob/master/backends) 文件夹中的标准后端（参见 [docs/BACKENDS.md](https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md)）。

示例的目的是展示与后端的集成，让你试用 Dear ImGui，并引导你将
Dear ImGui 集成到你自己的应用程序/游戏/引擎中。
**一旦 Dear ImGui 完成设置并运行起来，请运行并参考 imgui_demo.cpp 中的 `ImGui::ShowDemoWindow()`，了解最终用户 API 的用法。**

你可以在以下位置找到其中一些示例应用程序的 Windows 二进制文件：
  https://www.dearimgui.com/binaries


### 快速开始

在典型现有应用程序中集成，使用标准后端时应该只需不到 20 行代码。

```cpp
初始化时：
  调用 ImGui::CreateContext()
  为每个后端调用 ImGui_ImplXXXX_Init()。

在你的帧开始时：
  为每个后端调用 ImGui_ImplXXXX_NewFrame()。
  调用 ImGui::NewFrame()

在你的帧结束时：
  调用 ImGui::Render()
  为你的 Renderer 后端调用 ImGui_ImplXXXX_RenderDrawData()。

关闭时：
  为每个后端调用 ImGui_ImplXXXX_Shutdown()。
  调用 ImGui::DestroyContext()
```

主要资源：
- 阅读 **[快速开始](https://github.com/ocornut/imgui/wiki/Getting-Started) wiki 指南**，获取关于如何将 Dear ImGui 集成到现有应用程序的详细示例。

其他资源：
- 阅读 https://www.dearimgui.com/faq 上的 FAQ
- 阅读 imgui.cpp 中的 'PROGRAMMER GUIDE' 章节。
- 阅读每个文件顶部的注释和说明。

如果你使用这里提供的任何后端，可以将 backends/imgui_impl_xxxx(.cpp,.h)
文件添加到你的项目中并按原样使用。每个 imgui_impl_xxxx.cpp 文件都带有各自独立的
更新日志（Changelog），因此如果你以后想更新它们，会更容易跟进变更内容。


### 示例应用程序

[example_theme_studio/](../examples/example_theme_studio/README.md) <BR>
本仓库新增的 GLFW + OpenGL3 主题工作台，使用 CMake 构建。<BR>
提供 00 原生只读基准及 01、03、04、07、10 五款自定义主题，支持外观编辑、INI 保存/重载和固定条件的 CPU/几何量比较。<BR>
主题实现独立于核心，位于 [misc/themes/](../misc/themes/)。查看[主题预览](themes/README.md)和[构建、运行、接入说明](../examples/example_theme_studio/README.md)。<BR>
当前实际运行验证覆盖 Linux。

[example_allegro5/](https://github.com/ocornut/imgui/blob/master/examples/example_allegro5/) <BR>
Allegro 5 示例。<BR>
= main.cpp + imgui_impl_allegro5.cpp

[example_android_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_android_opengl3/) <BR>
Android + OpenGL3 (ES) 示例。<BR>
= main.cpp + imgui_impl_android.cpp + imgui_impl_opengl3.cpp

[example_apple_metal/](https://github.com/ocornut/imgui/tree/master/examples/example_apple_metal/) <BR>
OSX 与 iOS + Metal 示例。<BR>
= main.mm + imgui_impl_osx.mm + imgui_impl_metal.mm <BR>
它基于 Xcode 9 起随 Xcode 提供的"跨平台"游戏模板。

[example_apple_metal4/](https://github.com/ocornut/imgui/blob/master/examples/example_apple_metal4/) <BR>
OSX 与 iOS + Metal4 示例，仅限 Mac。<BR>
= main.mm + imgui_impl_osx.mm + imgui_impl_metal4.mm <BR>

[example_apple_opengl2/](https://github.com/ocornut/imgui/blob/master/examples/example_apple_opengl2/) <BR>
OSX + OpenGL2 示例。<BR>
= main.mm + imgui_impl_osx.mm + imgui_impl_opengl2.cpp <BR>

[example_apple_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_apple_opengl3/) <BR>
OSX + OpenGL3 示例。<BR>
= main.mm + imgui_impl_osx.mm + imgui_impl_opengl3.cpp <BR>

[example_glfw_wgpu/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_wgpu/) <BR>
GLFW + WebGPU 示例。支持 Emscripten（web）、Dawn（原生）、WGPU（原生）。<BR>
= main.cpp + imgui_impl_glfw.cpp + imgui_impl_wgpu.cpp
注意，'example_glfw_opengl3' 和 'example_sdl2_opengl3' 示例也支持 Emscripten！

[example_glfw_metal/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_metal/) <BR>
GLFW (Mac) + Metal 示例。<BR>
= main.mm + imgui_impl_glfw.cpp + imgui_impl_metal.mm

[example_glfw_opengl2/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl2/) <BR>
GLFW + OpenGL2 示例（遗留、固定管线）。<BR>
= main.cpp + imgui_impl_glfw.cpp + imgui_impl_opengl2.cpp <BR>
**如果你的代码/引擎使用现代 GL 或 WEBGL（着色器、VBO、VAO 等），则不要使用此示例** <BR>
这段代码主要是作为参考资料提供的，以便了解 Dear ImGui 的集成方式，因为它更短。
如果你的代码使用 GL3+ 上下文或任何半现代 GL 调用，使用此渲染器很可能会
让事情变得更复杂，需要你的代码将许多 GL 属性重置为初始
状态，并且可能让你的 GPU 驱动感到困惑。一星，不推荐。

[example_glfw_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/) <BR>
GLFW (Win32, Mac, Linux) + OpenGL3+/ES2/ES3 示例（现代、可编程管线）。<BR>
= main.cpp + imgui_impl_glfw.cpp + imgui_impl_opengl3.cpp <BR>
它使用更现代的 GL 调用和自定义着色器。<BR>
它支持使用 Emscripten 构建并面向 WebGL。<BR>
如果你在应用程序中使用现代 GL 或 WebGL，建议优先使用这个示例。

[example_glfw_vulkan/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_vulkan/) <BR>
GLFW (Win32, Mac, Linux) + Vulkan 示例。<BR>
= main.cpp + imgui_impl_glfw.cpp + imgui_impl_vulkan.cpp <BR>
这个示例相当长且繁琐，因为：Vulkan。
对于此示例，main.cpp 文件破例使用了 imgui_impl_vulkan.h/cpp 中的辅助函数。

[example_glut_opengl2/](https://github.com/ocornut/imgui/blob/master/examples/example_glut_opengl2/) <BR>
GLUT（例如 Linux/Windows 上的 FreeGLUT、OSX 上的 GLUT 框架）+ OpenGL2 示例。<BR>
= main.cpp + imgui_impl_glut.cpp + imgui_impl_opengl2.cpp <BR>
注意，GLUT/FreeGLUT 基本上是过时的软件，建议使用 GLFW 或 SDL。

[example_null/](https://github.com/ocornut/imgui/blob/master/examples/example_null/) <BR>
Null 示例，编译并链接 imgui，创建上下文，以无输入、无图形输出的方式无头运行。<BR>
= main.cpp + imgui_impl_null.cpp<BR>
它用于在尽可能多的配置中快速测试核心 imgui 文件的编译。
因为这个应用程序既不创建窗口也不创建图形上下文，所以没有图形输出。
请注意，imgui_impl_null 本身是一个相当空的后端。我们提供它是为了保持一致性，
但不使用 null 后端创建一个骨架应用程序也同样简单。

[example_qnx_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_qnx_opengl3/) <BR>
QNX Screen + OpenGL ES 3 示例。<BR>
= main.cpp + imgui_impl_qnx.cpp + imgui_impl_opengl3.cpp <BR>
它使用 QNX 递归 make 构建系统，并使用 `IMGUI_IMPL_OPENGL_ES3` 编译 OpenGL3 渲染器。

[example_qnx_vulkan/](https://github.com/ocornut/imgui/blob/master/examples/example_qnx_vulkan/) <BR>
使用 `VK_QNX_screen_surface` 的 QNX Screen + Vulkan 示例。<BR>
= main.cpp + imgui_impl_qnx.cpp + imgui_impl_vulkan.cpp <BR>
它使用 QNX 递归 make 构建系统以及 Vulkan 后端的示例辅助结构。

[example_sdl2_directx11/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_directx11/) <BR>
SDL2 + DirectX11 示例，仅限 Windows。<BR>
= main.cpp + imgui_impl_sdl2.cpp + imgui_impl_dx11.cpp <BR>
它用于演示如何将 DirectX 与 SDL2 结合使用。

[example_sdl2_metal/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_metal/) <BR>
SDL2 + Metal 示例，仅限 Mac。<BR>
= main.mm + imgui_impl_sdl2.cpp + imgui_impl_metal.mm

[example_sdl2_opengl2/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_opengl2/) <BR>
SDL2 (Win32, Mac, Linux 等) + OpenGL 示例（遗留、固定管线）。<BR>
= main.cpp + imgui_impl_sdl2.cpp + imgui_impl_opengl2.cpp <BR>
**如果你的代码/引擎使用 GL 或 WEBGL（着色器、VBO、VAO 等），则不要使用 OPENGL2 代码** <BR>
这段代码主要是作为参考资料提供的，以便了解 Dear ImGui 的集成方式，因为它更短。
如果你的代码使用 GL3+ 上下文或任何半现代 GL 调用，使用此渲染器很可能会
让事情变得更复杂，需要你的代码将许多 GL 属性重置为初始
状态，并且可能让你的 GPU 驱动感到困惑。一星，不推荐。

[example_sdl2_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_opengl3/) <BR>
SDL2 (Win32, Mac, Linux 等) + OpenGL3+/ES2/ES3 示例。<BR>
= main.cpp + imgui_impl_sdl2.cpp + imgui_impl_opengl3.cpp <BR>
它使用更现代的 GL 调用和自定义着色器。<BR>
它支持使用 Emscripten 构建并面向 WebGL。<BR>
如果你在应用程序中使用现代 GL 或 WebGL，建议优先使用这个示例。

[example_sdl2_sdlrenderer2/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_sdlrenderer2/) <BR>
SDL2 (Win32, Mac, Linux 等) + 用于 SDL2 的 SDL_Renderer 示例。<BR>
= main.cpp + imgui_impl_sdl2.cpp + imgui_impl_sdlrenderer2.cpp <BR>
这需要 SDL 2.0.18+（2021 年 11 月发布）<BR>

[example_sdl2_vulkan/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_vulkan/) <BR>
SDL2 (Win32, Mac, Linux 等) + Vulkan 示例。<BR>
= main.cpp + imgui_impl_sdl2.cpp + imgui_impl_vulkan.cpp <BR>
这个示例相当长且繁琐，因为：Vulkan。<BR>
对于此示例，main.cpp 文件破例使用了 imgui_impl_vulkan.h/cpp 中的辅助函数。

[example_sdl2_wgpu/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_wgpu/) <BR>
SDL2 + WebGPU 示例。支持 Emscripten（web）、Dawn（原生）、WGPU（原生）。<BR>
= main.cpp + imgui_impl_sdl2.cpp + imgui_impl_wgpu.cpp
（注意，'example_sdl2_opengl3' 示例也支持 Emscripten！）

[example_sdl3_directx11/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_directx11/) <BR>
SDL3 + DirectX11 示例，仅限 Windows。<BR>
= main.cpp + imgui_impl_sdl3.cpp + imgui_impl_dx11.cpp <BR>

[example_sdl3_metal/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_metal/) <BR>
SDL3 + Metal 示例，仅限 Mac。<BR>
= main.cpp + imgui_impl_sdl3.cpp + imgui_impl_metal.mm <BR>

[example_sdl3_metal4/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_metal/) <BR>
SDL3 + Metal4 示例，仅限 Mac。<BR>
= main.cpp + imgui_impl_sdl3.cpp + imgui_impl_metal4.mm <BR>

[example_sdl3_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_opengl3/) <BR>
SDL3 (Win32, Mac, Linux 等) + OpenGL3+/ES2/ES3 示例。<BR>
= main.cpp + imgui_impl_sdl3.cpp + imgui_impl_opengl3.cpp <BR>
它使用更现代的 GL 调用和自定义着色器。<BR>
它支持使用 Emscripten 构建并面向 WebGL。<BR>

[example_sdl3_sdlgpu3/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_sdlgpu3/) <BR>
SDL3 (Win32, Mac, Linux 等) + 用于 SDL3 的 SDL_GPU 示例。<BR>
= main.cpp + imgui_impl_sdl3.cpp + imgui_impl_sdlrenderer3.cpp <BR>

[example_sdl3_sdlrenderer3/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_sdlrenderer3/) <BR>
SDL3 (Win32, Mac, Linux 等) + 用于 SDL3 的 SDL_Renderer 示例。<BR>
= main.cpp + imgui_impl_sdl3.cpp + imgui_impl_sdlrenderer3.cpp <BR>

[example_sdl3_vulkan/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_vulkan/) <BR>
SDL3 (Win32, Mac, Linux 等) + Vulkan 示例。<BR>
= main.cpp + imgui_impl_sdl3.cpp + imgui_impl_vulkan.cpp <BR>
这个示例相当长且繁琐，因为：Vulkan。<BR>
对于此示例，main.cpp 文件破例使用了 imgui_impl_vulkan.h/cpp 中的辅助函数。

[example_sdl3_wgpu/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_wgpu/) <BR>
SDL3 + WebGPU 示例。支持 Emscripten（web）、Dawn（原生）、WGPU（原生）。<BR>
= main.cpp + imgui_impl_sdl3.cpp + imgui_impl_wgpu.cpp
（注意，'example_sdl3_opengl3' 示例也支持 Emscripten！）

[example_win32_directx9/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx9/) <BR>
DirectX9 示例，仅限 Windows。<BR>
= main.cpp + imgui_impl_win32.cpp + imgui_impl_dx9.cpp

[example_win32_directx10/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx10/) <BR>
DirectX10 示例，仅限 Windows。<BR>
= main.cpp + imgui_impl_win32.cpp + imgui_impl_dx10.cpp

[example_win32_directx11/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx11/) <BR>
DirectX11 示例，仅限 Windows。<BR>
= main.cpp + imgui_impl_win32.cpp + imgui_impl_dx11.cpp

[example_win32_directx12/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx12/) <BR>
DirectX12 示例，仅限 Windows。<BR>
= main.cpp + imgui_impl_win32.cpp + imgui_impl_dx12.cpp <BR>
这个示例相当长且繁琐，因为：DirectX12。

[example_win32_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_opengl3/) <BR>
原生 Windows + OpenGL3 示例（现代、可编程管线）<BR>
= main.cpp + imgui_impl_win32.cpp + imgui_impl_opengl3.cpp <BR>

[example_win32_vulkan/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_vulkan/) <BR>
原生 Windows + Vulkan 示例 <BR>
= main.cpp + imgui_impl_win32.cpp + imgui_impl_vulkan.cpp <BR>


### 杂项

**构建**

遗憾的是，如今如果不依赖第三方软件和构建系统，使用外部
库（也就是我们这里用来创建窗口和渲染 3D 三角形的那种库）来创建和维护可移植的构建文件仍然很繁琐。对于这里的大多数示例，我们选择提供：
 - 用于 Linux/OSX 的 Makefile
 - 用于 Visual Studio 2008+ 的批处理文件
 - 用于 Visual Studio 2012+ 的 .sln 项目文件
 - 用于 Apple 示例的 Xcode 项目文件
如果它们在你的环境中无法使用，请告诉我们！
你大概可以直接将 imgui_impl_xxx.cpp/.h 文件导入到你自己的代码库中，或者用命令行编译器
直接编译它们。

如果你有兴趣使用 Cmake 构建和链接示例，请参见：
  https://github.com/ocornut/imgui/pull/1713 和 https://github.com/ocornut/imgui/pull/3027

**关于鼠标光标延迟**

Dear ImGui 对大多数行为不会引入明显的额外延迟，
例如，在 NewFrame() 之前最后传给 'io.AddMousePosEvent()' 的值将使窗口在
EndFrame()/Render() 时移动到正确位置。在 60 FPS 下，你的体验应该很不错。

然而，请考虑 OS 鼠标光标通常通过一条非常特定的硬件加速
路径渲染，这使它们比通过常规图形 API 渲染的大多数内容（包括
但不限于 Dear ImGui 窗口）感觉更平滑。由于 UI 渲染和交互与鼠标处于同一平面，
这种脱节可能会让特别敏感的用户感到不适。
你可以尝试启用 io.MouseDrawCursor 标志，请求 Dear ImGui 使用
常规图形 API 绘制鼠标光标，以帮助你直观地看到"硬件"光标与
常规渲染的软件光标之间的差异。
然而，以 60 FPS 渲染鼠标光标会感觉迟钝，因此你很可能不想一直启用它。
只有在进行交互式拖拽时，才切换到软件渲染的光标，这可能对用户体验有益。

请注意，某些设置配置或 GPU 驱动可能会根据其设置引入额外的显示延迟。
如果你注意到拖动窗口有延迟，并且不确定原因是什么：尝试直接在鼠标光标下绘制一个简单的
2D 形状来帮助定位问题！
