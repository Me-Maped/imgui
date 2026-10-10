_(你可以在 https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md 浏览此文件，或使用任意 Markdown 查看器查看)_

# <a id="dear-imgui-backends"></a>Dear ImGui: 后端

## <a id="index"></a>目录

- [简介](#introduction)
   - [快速开始](#getting-started)
   - [什么是后端？](#what-are-backends)
- [使用标准后端](#using-standard-backends)
- [使用第三方后端](#using-third-party-backends)
- [编写你自己的后端](#writing-your-own-backend)
  - [使用自定义引擎？](#using-a-custom-engine)
  - [平台：实现你的平台后端](#platform-implementing-your-platform-backend)
  - [渲染：实现你的 RenderDrawData 函数](#rendering-implementing-your-renderdrawdata-function)
  - [渲染：添加对 `ImGuiBackendFlags_RendererHasTextures` 的支持（1.92+）](#rendering-adding-support-for-imguibackendflags_rendererhastextures-192)

## <a id="introduction"></a>简介

### <a id="getting-started"></a>快速开始

💡 **[快速开始](https://github.com/ocornut/imgui/wiki/Getting-Started) wiki 指南**提供了如何将 Dear ImGui 集成到现有应用程序的示例。
<BR> [EXAMPLES.MD](https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md) 文档也值得一读。

### <a id="what-are-backends"></a>什么是后端？

Dear ImGui 具有高度可移植性，运行和渲染只需要少数几个条件，通常是：

 - 必需：提供鼠标/键盘输入（送入 `ImGuiIO` 结构体）。
 - 必需：创建、更新和销毁纹理。
 - 必需：使用裁剪矩形渲染带索引的纹理三角形。

 额外功能是可选启用的，我们的后端会尽可能多地支持它们：

 - 可选：自定义纹理绑定支持。
 - 可选：剪贴板支持。
 - 可选：游戏手柄支持。
 - 可选：鼠标光标形状支持。
 - 可选：IME 支持。
 - 可选：多视口支持。
 等等。

这基本上就是每个后端所做的事情，外加必要的可移植性杂项处理。使用标准后端可确保你能获得所有这些功能，包括那些在你这边实现起来更困难的功能（例如多视口支持）。

重要的是理解 Dear ImGui 核心库（根文件夹中的文件）与这里描述的后端（backends/ 文件夹）之间的区别。

- 有些问题可能只与特定后端或平台有关。
- 你应该能够为几乎任何平台和任何 3D 图形 API 编写后端。
  例如，你可以发挥创意，使用软件渲染，或在另一台机器上远程渲染。

## <a id="using-standard-backends"></a>使用标准后端

**[backends/](https://github.com/ocornut/imgui/blob/master/backends) 文件夹包含面向流行平台/图形 API 的后端，你可以在自己的应用程序或引擎中使用它们来轻松集成 Dear ImGui。** 每个后端通常自包含于一对文件中：imgui_impl_XXXX.cpp + imgui_impl_XXXX.h。

- 'Platform'（平台）后端负责：鼠标/键盘/游戏手柄输入、光标形状、计时和窗口管理。<BR>
  例如 Windows（[imgui_impl_win32.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_win32.cpp)）、SDL3（[imgui_impl_sdl3.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_sdl3.cpp)）、GLFW（[imgui_impl_glfw.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_glfw.cpp)）等。

- 'Renderer'（渲染器）后端负责：创建图集纹理，以及渲染 imgui 绘制数据。<BR>
  例如 DirectX11（[imgui_impl_dx11.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_dx11.cpp)）、OpenGL/WebGL（[imgui_impl_opengl3.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_opengl3.cpp)）、Vulkan（[imgui_impl_vulkan.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_vulkan.cpp)）等。

- 对于某些高层框架，单个后端通常同时处理 'Platform' 和 'Renderer' 两部分。<BR>
  例如 Allegro 5（[imgui_impl_allegro5.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_allegro5.cpp)）。如果你最终为自己的引擎创建了自定义后端，你可能也想这样做。

应用程序通常组合一个 Platform 后端 + 一个 Renderer 后端 + Dear ImGui 主源码。
例如，[example_win32_directx11](https://github.com/ocornut/imgui/tree/master/examples/example_win32_directx11) 应用程序组合了 imgui_impl_win32.cpp + imgui_impl_dx11.cpp。[examples/](https://github.com/ocornut/imgui/blob/master/examples/) 文件夹中有 20 多个示例。详见 [EXAMPLES.MD](https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md)。

**一旦 Dear ImGui 完成设置并运行起来，请运行并参考 imgui_demo.cpp 中的 `ImGui::ShowDemoWindow()`，了解最终用户 API 的用法。**

### <a id="list-of-standard-backends"></a>标准后端列表

在 [backends/](https://github.com/ocornut/imgui/blob/master/backends) 文件夹中：

平台后端列表：

    imgui_impl_android.cpp      ; Android 原生应用 API
    imgui_impl_glfw.cpp         ; GLFW (Windows, macOS, Linux, 等) http://www.glfw.org/
    imgui_impl_osx.mm           ; macOS 原生 API (功能不如 glfw/sdl 后端完整)
    imgui_impl_sdl2.cpp         ; SDL2 (Windows, macOS, Linux, iOS, Android) https://www.libsdl.org
    imgui_impl_sdl3.cpp         ; SDL3 (Windows, macOS, Linux, iOS, Android) https://www.libsdl.org
    imgui_impl_qnx.cpp          ; QNX Screen
    imgui_impl_win32.cpp        ; Win32 原生 API (Windows)
    imgui_impl_glut.cpp         ; GLUT/FreeGLUT (这是史前软件，今天绝对不推荐使用！)

渲染器后端列表：

    imgui_impl_dx9.cpp          ; DirectX9
    imgui_impl_dx10.cpp         ; DirectX10
    imgui_impl_dx11.cpp         ; DirectX11
    imgui_impl_dx12.cpp         ; DirectX12
    imgui_impl_metal.mm         ; Metal (ObjC 或 C++)
    imgui_impl_metal4.mm        ; Metal 4 (ObjC 或 C++)
    imgui_impl_opengl2.cpp      ; OpenGL 2 (遗留固定管线。不要与现代 OpenGL 代码一起使用！)
    imgui_impl_opengl3.cpp      ; OpenGL 3/4, OpenGL ES 2/3, WebGL
    imgui_impl_sdlgpu3.cpp      ; SDL_GPU (SDL3 的可移植 3D 图形 API)
    imgui_impl_sdlrenderer2.cpp ; SDL_Renderer (SDL2 的可选组件，从 SDL 2.0.18+ 起可用)
    imgui_impl_sdlrenderer3.cpp ; SDL_Renderer (SDL3 的可选组件。优先使用 SDL_GPU！)。
    imgui_impl_vulkan.cpp       ; Vulkan
    imgui_impl_wgpu.cpp         ; WebGPU (web + 桌面)

高层框架后端列表（组合 Platform + Renderer）：

    imgui_impl_allegro5.cpp
    imgui_impl_null.cpp

Emscripten 也受支持！
SDL2+GL、SDL3+GL、GLFW+GL 和 GLFW+WebGPU 示例都已准备好使用 Emscripten 构建和运行。

### <a id="recommended-backends"></a>推荐的后端

如果你不确定该使用哪个后端，以下是面向可移植应用程序推荐的平台/框架：

|库 |网站 |后端 |备注 |
|--------|--------|--------|-----|
| SDL3 | https://www.libsdl.org | imgui_impl_sdl3.cpp | 推荐 |
| SDL2 | https://www.libsdl.org | imgui_impl_sdl2.cpp | |
| GLFW | https://github.com/glfw/glfw | imgui_impl_glfw.cpp | |
| Sokol | https://github.com/floooh/sokol | [util/sokol_imgui.h](https://github.com/floooh/sokol/blob/master/util/sokol_imgui.h) | 比 GLFW/SDL 更底层 |

如果你的应用程序运行在 Windows 上，或者你使用了多视口，imgui_impl_win32 后端在某些细节上处理得比其他后端更好。

## <a id="using-third-party-backends"></a>使用第三方后端

完整列表请参见 https://github.com/ocornut/imgui/wiki/Bindings （例如 Adventure Game Studio、Cinder、Cocos2d-x、Game Maker Studio2、Godot、LÖVE+LUA、Magnum、Monogame、Ogre、openFrameworks、OpenSceneGraph、SFML、Sokol、Unity、Unreal Engine 以及许多其他）。

## <a id="writing-your-own-backend"></a>编写你自己的后端

### <a id="using-a-custom-engine"></a>使用自定义引擎？

你很可能会忍不住想从使用自己的自定义/高层设施重写自己的后端开始……<BR>
三思！

简而言之：
- 编写你自己的渲染器后端很容易。
- 编写你自己的平台后端更难，你更有可能引入 bug。
- **创建自己的后端不太可能为你的项目带来价值。**

**考虑直接按原样使用现有的后端**。
你将在集成库时节省大量时间。
标准后端经过实战检验，能处理许多你很可能实现错误的细微之处。
如果确实需要，你可以稍后再决定自己重写一个自定义后端。
在大多数情况下，自定义后端比我们提供的标准后端功能更少、bug 更多。
如果你想要可移植性，可以使用多个后端，并在编译时
或运行时在它们之间选择。

**示例 A**：你的引擎构建在 Windows + DirectX11 之上，但你有自己叠加在 DirectX11 之上的高层渲染
系统。<BR>
建议：先尝试使用 imgui_impl_win32.cpp + imgui_impl_dx11.cpp。
一旦它工作正常，如果确实需要，你可以用使用自己渲染函数的自定义渲染器
替换 imgui_impl_dx11.cpp 的代码，并继续使用标准 Win32 代码等。

**示例 B**：你的引擎运行在 Windows、Mac、Linux 上，并分别使用 DirectX11、Metal 和 Vulkan。<BR>
建议：使用多个通用后端！
一旦它工作正常，如果确实需要，你可以用你自己的抽象替换后端的一部分。

**示例 C**：你的引擎运行在我们无法提供公开后端的平台上（例如 PS4/PS5、Switch），
并且你到处都有高层系统。<BR>
建议：先尝试使用一个非可移植后端（例如 win32 + 底层图形 API），让你的
桌面构建先跑起来。这会让你更快运行起来，并让你熟悉
Dear ImGui 的工作方式和设置方式。之后你可以使用自己的引擎 API 重写一个自定义后端……

另外：
'docking' 分支的[多视口功能](https://github.com/ocornut/imgui/wiki/Multi-Viewports)允许
Dear ImGui 窗口无缝地从主应用程序窗口中分离出来。这是通过为 Platform 和 Renderer 后端增加
一个额外层来实现的，它允许 Dear ImGui 传递平台特定的
请求，例如："创建一个额外的 OS 窗口"、"创建一个渲染上下文"、"获取此窗口的 OS 位置"，
但有些事更困难，例如"找到鼠标位置下的 OS 窗口，但其中一些窗口被标记为穿透"。详见 'ImGuiPlatformIO'。
使用 100% 你自己的抽象来正确支持多视口功能，
比支持单视口更困难。
如果你决定使用未经修改的 imgui_impl_XXXX.cpp 文件，你就可以自动受益于
与视口和平台窗口相关的改进和修复，而无需在你这边做额外工作。

### <a id="platform-implementing-your-platform-backend"></a>平台：实现你的平台后端

imgui_impl_XXX.cpp 文件中的平台后端包含许多实现。

**在你的 `ImGui_ImplXXX_Init()` 函数中：**
- 你可以分配后端数据，并使用 `io.BackendPlatformUserData` 稍后存储/取回它。
- 将 `io.BackendPlatformName` 设置为名称 `"imgui_impl_xxxx"`，它将在例如关于框中可用。
- 将 `io.BackendPlatformUserData` 设置为你的后端数据。
- 使用支持的可选功能设置 `io.BackendFlags`：
  - `ImGuiBackendFlags_HasGamepad`：支持游戏手柄，且当前已连接一个。
  - `ImGuiBackendFlags_HasMouseCursors`：支持遵循 GetMouseCursor() 的值来改变 OS 光标形状。
  - `ImGuiBackendFlags_HasSetMousePos`：支持 io.WantSetMousePos 请求以重新定位 OS 鼠标位置（仅在设置了 io.ConfigNavMoveSetMousePos 时使用）。
  - `ImGuiBackendFlags_PlatformHasViewports` 支持多个视口。（仅多视口）
  - `ImGuiBackendFlags_HasMouseHoveredViewport` 支持使用鼠标下方的视口调用 io.AddMouseViewportEvent()。如果可能，请忽略带有 ImGuiViewportFlags_NoInputs 标志的视口。如果无法做到这一点，Dear ImGui 需要使用一个有缺陷的启发式方法来找到鼠标位置下的视口，因为它不知道外部窗口。（仅多视口）

**在你的 `ImGui_ImplXXX_NewFrame()` 函数中：**
- 将 `io.DeltaTime` 设置为自上一帧以来经过的时间（以秒为单位）。
- 将 `io.DisplaySize` 设置为你的窗口大小。
- 将 `io.DisplayFrameBufferSize` 设置为你的窗口像素密度（仅 macOS/iOS）。
- 更新鼠标光标形状（如果支持）。

**在你的 `ImGui_ImplXXX_NewFrame()` 函数或事件处理器中：**
- **鼠标支持**
  - 使用 `io.AddMousePosEvent()`、`io.AddMouseButtonEvent()`、`io.AddMouseWheelEvent()` 传递鼠标事件。
  - 如果你能够区分 Mouse、TouchScreen 和 Pen 输入，请使用 `io.AddMouseSourceEvent()`。TouchScreen 和 Pen 输入对于某些 Dear ImGui 功能需要不同的逻辑。
  - 使用 `io.AddMouseViewportEvent()` 指定鼠标正悬停在哪个视口/OS 窗口上。请仔细阅读说明，因为这并不像看起来那么简单！（仅多视口）
- **键盘支持**
  - 使用 `io.AddKeyEvent()` 传递按键事件。
  - 使用 `io.AddInputCharacter()` 传递文本/字符事件。
- **游戏手柄支持**
  - 使用 `io.AddKeyEvent()` 和 `io.AddKeyAnalogEvent()` 传递游戏手柄事件，使用 `ImGuiKey_GamepadXXX` 值。
- **杂项**
  - 剪贴板支持：在 `ImGuiPlatformIO` 中设置 `Platform_GetClipboardTextFn()`、`Platform_SetClipboardTextFn()` 处理器。
  - 在 Shell 中打开支持：在 `ImGuiPlatformIO` 中设置 `Platform_OpenInShellFn()` 处理器。
  - IME 支持：在 `ImGuiPlatformIO` 中设置 `Platform_SetImeDataFn()` 处理器。
  - 使用 `io.AddFocusEvent()` 在应用程序窗口获得/失去焦点时通知。
- **多视口支持**
  - 如果支持，更新显示器列表。
  - 在 `ImGuiPlatformIO` 中设置所有必需的处理器，以创建/销毁/移动/调整大小/设置标题/聚焦等窗口。

### <a id="rendering-implementing-your-renderdrawdata-function"></a>渲染：实现你的 RenderDrawData 函数

注意：设置 `ImGuiBackendFlags_RendererHasVtxOffset` 以表明你的后端能够处理带顶点偏移的渲染（`ImDrawCmd::VtxOffset` 字段）。
否则，渲染将被限制为每个窗口 64K 个顶点，这可能会限制高级绘图。
作为替代方案，你也可以在 `imconfig.h` 文件中使用 `#define ImDrawIdx unsigned int` 来支持 32 位索引。

```cpp
void MyImGuiBackend_RenderDrawData(ImDrawData* draw_data)
{
    // TODO: 更新纹理。
    // - 大多数情况下，该列表只有 1 个状态为 OK 的元素，也就是无事可做。
    // - 这几乎总是指向 ImGui::GetPlatformIO().Textures[]，但它是 ImDrawData 的一部分，以便允许覆盖或禁用纹理更新。
    if (draw_data->Textures != nullptr)
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
                MyImGuiBackend_UpdateTexture(tex);

    // TODO: 设置渲染状态：
    // - 启用 Alpha 混合
    // - 不进行背面剔除
    // - 不进行深度测试，不进行深度写入
    // - 启用剪刀测试（Scissor）
    MyEngineSetupenderState();

    // TODO: 将纹理采样器设置为双线性（而不是 point/nearest 过滤）。

    // TODO: 设置覆盖 draw_data->DisplayPos 到 draw_data->DisplayPos + draw_data->DisplaySize 的视口

    // TODO: 设置覆盖 draw_data->DisplayPos 到 draw_data->DisplayPos + draw_data->DisplaySize 的正交投影矩阵

    // TODO: 设置着色器：顶点 { float2 pos, float2 uv, u32 color }，片段着色器从 1 张纹理采样颜色，乘以顶点颜色。

    // 渲染命令列表
    ImVec2 clip_off = draw_data->DisplayPos;
    ImVec2 clip_scale = draw_data->FramebufferScale;
    for (const ImDrawList* draw_list : draw_data->CmdLists)
    {
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;  // 由 Dear ImGui 生成的顶点缓冲
        const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;   // 由 Dear ImGui 生成的索引缓冲
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                if (pcmd->UserCallback == platform_io.DrawCallback_ResetRenderState)
                    MyEngineSetupSenderState();
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // 将裁剪/剪裁矩形投影到帧缓冲空间
                // - 裁剪坐标在 imgui 坐标空间中提供：
                //   - 对于给定视口，draw_data->DisplayPos == viewport->Pos 且 draw_data->DisplaySize == viewport->Size
                //   - 在单视口应用程序中，draw_data->DisplayPos == (0,0) 且 draw_data->DisplaySize == io.DisplaySize，但始终使用 GetMainViewport()->Pos/Size 而不是硬编码这些值。
                //   - 为了支持多视口应用程序（参见 github 上的 'docking' 分支），
                //     始终从裁剪边界中减去 draw_data->DisplayPos，以将它们转换到你的视口空间。
                // - 注意 pcmd->ClipRect 包含 Min+Max 边界。有些图形 API 可能使用 Min+Max，另一些可能使用 Min+Size（size 即 Max-Min）
                ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                // 我们使用剪刀测试来裁剪某些对象。所有底层图形 API 都应该支持它。
                // - 如果你的引擎还不支持剪刀测试，可以先忽略这一点。你会遇到一些小问题
                //   （某些元素会显示在其边界之外），但你可以在其他一切正常后再修复它！
                MyEngineSetScissor(clip_min.x, clip_min.y, clip_max.x, clip_max.y);

                // 绘制调用的纹理由 pcmd->GetTexID() 指定。
                // 绝大多数绘制调用将使用 Dear ImGui 纹理图集，其值是你自己在初始化期间设置的。
                MyEngineBindTexture((MyTexture*)pcmd->GetTexID());

                // 渲染 'pcmd->ElemCount/3' 个索引三角形。
                // 默认情况下，索引 ImDrawIdx 是 16 位的，如果你的引擎不支持 16 位索引，可以在 imconfig.h 中将它们改为 32 位。
                MyEngineDrawIndexedTriangles(pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer + pcmd->IdxOffset, vtx_buffer, pcmd->VtxOffset);
            }
        }
    }
}
```

### <a id="rendering-adding-support-for-imguibackendflags_rendererhastextures-192"></a>渲染：添加对 `ImGuiBackendFlags_RendererHasTextures` 的支持（1.92+）

版本 [1.92.0](https://github.com/ocornut/imgui/releases/tag/v1.92.0)（2025 年 6 月）在渲染后端中加入了纹理支持，这是支持动态字体缩放等功能的基础。

**为了向前推进并利用所有新功能，到 2026 年 6 月之前，所有后端很可能都将需要支持 `ImGuiBackendFlags_RendererHasTextures`。**

诸如 `Build()`、`GetTexDataAsRGBA32()`、`GetTexDataAsAlpha8()`、`SetTexID()`、`IsBuilt()` 等 `ImFontAtlas` 函数已被废弃，取而代之的是遍历 `Textures[]` 数组，并在 Dear ImGui 请求时更新其状态。

**TD;DR：在标准后端中添加对 `ImGuiBackendFlags_RendererHasTextures` 支持的提交列表：**

- Allegro5: [ee8941e](https://github.com/ocornut/imgui/commit/ee8941e) (+35 行)
- DirectX9: [75efba7](https://github.com/ocornut/imgui/commit/75efba7) (+48 行)
- DirectX10: [2d2b1bc](https://github.com/ocornut/imgui/commit/2d2b1bc) (+40 行)
- DirectX11: [372fd27](https://github.com/ocornut/imgui/commit/372fd27) (+40 行)
- DirectX12: [eefe5d5](https://github.com/ocornut/imgui/commit/eefe5d5) (+87 行)
- Metal: [26c017d](https://github.com/ocornut/imgui/commit/26c017d) (+55 行)
- OpenGL Legacy: [0430c55](https://github.com/ocornut/imgui/commit/0430c55) (+25 行)
- OpenGL3/WebGL/ES: [dbb91a5](https://github.com/ocornut/imgui/commit/dbb91a5) (+47 行)
- SDL_Renderer2: [9fa65cd](https://github.com/ocornut/imgui/commit/9fa65cd) (+20 行)
- SDL_Renderer3: [e538883](https://github.com/ocornut/imgui/commit/e538883) (+19 行)
- SDL_GPU: [16fe666](https://github.com/ocornut/imgui/commit/16fe666) (+41 行)
- Vulkan: [abe294b](https://github.com/ocornut/imgui/commit/abe294b) (+33 行)
- WGPU: [571dae9](https://github.com/ocornut/imgui/commit/571dae9) (+30 行)

**说明：**

- 设置 `ImGuiBackendFlags_RendererHasTextures` 以表明你的后端能够处理该功能。
- 在渲染期间，例如在你的 RenderDrawData 函数中，遍历 `ImDrawData->Textures` 数组并处理所有纹理。
- 在关闭期间，遍历 `ImGui::GetPlatformIO().Textures` 并销毁所有纹理。
- （这两个数组都是 `ImVector<ImTextureData*>`。它们只是位于不同位置，因为：为了让高级用户执行多线程渲染，我们在 ImDrawData 中存储了一个指向纹理列表的指针，目的是让多线程渲染用户用自己的指针替换它。）

处理纹理的伪代码：
```cpp
if (draw_data->Textures != nullptr)
    for (ImTextureData* tex : *draw_data->Textures)
        if (tex->Status != ImTextureStatus_OK)
            MyImGuiBackend_UpdateTexture(tex);
```
```cpp
void MyImGuiBackend_UpdateTexture(ImTextureData* tex)
{
    if (tex->Status == ImTextureStatus_WantCreate)
    {
        // 根据 tex->Width、tex->Height 创建纹理。
        // - 大多数后端只支持 tex->Format == ImTextureFormat_RGBA32。
        // - 特别受内存限制的平台上的后端可能支持 tex->Format == ImTextureFormat_Alpha8。

        // 上传所有纹理像素
        // - 从我们 CPU 侧的纹理副本读取，并复制到你的图形 API。
        // - 按需使用 tex->Width、tex->Height、tex->GetPixels()、tex->GetPixelsAt()、tex->GetPitch()。

        // 存储你的数据，并确认创建完成。
        tex->SetTexID(xxxx); // 指定特定于后端的 ImTextureID 标识符，它将被存储在 ImDrawCmd 中。
        tex->SetStatus(ImTextureStatus_OK);
        tex->BackendUserData = xxxx; // 如需要，存储更多后端数据（大多数后端会分配一个小纹理来在其中存储数据）
    }
    if (tex->Status == ImTextureStatus_WantUpdates)
    {
        // 向现有纹理上传一个像素矩形区域
        // - 我们只会写入从未使用过的纹理区域！
        // - 使用 tex->TexID 或 tex->BackendUserData 取回你存储的数据。
        // - 使用 tex->UpdateRect.x/y、tex->UpdateRect.w/h 获取块的位置和大小。
        //   - 使用 tex->Updates[] 获取 tex->UpdateRect 内的各个子区域。不推荐。
        // - 从我们 CPU 侧的纹理副本读取，并复制到你的图形 API。
        // - 按需使用 tex->Width、tex->Height、tex->GetPixels()、tex->GetPixelsAt()、tex->GetPitch()。

        // 确认更新完成
        tex->SetStatus(ImTextureStatus_OK);
    }
    if (tex->Status == ImTextureStatus_WantDestroy && tex->UnusedFrames > 0)
    {
        // 如果你使用分阶段渲染并且有正在进行的渲染，请根据需要将 tex->UnusedFrames > 0 的检查改为更高的计数，例如 > 2

        // 销毁纹理
        // - 使用 tex->TexID 或 tex->BackendUserData 取回你存储的数据。
        // - 在你的图形 API 中销毁纹理。

        // 确认销毁完成
        tex->SetTexID(ImTextureID_Invalid);
        tex->SetStatus(ImTextureStatus_Destroyed);
    }
}
```
具体示例请参考上文"在标准后端中添加对 `ImGuiBackendFlags_RendererHasTextures` 支持的提交列表"。
