# Dear ImGui QNX Screen + OpenGL ES 3 示例

本示例使用：

- `imgui_impl_qnx.cpp` 作为 QNX Screen 平台后端
- `imgui_impl_opengl3.cpp` 作为渲染器后端
- 使用 EGL 创建 OpenGL ES 3 上下文和 Screen 窗口表面

`imgui_impl_opengl3.cpp` 同时支持桌面 OpenGL 和 OpenGL ES。在 QNX 上，本示例
使用 `IMGUI_IMPL_OPENGL_ES3` 编译它，并使用 `#version 300 es`。
无需单独的 Screen/OpenGL 渲染器后端。

## 构建

加载 QNX SDP 环境，然后运行：

```sh
cd examples/example_qnx_opengl3
make
```

QNX 递归 make 文件会针对 SDP 配置中启用的 CPU 进行构建，并将程序安装为
`example_qnx_opengl3`。

## 输入

应用程序轮询其 `screen_context_t` 事件队列，并将每个非 `SCREEN_EVENT_NONE`
事件转发给 `ImGui_ImplQNX_ProcessEvent()`。按 Escape 退出示例。
