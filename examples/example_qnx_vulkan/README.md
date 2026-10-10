# Dear ImGui QNX Screen + Vulkan 示例

本示例使用：

- `imgui_impl_qnx.cpp` 作为 QNX Screen 平台后端
- 未修改的 `imgui_impl_vulkan.cpp` 渲染器后端
- `vkCreateScreenSurfaceQNX()` 和 `VK_QNX_screen_surface`

无需单独的 Screen/Vulkan 渲染器后端。QNX 相关的工作仅限于 Screen 平台后端、
原生窗口设置以及 Vulkan WSI 表面创建。

## 构建

加载 QNX SDP 环境，然后运行：

```sh
cd examples/example_qnx_vulkan
make
```

构建过程定义 `VK_USE_PLATFORM_SCREEN_QNX` 并链接 `libvulkan` 和
`libscreen`。

## 输入与窗口尺寸调整

应用程序将 Screen 输入事件转发给 `ImGui_ImplQNX_ProcessEvent()`。
当 Screen 窗口尺寸变化，或 Vulkan 报告 `VK_ERROR_OUT_OF_DATE_KHR` /
`VK_SUBOPTIMAL_KHR` 时，它会重新创建 Vulkan 交换链。按 Escape 退出。
