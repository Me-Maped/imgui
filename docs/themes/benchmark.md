# 00 原生样式与五款主题：首轮实际比较

这是真实 OpenGL 窗口下的 UI 构建 CPU 与几何量采样，不是 GPU 计时。

## 条件

- 仓库版本：Dear ImGui 1.93.0 WIP。
- CMake Release 构建，NDEBUG；Linux / GLFW / OpenGL3。
- 渲染器：AMD Radeon 780M Graphics (radeonsi, phoenix, ACO, DRM 3.64, 7.2.9-2-cachyos)。
- 逻辑窗口与实际帧缓冲均为 1440×960，DroidSans，字号 17，UI scale=1。
- 每款预热 120 帧，随后采样 600 帧；固定顺序 00、01、03、04、07、10。
- 隐藏窗口，请求 VSync=0；相同固定场景，25 个被检查的界面项全部可见。
- CPU 计时包含 NewFrame、场景构建/可见性检查、Render；不包含实际 OpenGL 提交或交换缓冲。

使用相同参数重新运行（从仓库根目录执行，先按[使用说明](../../examples/example_theme_studio/README.md)完成 Release 构建）：

```sh
examples/example_theme_studio/build-release/theme_studio --benchmark /tmp/imgui-theme-benchmark.csv
```

## 观测结果

| 主题 | CPU 中位数 μs | CPU P95 μs | 相对 00 μs | 顶点 | 索引 | 绘制命令 |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| 00 原始 | 6.602 | 14.838 | 0.000 | 1488 | 2295 | 3 |
| 01 赛博朋克 | 7.214 | 7.665 | +0.612 | 1736 | 3147 | 3 |
| 03 简约留白 | 8.156 | 16.872 | +1.554 | 1716 | 3153 | 3 |
| 04 精密线条 | 8.822 | 23.354 | +2.220 | 1592 | 2613 | 3 |
| 07 新粗野主义 | 8.145 | 15.860 | +1.543 | 1608 | 2637 | 3 |
| 10 深色模块卡片 | 8.727 | 15.279 | +2.125 | 1760 | 3255 | 3 |

本次场景中，自定义主题增加了一些几何量，但没有增加绘制命令数。CPU 中位数增量在约 0.6–2.2 μs 之间。P95 明显波动，且统计窗口较短，不能据此断言某款主题在其他机器或场景中始终更快。

原始测量值及元信息见 [benchmark.csv](benchmark.csv)。归档时仅将字体路径规范为仓库相对路径 `misc/fonts/DroidSans.ttf`，所有测量数值保持原样。若后续修改主题配置或场景，须重新采样；这里保留的是该次运行的记录。

返回[主题预览](README.md)或[构建与使用说明](../../examples/example_theme_studio/README.md)。
