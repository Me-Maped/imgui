# ImGui Theme Studio

独立的主题演示与可复用主题模块，包含只读原生基准和五套自定义主题，适配本仓库的 Dear ImGui 1.93.0 WIP。没有改动 ImGui 核心、后端或其他示例。

| 配置 | 主题 | 实现范围 |
| --- | --- | --- |
| 内置，无配置文件 | 00 ImGui Default | 原始尺寸、深色配色、原生按钮；只读 |
| `cyberpunk.ini` | 01 赛博朋克 | 深蓝黑、青色、品红；无发光 |
| `minimal.ini` | 03 简约留白 | 暖白、灰阶、低饱和绿 |
| `wireframe.ini` | 04 精密线条 | 石墨灰、细边框、琥珀强调、轮廓按钮 |
| `brutalist.ini` | 07 新粗野主义 | 黑描边、蓝黄撞色、实色矩形硬阴影 |
| `bento.ini` | 10 深色模块卡片 | 石墨色卡片、青柠强调、小圆角 |

六款共用应用布局与标准控件。五款自定义主题以视觉稿为参考，并非对 AI 图稿逐像素复刻。00 是该应用使用 ImGui 默认样式的基准，不是另一个 Demo 窗口。

[主题预览与单款截图](../../docs/themes/README.md) · [性能验证记录](../../docs/themes/benchmark.md) · [工程 README](../../docs/README.md#themes--styles)

## 编译与运行

需要 CMake、C++11 编译器、pkg-config、GLFW3、OpenGL 开发依赖；构建不会联网下载依赖。

从仓库根目录执行：

```sh
cmake -S examples/example_theme_studio -B examples/example_theme_studio/build
cmake --build examples/example_theme_studio/build -j 4
examples/example_theme_studio/build/theme_studio
```

默认显示 10。指定其他主题：

```sh
examples/example_theme_studio/build/theme_studio --theme cyberpunk
```

支持的参数：

- `--theme default|cyberpunk|minimal|wireframe|brutalist|bento`
- `--themes DIRECTORY`：五份主题文件所在目录。默认使用本示例源码下的 `themes/`。
- `--font PATH`：自定义 TTF/OTF。默认使用仓库自带 `misc/fonts/DroidSans.ttf`。需要中文时，请指定含中文字形的字体。
- `--frames N`：运行指定帧数后退出，便于自动检查。
- `--capture-dir EXISTING_DIRECTORY`：在隐藏窗口中依次渲染六款主题，将实际 OpenGL 帧缓冲保存为 PPM 后退出。需要可连接的图形会话；输出目录须已存在。
- `--benchmark RESULTS.csv`：固定条件的 CPU/几何量比较，完成后退出。
- `--warmup N` / `--samples N`：比较模式每款主题的预热与采样帧数，默认 120 / 600；仅与 `--benchmark` 一起使用。

默认字体与配置路径由 CMake 指向当前源码目录；搬移程序时，通过上述参数指定资源位置。

## 编辑主题

顶部切换主题，右侧编辑圆角、边框、间距和颜色。更改在下一帧应用，每套主题分别保留内存中的编辑结果。

00 的编辑面板只显示说明，不提供修改/保存操作。主题模块也拒绝对 00 保存或通过配置构造；即使直接修改其内存对象，`Apply()` 仍会应用标准原始外观。00 的语义按钮直接转发给原生 `ImGui::Button()`。

- **Save theme**：保存当前主题到对应 INI。通过同目录临时文件替换，失败时报告错误。
- **Reload**：从磁盘重新读取当前主题。格式错误、未知字段、越界或非有限数值会被拒绝，保留当前外观。
- **Reset to built-in preset**：恢复当前主题的内置外观；需要再点击 Save 才覆盖磁盘文件。
- 切换主题不自动保存，退出不会保存未提交编辑。
- 中间的 Apply、Reset、Delete 是示例数据操作；右侧的主题操作不修改这些数据。

INI 示例：

```ini
[theme]
preset=cyberpunk

[style]
FrameRounding=2
FramePadding=12,6
ItemSpacing=12,8

[colors]
WindowBg=#090D18
ChildBg=#121B2B
Text=#E3F8FF
SliderGrab=#00E5FF

[extras]
accent=#00E5FF
accent_text=#071018
shadow_offset=0
outline_buttons=false
```

`[colors]` 使用 `ImGui::GetStyleColorName()` 返回的名字，接受 `#RRGGBB`、`#RRGGBBAA` 或 `r,g,b,a`（0..1）。`[style]` 仅接受模块中列出的外观字段；所有尺寸都是 DPI=1 的基准值。保存会写出完整的已支持外观字段和颜色。

`[extras]` 管理封装按钮的主色、文字色、危险操作色及硬阴影。修改这里的 `accent` 不会自动覆盖你单独设置的 `[colors]`；滑条、选中行等仍可分别调色。

## 接入自己的程序

将 `../../misc/themes/imgui_themes.cpp` 加入构建，并把同目录加入头文件搜索路径。模块只依赖 `imgui.h` 和 C++ 标准库，不依赖 GLFW、OpenGL 或 `imgui_internal.h`。

```cpp
#include "imgui_themes.h"

// 在 ImGui context 创建后初始化；长期保留 theme 作为未缩放的源数据。
ImGuiThemes::Theme theme = ImGuiThemes::Make(ImGuiThemes::Id::Cyberpunk);
std::string error;
if (!ImGuiThemes::Load("themes/cyberpunk.ini", theme, error))
{
    // 在自己的日志或界面显示 error；失败时 theme 仍是原来的可用值。
}

// 初始化、主题编辑/切换或 DPI 改变时，在 NewFrame() 之前执行。
const float dpi = 1.0f;
ImGuiThemes::Apply(theme, dpi);
```

业务代码可继续使用 `ImGui::Button()`；需要主按钮、危险按钮或轮廓/硬阴影效果时，使用：

```cpp
bool clicked = ImGuiThemes::Button(
    theme, ImGuiThemes::ButtonRole::Primary, "Apply", ImVec2(120, 0), dpi);
// clicked 交给原有业务处理。主题模块不执行任何业务动作。
```

`Apply()` 只复制白名单内的视觉尺寸与颜色，保留字体缩放、透明度和悬停延迟等运行时配置。演示程序自己管理 `FontScaleDpi`；接入其他程序时，字体和 DPI 策略仍由宿主决定。

每次从未缩放主题计算尺寸，避免反复缩放误差。开启的边框至少保留 1 个 ImGui 坐标单位，关闭的边框保持 0。主题文件与窗口布局配置相互独立。

## 验证与开销边界

```sh
ctest --test-dir examples/example_theme_studio/build --output-on-failure
```

无窗口构建测试：

```sh
cmake -S examples/example_theme_studio -B /tmp/imgui-theme-tests -DTHEME_STUDIO_BUILD_APP=OFF
cmake --build /tmp/imgui-theme-tests -j 4
ctest --test-dir /tmp/imgui-theme-tests --output-on-failure
```

自动检查覆盖：

- 反复 DPI 应用不累乘，75% 缩放仍保留开启的边框。
- 换肤保留字体/交互设置及示例数据、选中项。
- 配置错误不产生部分更新，编辑后的颜色和语义参数可保存再加载。
- 六款主题生成实际绘制数据；语义按钮只触发一次点击，并恢复样式栈。
- 00 与原生按钮的顶点位置、颜色及几何量一致；基准外观与持久化保持只读。
- 封装按钮的零尺寸、正尺寸、负尺寸与标准 Button 一致。
- 完全裁切的 100 个硬阴影按钮不生成额外阴影几何。

已在 Linux / GLFW / OpenGL3 / AMD Radeon 780M 上完成实际渲染验证，见[验证截图](../../docs/themes/README.md)。三组测试已通过，新增统计测试覆盖预热排除、样本量、百分位数、CSV 转义/增量、裁切和几何变化拒绝；已完成独立代码审查。

主题没有新增效果通道、纹理材质或渲染回调；07 的硬阴影使用普通实色矩形。绘制命令/顶点数可由 `studio_tests` 输出观察，它们不是 GPU 耗时基准。Windows/macOS、中文字体、多显示器 DPI 和完整人工交互验收尚未验证。

## 原生基准与性能比较

查看 00：

```sh
examples/example_theme_studio/build/theme_studio --theme default
```

运行比较时使用 Release 构建：

```sh
cmake -S examples/example_theme_studio -B examples/example_theme_studio/build-release -DCMAKE_BUILD_TYPE=Release
cmake --build examples/example_theme_studio/build-release -j 4
examples/example_theme_studio/build-release/theme_studio --benchmark /tmp/imgui-theme-benchmark.csv
```

如需更长的采样：

```sh
examples/example_theme_studio/build-release/theme_studio --benchmark /tmp/imgui-theme-benchmark.csv --warmup 240 --samples 3000
```

比较会使用隐藏但真实的 OpenGL 窗口，依次测量 00、01、03、04、07、10：

- 固定 1440×960 逻辑窗口、字号 17、UI scale=1、相同字体与输入数据；记录实际帧缓冲尺寸。
- 使用同一固定组件场景，隐藏主题编辑器、主题名称及动态 FPS 文本；禁用交互，请求 VSync=0。
- UI CPU 范围是 `NewFrame()` → 固定界面构建 → `Render()`，包括同一套可见性诊断；不包括主题应用、backend NewFrame、GPU 提交、交换缓冲、统计排序或文件输出。
- 每款独立预热后采样。中位数采用排序后中间值（偶数时取两者均值）；P95 采用 nearest-rank。
- 所有被检查的组件必须完全可见，各主题提交数量一致；采样期间几何量必须保持稳定。裁切、尺寸改变、错误配置或中途退出会阻止新 CSV 导出，已有结果文件保留。
- CSV 包含 CPU 中位数/P95、顶点数、索引数、非空且与视口相交的普通绘制命令数、相对 00 的增量，以及字体路径、渲染器、版本、尺寸和断言构建标记。

测试按固定顺序运行，CPU 结果仍会受系统调度、频率和后台任务影响。CSV 使用运行时加载的五份配置；修改配置后应重新测量，并保留所用配置才能复现。不要把单次微秒级排序当作普遍结论，也不要把 CPU 指标解释为 GPU 或整个应用帧耗时。

已归档的实际采样数据见 [benchmark.csv](../../docs/themes/benchmark.csv)，条件与解释见[性能验证记录](../../docs/themes/benchmark.md)。
