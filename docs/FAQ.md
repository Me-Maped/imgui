# <a id="faq-frequently-asked-questions"></a>FAQ（常见问题解答）

你可以使用短链接引用本文档：
  https://www.dearimgui.com/faq
或它的真实地址：
  https://github.com/ocornut/imgui/blob/master/docs/FAQ.md
或者用任意 Markdown 查看器查看此文件。


## <a id="index"></a>索引

### <a id="basics"></a>基础

- [文档在哪里？](#q-where-is-the-documentation)
- [这个库叫什么名字？](#q-what-is-this-library-called)
- [Dear ImGui 与传统 UI 工具包有何区别？](#q-what-is-the-difference-between-dear-imgui-and-traditional-ui-toolkits)
- [我应该使用哪个版本？](#q-which-version-should-i-get)

### <a id="integration"></a>集成

- **[如何开始？](#q-how-to-get-started)**
- **[我如何判断应该把鼠标/键盘事件分发给 Dear ImGui 还是我的应用程序？](#q-how-can-i-tell-whether-to-dispatch-mousekeyboard-to-dear-imgui-or-my-application)**
- [如何启用键盘或游戏手柄控制？](#q-how-can-i-enable-keyboard-or-gamepad-controls)
- [如何在没有鼠标、键盘或屏幕的机器上使用它？（输入共享、远程显示）](#q-how-can-i-use-this-on-a-machine-without-mouse-keyboard-or-screen-input-share-remote-display)
- [如何创建我自己的后端？](#q-how-can-i-create-my-own-backend)
- [我把 Dear ImGui 集成到引擎中，但显示的是小方块而不是文字……](#q-i-integrated-dear-imgui-in-my-engine-and-little-squares-are-showing-instead-of-text)
- [我把 Dear ImGui 集成到引擎中，移动窗口时有些元素被裁剪或消失……](#q-i-integrated-dear-imgui-in-my-engine-and-some-elements-are-clipping-or-disappearing-when-i-move-windows-around)
- [我把 Dear ImGui 集成到引擎中，有些元素显示在了预期窗口边界之外……](#q-i-integrated-dear-imgui-in-my-engine-and-some-elements-are-displaying-outside-their-expected-windows-boundaries)

### <a id="usage"></a>用法

- **[关于 ID 栈系统……](#q-about-the-id-stack-system)**
  - **[如何让多个控件使用相同的标签？](#q-how-can-i-have-multiple-widgets-with-the-same-label)（使用 `##` 或 `PushID()`）**
  - **[如何让控件使用空标签？](#q-how-can-i-have-widgets-with-an-empty-label)（使用 `##`）**
  - **[如何让标签动态变化？](#q-how-can-i-make-a-label-dynamic)（使用 `###`）**
  - **[标签与 ID 栈系统的总体说明。](#general-description-of-the-label-and-id-stack-system)**
- [如何显示图像？](#q-how-can-i-display-an-image)、[ImTextureID/ImTextureRef 是什么？](#q-what-are-imtextureidimtextureref)
- [如何对 ImVec2 使用数学运算符？](#q-how-can-i-use-maths-operators-with-imvec2)
- [如何使用我自己的数学类型代替 ImVec2/ImVec4？](#q-how-can-i-use-my-own-maths-types-instead-of-imvec2imvec4)
- [如何与标准 C++ 类型（如 std::string 和 std::vector）交互？](#q-how-can-i-interact-with-standard-c-types-such-as-stdstring-and-stdvector)
- [如何显示自定义形状？（使用低层 ImDrawList API）](#q-how-can-i-display-custom-shapes-using-low-level-imdrawlist-api)
- [关于多线程](#about-multi-threading)

### <a id="fonts-text"></a>字体、文本

- [我的应用程序中应该如何处理 DPI？](#q-how-should-i-handle-dpi-in-my-application)
- [如何加载非默认的字体？](#q-how-can-i-load-a-different-font-than-the-default)
- [如何在我的应用程序中方便地使用图标？](#q-how-can-i-easily-use-icons-in-my-application)
- [如何加载多种字体？](#q-how-can-i-load-multiple-fonts)
- [如何显示并输入非拉丁字符，例如中文、日文、韩文、西里尔文？](#q-how-can-i-display-and-input-non-latin-characters-such-as-chinese-japanese-korean-cyrillic)

### <a id="concerns"></a>顾虑

- [谁在使用 Dear ImGui？](#q-who-uses-dear-imgui)
- [可以用 Dear ImGui 创建复杂/严肃的工具吗？](#q-can-you-create-elaborateserious-tools-with-dear-imgui)
- [可以更换 Dear ImGui 的外观吗？](#q-can-you-reskin-the-look-of-dear-imgui)
- [为什么使用 C++（而不是 C）？](#q-why-using-c-as-opposed-to-c)

### <a id="community"></a>社区

- [我该如何提供帮助？](#q-how-can-i-help)

--------

# <a id="qa-basics"></a>问答：基础

### <a id="q-where-is-the-documentation"></a>问：文档在哪里？

**这个库目前的文档还很不完善，并且假定用户熟悉 C/C++。**
- [Wiki](https://github.com/ocornut/imgui/wiki) 是许多资源和链接的集散地。
- 实用的[入门](https://github.com/ocornut/imgui/wiki/Getting-Started)指南，介绍如何将 Dear ImGui 集成到现有应用程序中。
- [examples/](https://github.com/ocornut/imgui/blob/master/examples/) 文件夹中提供了 20 多个使用 OpenGL/DirectX 等的独立示例应用程序，用于说明如何将 Dear ImGui 与自己的引擎/应用程序集成。你可以运行这些应用程序并探索它们。
- 参见 [imgui_demo.cpp](https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp) 中的演示代码，尤其是 `ImGui::ShowDemoWindow()` 函数。该演示涵盖了 Dear ImGui 的大部分功能，因此你可以阅读代码并查看其输出。
- 参见 pthom 的在线 [imgui_explorer](https://pthom.github.io/imgui_explorer)，它是带源码浏览器的网页版演示。
- 参见文档：[Backends](https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md)、[Examples](https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md)、[Fonts](https://github.com/ocornut/imgui/blob/master/docs/FONTS.md)。
- 参见 [imgui.cpp](https://github.com/ocornut/imgui/blob/master/imgui.cpp) 顶部的文档和注释，以及 [imgui.h](https://github.com/ocornut/imgui/blob/master/imgui.h) 中通用的 API 注释。
- [术语表](https://github.com/ocornut/imgui/wiki/Glossary)页面可能会有用。
- 可以在 [Issues](https://github.com/ocornut/imgui/issues) 和 [Discussions](https://github.com/ocornut/imgui/discussions) 版块中搜索以往的问题和议题。
- 你的编程 IDE 是你的好帮手，找到类型或函数声明即可找到与之关联的注释。
- `ImGui::ShowMetricsWindow()` 函数会暴露大量内部信息和工具。虽然它主要被设计为调试工具，但能访问这些信息通常有助于理解相关概念。

##### [返回索引](#index)

---

### <a id="q-what-is-this-library-called"></a>问：这个库叫什么名字？

**这个库叫做 Dear ImGui**。请称它为 Dear ImGui（而不是 ImGui，也不是 IMGUI）。

（这个库在 2014 年发布 1.0 版本时误打误撞地以 "ImGui" 之名诞生，因为当时我没有给它起一个合适的名字，也完全没有预料到它会流行起来。然而，IMGUI（immediate-mode graphical user interface，即时模式图形用户界面）这个术语在此之前就已被创造出来，并被用于各种其他场合，例如 Unity 就使用了自己的 IMGUI 范式实现。为了在不影响现有代码库的前提下减少歧义，我在 2015 年 12 月为这个库决定了一个完全限定名称 "Dear ImGui"。

##### [返回索引](#index)

---

### <a id="q-what-is-the-difference-between-dear-imgui-and-traditional-ui-toolkits"></a>问：Dear ImGui 与传统 UI 工具包有何区别？

以下是对 Dear ImGui 与传统工具包所采用方法的非常简化的比较：

| Dear ImGui | Qt/GTK/WPF…… |
|--------------------------|--------------------------|
| 每次更新都完整地发出 UI。 | UI 只发出一次，之后再进行修改。 |
| UI 布局完全是动态的，可以随时变化。<BR>UI 通常以编程方式发出，这有利于反映动态的数据集合。 | UI 布局大多是静态的。<BR>UI 可以通过编程方式发出，也可以来自离线工具创建的数据。UI 需要额外的代码才能演进，而如果需要反映动态数据和系统，这往往既繁琐又容易出错。 |
| UI 系统针对最坏情况进行了优化，即使频繁变化也能保持最优性能。 | UI 系统针对没有任何变化的情况进行了优化。一旦发生变化，性能往往会下降。 |
| 应用程序可以基于任意逻辑提交 UI，而无需记录该逻辑的所有方面。 | 应用程序需要对 UI 元素进行更多的记录管理。 |
| UI 库存储的数据量极少。在某一时刻，它通常不知道也不记得还显示了哪些其他控件、接下来会来哪些控件。因此，某些布局功能（对齐、调整大小）实现起来不那么容易，或者需要专门的代码。 | UI 库存储整个控件树和状态。UI 库可以利用这些保留的数据轻松地进行布局。 |
| UI 代码可以添加在任何地方。<BR>你甚至可以创建 UI 来编辑一个局部变量！ | UI 代码需要添加在专门的位置。 |
| UI 布局/逻辑/操作/数据绑定在代码中都彼此相邻。 | UI 布局/逻辑/操作/数据绑定位于不同的函数、文件或格式中。 |
| 数据天然始终同步。 | 使用回调/信号/槽来同步数据（容易出错）。 |
| API 简单易学。特别是做基本的事情非常容易。<BR>对更多程序员有吸引力。 | API 更复杂、更专门化。<BR>往往需要专门的程序员。 |
| API 是低层的。抽象很少，使用原始的语言类型和你已有的数据。 | API 层次更高。有更多抽象和高级语言特性。 |
| 外观不那么花哨。但在不断改进 :) | 标准的外观。 |
| 自己编译。易于调试、改造、修改、研究。 | 大多使用预编译库。编译、修改或研究即使并非不可能，也令人生畏。 |
| 可在所有平台运行（包括 Web、游戏主机、移动设备、遗留系统）。 | 在主要桌面平台上运行。 |

地道的 Dear ImGui 代码：
```cpp
if (ImGui::Button("Save"))
    MySaveFunction();

ImGui::SliderFloat("Slider", &m_MyValue, 0.0f, 1.0f);
```
使用传统工具包的地道代码：
```cpp
UiButton* button = new UiButton("Save");
button->OnClick = &MySaveFunction;
parent->Add(button);

UiSlider* slider = new UiSlider("Slider");
slider->SetRange(0.0f, 1.0f);
slider->BindData<float>(&m_MyValue);
parent->Add(slider);
```
这只是为了让你对主要区别有一个直观感受，但其中的优劣比这要深入得多。

其中一些特性通常与笼统的术语 "IMGUI" 相关联，但该术语并没有简单且公认的定义。关于 IMGUI 的含义存在许多错误的说法和误解。部分原因是大多数流行的 IMGUI 实现（包括 Dear ImGui）都源自游戏行业的需求，并针对特定的用例，导致人们将 IMGUI 的特性与某个特定库的行为混为一谈。然而，完全有可能实现一个与 Dear ImGui 特性截然不同的 IMGUI 库。我对 IMGUI 定义的理解是：

**IMGUI 指的是 API：字面意义上就是应用程序与 UI 系统之间的接口。**
- IMGUI API 倾向于让应用程序代码拥有自己的数据，并成为数据的唯一事实来源。
- IMGUI API 尽量减少应用程序需要保留/管理与 UI 系统相关的数据。
- IMGUI API 尽量减少 UI 系统需要保留/管理与应用程序相关的数据。
- 应用程序数据与 UI 数据之间的同步是自然而然的，并且更不容易出错。

**IMGUI 并不指实现。UI 库代码内部发生什么都不重要。**
<BR>另见：[关于 IMGUI 范式的许多文章链接](https://github.com/ocornut/imgui/wiki/#about-the-imgui-paradigm)。

##### [返回索引](#index)

---

### <a id="q-which-version-should-i-get"></a>问：我应该使用哪个版本？
我偶尔会打上[发布版本](https://github.com/ocornut/imgui/releases)标签，但一般来说，同步到 master/最新版本是安全且推荐的。这个库相当稳定，并且一旦报告回归问题，通常很快就会被修复。

你可以使用 ['docking'](https://github.com/ocornut/imgui/tree/docking) 分支，它包含：
- [停靠（Docking）功能](https://github.com/ocornut/imgui/wiki/Docking)
- [多视口（Multi-viewport）功能](https://github.com/ocornut/imgui/wiki/Multi-Viewports)

许多项目都在使用这个分支，并且它会定期与 master 保持同步。

##### [返回索引](#index)

----

# <a id="qa-integration"></a>问答：集成

### <a id="q-how-to-get-started"></a>问：如何开始？

阅读[入门指南](https://github.com/ocornut/imgui/wiki/Getting-Started)。<BR>
阅读 [docs/EXAMPLES.md](https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md)。<BR>
阅读 [docs/BACKENDS.md](https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md)。<BR>
阅读 [imgui.cpp](https://github.com/ocornut/imgui/blob/master/imgui.cpp) 的 `PROGRAMMER GUIDE`（程序员指南）部分。<BR>
[Wiki](https://github.com/ocornut/imgui/wiki) 是许多资源和链接的集散地。

对于初次使用、在编译/链接/运行或加载字体方面遇到问题的用户，请使用 [GitHub Discussions](https://github.com/ocornut/imgui/discussions)。

##### [返回索引](#index)

---

### <a id="q-how-can-i-tell-whether-to-dispatch-mousekeyboard-to-dear-imgui-or-my-application"></a>问：我如何判断应该把鼠标/键盘事件分发给 Dear ImGui 还是我的应用程序？

你可以从 ImGuiIO 结构中读取 `io.WantCaptureMouse`、`io.WantCaptureKeyboard` 和 `io.WantTextInput` 标志。
- 当设置了 `io.WantCaptureMouse` 时，你需要对底层应用程序屏蔽/隐藏鼠标输入。
- 当设置了 `io.WantCaptureKeyboard` 时，你需要对底层应用程序屏蔽/隐藏键盘输入。
- 当设置了 `io.WantTextInput` 时，你可以通知你的操作系统/引擎弹出屏幕键盘（如果有的话，例如在手机上或主机操作系统中）。

重要：无论 `io.WantCaptureMouse`/`io.WantCaptureKeyboard` 的值如何，你都应该始终把鼠标/键盘输入传给 Dear ImGui。这是因为，例如我们需要检测你点击了空白区域以便让其自身的窗口失去焦点，以及其它原因。

```cpp
void MyLowLevelMouseButtonHandler(int button, bool down)
{
    // (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, down);

    // (2) ONLY forward mouse data to your underlying app/game.
    if (!io.WantCaptureMouse)
        my_game->HandleMouseData(...);
}
```

**注意：** `io.WantCaptureMouse` 比任何手动尝试“检查鼠标是否悬停在某个窗口上”都要更正确（不要那样做！）。它能正确处理鼠标拖拽（无论是从你的应用程序上开始拖拽，还是从某个 Dear ImGui 窗口上开始拖拽），并能处理例如弹出窗口和模态窗口对输入的拦截。

**注意：** 文本输入控件会在回车键的 "KeyDown" 事件时释放焦点，因此你的应用程序随后收到的 "KeyUp" 事件通常会带有 `io.WantCaptureKeyboard == false`。根据你的应用程序逻辑，收到该 KeyUp 事件可能会也可能不会造成不便。你可能想跟踪哪些按键按下事件是针对 Dear ImGui 的，例如用一个 bool 数组，并过滤掉相应的按键抬起事件。）

##### [返回索引](#index)

---

### <a id="q-how-can-i-enable-keyboard-or-gamepad-controls"></a>问：如何启用键盘或游戏手柄控制？
- 游戏手柄/键盘导航功能相当可用，并且一直在改进。最初的关注点是对游戏手柄的支持，但键盘的可用性正变得越来越好。游戏手柄支持在使用 Dear ImGui 于游戏主机（例如 PS4、Switch、XB1）且没有连接鼠标时特别有用！
- 键盘：设置 `io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard` 以启用。
- 游戏手柄：设置 `io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad` 以启用（需要支持它的后端）。
- 参见[游戏手柄的按键说明图](https://www.dearimgui.com/controls_sheets)（适用于 PS4、XB1、Switch 游戏手柄的 PNG/PSD 参考图）。
- 更多细节参见 [imgui.cpp](https://github.com/ocornut/imgui/blob/master/imgui.cpp) 的 `USING GAMEPAD/KEYBOARD NAVIGATION CONTROLS` 部分。

##### [返回索引](#index)

---

### <a id="q-how-can-i-use-this-on-a-machine-without-mouse-keyboard-or-screen-input-share-remote-display"></a>问：如何在没有鼠标、键盘或屏幕的机器上使用它？（输入共享、远程显示）
- 你可以借助 [Synergy](https://symless.com/synergy) 等方案，把电脑鼠标无缝共享给主机/平板/手机。
这是提升开发者效率的首选方案。
特别是 [micro-synergy-client 仓库](https://github.com/symless/micro-synergy-client) 提供了简单
且可移植的源代码（uSynergy.c/.h），实现了一个小型可嵌入客户端，你可以在任何平台上使用它，基于 Synergy 1.x 协议
连接到你的宿主计算机。请确保在电脑上下载 Synergy 1 服务端。
主机 SDK 有时也提供类似 Synergy 协议的等价工具或封装。
- 游戏主机用户：考虑用 DualShock4 触摸板或一个闲置的模拟摇杆来模拟鼠标光标，作为鼠标模拟的备选方案。
- 你也可以使用第三方方案，例如 [netImgui](https://github.com/sammyfreg/netImgui)、[Remote ImGui](https://github.com/JordiRos/remoteimgui) 或 [imgui-ws](https://github.com/ggerganov/imgui-ws)，它们通过网络发送顶点进行渲染，让你即使在无屏幕的机器上也能使用 Dear ImGui。大部分细节参见 [Wiki](https://github.com/ocornut/imgui/wiki) 索引。
- 对于触摸输入，你可以增大控件的命中区域（通过 `style.TouchPadding` 设置）来适应触摸输入精度不足的问题，但建议你使用鼠标或游戏手柄，以便针对屏幕空间利用率和精度进行优化。

##### [返回索引](#index)

---

### <a id="q-how-can-i-create-my-own-backend"></a>问：如何创建我自己的后端？
- 参见 [docs/BACKENDS.md](https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md)。
- 参见 imgui.cpp 顶部的文档。

##### [返回索引](#index)

---

### <a id="q-i-integrated-dear-imgui-in-my-engine-and-little-squares-are-showing-instead-of-text"></a>问：我把 Dear ImGui 集成到引擎中，但显示的是小方块而不是文字……
你的渲染后端没有正确使用字体纹理，或者它还没有被上传到 GPU。
- 如果是在使用标准后端时发生（1.92 之前）：A) 你是否在 `ImGui_ImplXXX_NewFrame()` 之后修改了字体图集？B) 可能是纹理上传失败，**如果你的纹理图集太大就会这样**。另见 [docs/FONTS.md](https://github.com/ocornut/imgui/blob/master/docs/FONTS.md)。
- 如果是在使用标准后端时发生（1.92 之后）：请报告问题。
- 如果是在使用自定义后端时发生：请确保你已经把字体纹理上传到 GPU，并且所有着色器和渲染状态都设置正确（例如纹理已绑定）。将你的代码与现有后端进行比较，并使用 [RenderDoc](https://renderdoc.org) 之类的图形调试器来调试你的渲染状态。

##### [返回索引](#index)

---

### <a id="q-i-integrated-dear-imgui-in-my-engine-and-some-elements-are-clipping-or-disappearing-when-i-move-windows-around"></a>问：我把 Dear ImGui 集成到引擎中，移动窗口时有些元素被裁剪或消失……
### <a id="q-i-integrated-dear-imgui-in-my-engine-and-some-elements-are-displaying-outside-their-expected-windows-boundaries"></a>问：我把 Dear ImGui 集成到引擎中，有些元素显示在了预期窗口边界之外……
你的渲染函数中很可能没有正确处理裁剪矩形。
每条绘制命令都需要使用 ImDrawCmd 结构中提供的裁剪矩形（`ImDrawCmd->CllipRect`）来渲染三角形。
Dear ImGui 提供的矩形定义方式为
`(x1=left,y1=top,x2=right,y2=bottom)`
而**不是**
`(x1,y1,width,height)`。
关于如何处理 `ClipRect` 字段，请参考 [backends/](https://github.com/ocornut/imgui/tree/master/backends) 文件夹中的渲染后端。
例如，[DirectX11 后端](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_dx11.cpp)就是这么做的：
```cpp
// Project scissor/clipping rectangles into framebuffer space
ImVec2 clip_off = draw_data->DisplayPos;
ImVec2 clip_min(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y);
ImVec2 clip_max(pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
    continue;

// Apply scissor/clipping rectangle
const D3D11_RECT r = { (LONG)clip_min.x, (LONG)clip_min.y, (LONG)clip_max.x, (LONG)clip_max.y };
ctx->RSSetScissorRects(1, &r);
```

##### [返回索引](#index)

---

# <a id="qa-usage"></a>问答：用法

## <a id="q-about-the-id-stack-system"></a>问：关于 ID 栈系统……

**使用相同的标签+ID 是最常见的用户错误！**
<br>**使用空标签等同于使用与父级控件相同的标签！**
<br>阅读本节中的问题，以更全面地理解 Dear ImGui 中标签和 ID 的工作方式。

简而言之；
- 控件标签也用于计算控件的唯一标识符。
- 唯一标识符是标签和父级作用域（例如父窗口或父树节点标签）的哈希值。
- 你可以使用 `PushID()` 向标识符追加内容，同时不使其可见。
- 你可以在标签中使用 `"##something"` 向标识符追加内容，同时不使其可见。
- 你可以在标签中使用 `"###something"` 使标识符忽略可见部分。

<table>
<tr>
<td><img src="https://github.com/user-attachments/assets/776a8315-1164-4178-9a8c-df52e0ff28aa"></td>
<td>
<pre lang="cpp">
ImGui::Begin("Incorrect!");
ImGui::DragFloat2("My value", &objects[0]->pos.x);
ImGui::DragFloat2("My value", &objects[1]->pos.x);
ImGui::DragFloat2("My value", &objects[2]->pos.x);
ImGui::End();
&nbsp;
ImGui::Begin("Correct!");
ImGui::DragFloat2("My value", &objects[0]->pos.x);
ImGui::DragFloat2("My value##2", &objects[1]->pos.x);
ImGui::DragFloat2("My value##3", &objects[2]->pos.x);
ImGui::End();
&nbsp;
ImGui::Begin("Also Correct!");
for (int n = 0; n < 3; n++)
{
    ImGui::PushID(n);
    ImGui::DragFloat2("My value", &objects[n]->pos.x);
    ImGui::PopID();
}
ImGui::End();
</pre>
</td>
</tr>
</table>

### <a id="q-how-can-i-have-multiple-widgets-with-the-same-label"></a>问：如何让多个控件使用相同标签？

A. 当控件位于同一作用域内且数量有限时，你可以使用 `"##something"` 后缀，该后缀将成为标识符的一部分，但不会作为标签显示。

```cpp
Button("Play");        // Label = "Play",   ID = hash of ("MyWindow", "Play")
Button("Play##foo1");  // Label = "Play",   ID = hash of ("MyWindow", "Play##foo1")  // Different from other buttons
Button("Play##foo2");  // Label = "Play",   ID = hash of ("MyWindow", "Play##foo2")  // Different from other buttons
```

B. 更一般地，例如在循环中，你可以使用 `PushID()/PopID()` 压入一个前缀，该前缀将成为标识符的一部分。

```cpp
// Using PushID() with a string
for (int i = 0; i < 100; i++)
{
  MyObject* obj = Objects[i];
  PushID(obj->Name);
  Button("Click");     // Label = "Click",  ID = hash of ("Window", obj->Name, "Click")
  PopID();
}
```

```cpp
// Using PushID() with an index
for (int i = 0; i < 100; i++)
{
  PushID(i);
  Button("Click");     // Label = "Click",  ID = hash of ("Window", i, "Click")
  PopID();
}
```

### <a id="q-how-can-i-have-widgets-with-an-empty-label"></a>问：如何让控件使用空标签？

如果你想完全隐藏标签，但仍需要 ID：

```cpp
Checkbox("##On", &b);  // Label = "",       ID = hash of (..., "##On")   // No visible label, just a checkbox!
```
##### [返回索引](#index)

### <a id="q-how-can-i-make-a-label-dynamic"></a>问：如何让标签动态变化？

Dear ImGui 非常动态，因此你可以在每一帧提交不同的控件。
然而，为了保留控件状态（例如哪个树节点处于打开状态、哪个按钮处于焦点状态），库内部会引用它们的唯一 ID。

有时你可能希望在保持 ID 不变的同时更改标签。这样可以在保留关联状态的同时更改/动态显示标签。
例如，你可能希望在窗口标题栏或按钮标签中包含不断变化的信息。

使用 "###" 可将前面部分排除在 ID 计算之外：
```cpp
Button("Hello###ID");   // Label = "Enable",   ID = hash of (..., "ID")
Button("World###ID");   // Label = "Disable",  ID = hash of (..., "ID")  // Same ID, different label
```
使用相同的 ID 可确保相关状态（例如控件是否处于焦点）不会在标签变化时丢失。

<table>
<tr>
<td><img src="https://github.com/user-attachments/assets/2bf18756-e122-498d-bbd4-5c44bb1018d5"></td>
<td>
<pre lang="cpp">
// Window label has animating FPS counter
// Window ID stays the same = hash of "MyGame"
char buf[128];
sprintf(buf, "My game (%.1f FPS)###MyGame", io.Framerate);
ImGui::Begin(buf);
&nbsp;
// Label changes between "Enable" and "Disable"
// ID stays the same = hash of ("MyGame", "MyButton")
if (ImGui::Button(enabled ? "Disable###MyButton" : "Enable###MyButton", { -FLT_MIN, 0.0f }))
    enabled = !enabled;
&nbsp;
ImGui::End();
</pre>
</td>
</tr>
</table>

（提示：我建议将 sprintf 包装成更紧凑易用的形式，例如我个人使用的 [ocornut/Str](https://github.com/ocornut/Str)。）

##### [返回索引](#index)

### <a id="general-description-of-the-label-and-id-stack-system"></a>标签和 ID 栈系统的一般说明

Dear ImGui 内部需要唯一标识 UI 元素。
通常不可点击的元素（例如对 Text() 函数的调用）不需要 ID。
交互式控件（例如对 Button() 函数的调用）需要唯一 ID。

**唯一 ID 在内部用于跟踪活动控件，并偶尔将状态关联到控件。<BR>
唯一 ID 是由多个元素的哈希值隐式构建的，这些元素标识了指向 UI 元素的“路径”。**

你可以使用 `Demo>Tools>ID Stack Tool` 或调用 `ImGui::ShowIDStackToolWindow()`。该工具会显示创建唯一 ID 的中间值，使调试和理解更加容易。

![Stack tool](https://user-images.githubusercontent.com/8225057/136235657-a0ea5665-dcd1-423f-9be6-dc3f8ced8f12.png)

- 唯一 ID 通常派生自字符串标签，并且至少限定在其宿主窗口的作用域内：
```cpp
Begin("MyWindow");
Button("OK");          // Label = "OK",     ID = hash of ("MyWindow", "OK")
Button("Cancel");      // Label = "Cancel", ID = hash of ("MyWindow", "Cancel")
End();
```
- 其他元素（例如树节点等）也会压入 ID 栈：
```cpp
Begin("MyWindow");
if (TreeNode("MyTreeNode"))
{
    Button("OK");      // Label = "OK",     ID = hash of ("MyWindow", "MyTreeNode", "OK")
    TreePop();
}
End();
```
- 在不同窗口或不同树位置中，两个标签为 “OK” 的项不会发生冲突：
```cpp
Begin("MyFirstWindow");
Button("OK");          // Label = "OK",     ID = hash of ("MyFirstWindow", "OK")
End();
Begin("MyOtherWindow");
Button("OK");          // Label = "OK",     ID = hash of ("MyOtherWindow", "OK")
End();
```

- 如果在同一位置两次使用相同的 ID，就会发生冲突：
```cpp
Begin("MyWindow");
Button("OK");
Button("OK");      // ERROR: ID collision with the first button! Interacting with either button will trigger the first one.
Button("");        // ERROR: ID collision with Begin("MyWindow")!
End();
```
别担心！这很容易解决，而且有很多解决方法！

- 在简单/局部上下文中解决 ID 冲突：
传递标签时，你可以选择在字符串本身中指定额外的 ID 信息。
使用 "##" 传递 ID 的补充信息，这些信息不会对最终用户可见。
当你知道在编译时将要创建哪些项等情况时，这有助于解决简单的冲突情况：
```cpp
Begin("MyWindow");
Button("Play");        // Label = "Play",   ID = hash of ("MyWindow", "Play")
Button("Play##foo1");  // Label = "Play",   ID = hash of ("MyWindow", "Play##foo1")  // Different from other buttons
Button("Play##foo2");  // Label = "Play",   ID = hash of ("MyWindow", "Play##foo2")  // Different from other buttons
Button("##foo");       // Label = "",       ID = hash of ("MyWindow", "##foo")       // Different from window
End();
```

- 以更通用的方式解决 ID 冲突：
使用 `PushID()` / `PopID()` 创建作用域并操作 ID 栈，以避免同一窗口内的 ID 冲突。在以编程方式迭代和创建大量 UI 元素时，这是区分 ID 最方便的方法。
你可以将指针、字符串或整数值压入 ID 栈。
请记住，ID 是由压入 ID 栈的_所有内容_连接而成的。
在栈的每一层，我们存储用于该 ID 栈层级中各项的种子。
```cpp
Begin("Window");
for (int i = 0; i < 100; i++)
{
  PushID(i);           // Push i to the id tack
  Button("Click");     // Label = "Click",  ID = hash of ("Window", i, "Click")
  PopID();
}
for (int i = 0; i < 100; i++)
{
  MyObject* obj = Objects[i];
  PushID(obj);
  Button("Click");     // Label = "Click",  ID = hash of ("Window", obj pointer, "Click")
  PopID();
}
for (int i = 0; i < 100; i++)
{
  MyObject* obj = Objects[i];
  PushID(obj->Name);
  Button("Click");     // Label = "Click",  ID = hash of ("Window", obj->Name, "Click")
  PopID();
}
End();
```
- 你可以将多个前缀堆叠到 ID 栈中：
```cpp
Button("Click");       // Label = "Click",  ID = hash of (..., "Click")
PushID("node");
  Button("Click");     // Label = "Click",  ID = hash of (..., "node", "Click")
  PushID(my_ptr);
    Button("Click");   // Label = "Click",  ID = hash of (..., "node", my_ptr, "Click")
  PopID();
PopID();
```
- 树节点通过调用 `PushID()` 隐式为你创建作用域：
```cpp
Button("Click");       // Label = "Click",  ID = hash of (..., "Click")
if (TreeNode("node"))  // <-- this function call will do a PushID() for you (unless instructed not to, with a special flag)
{
  Button("Click");     // Label = "Click",  ID = hash of (..., "node", "Click")
  TreePop();
}
```

处理树结构时，ID 用于保留每个树节点的打开/关闭状态。
根据你的使用场景，你可能希望使用字符串、索引或指针作为 ID。
- 例如，当跟踪一个可能随时间变化的单一指针时，使用静态字符串作为 ID，可以在目标对象发生变化时保留节点的打开/关闭状态。
- 例如，在显示对象列表时，使用索引或指针作为 ID 会以不同方式保留节点的打开/关闭状态。请根据你的具体情况判断哪种方式更合理！

##### [返回索引](#index)

---
### <a id="q-how-can-i-display-an-image"></a>问：如何显示一张图片？
### <a id="q-what-are-imtextureidimtextureref"></a>问：ImTextureID/ImTextureRef 是什么？

**简要说明：**
- 你可以使用 `ImGui::Image()`、`ImGui::ImageButton()` 等函数，或更低层的 `ImDrawList::AddImage()`，来发出使用你自己纹理的绘制调用。
- 若要加载并显示你自己的纹理，请参阅 [Wiki](https://github.com/ocornut/imgui/wiki) 上的 [Image Loading and Displaying Examples](https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples)。
- 实际纹理以何种方式标识取决于用户/引擎。这些标识符以不透明的 ImTextureID 值的形式被存储和传递。
- 默认情况下 ImTextureID 最多可存储 64 位。如有需要，你可以 `#define` 将其设为自定义类型/结构体。
- 从磁盘加载图像文件并将它们转换为纹理不属于 Dear ImGui 的职责范围（这是有充分理由的），但上面链接的示例可能是有用的参考。

**详情：**

1.92 在 2025 年 6 月引入了 `ImTextureRef`。
- 所有使用 `ImTextureID` 的绘制函数都已改为使用 `ImTextureRef`。
- 你可以轻松地从 `ImTextureID` 创建 `ImTextureRef`。
- **如果你将 Image 函数用于你自己加载/创建的纹理，那么你很可能只需在自己这一侧存储/操作 `ImTextureID`，然后将其作为 `ImTextureRef` 传入。**
- 只有在处理由后端自身管理的纹理（目前主要是指 atlas 纹理）时，你才_需要_操作 `ImTextureRef`。
- 我们有意不提供 `ImTextureRef` -> `ImTextureID` 的隐式转换运算符，因为在渲染前将 ImTextureRef 转换为 ImTextureID 在技术上是有损的。

**ImTextureID = 后端特定的低层标识符，用于标识已上传到 GPU/图形系统中的纹理。**
```cpp
#ifndef ImTextureID
typedef ImU64 ImTextureID;  // Default: store up to 64-bits (any pointer or integer). A majority of backends are ok with that.
#endif
```
- 当渲染后端创建纹理时，它会将其原生标识符存入一个 ImTextureID 值中（例如：DX11 后端使用 `ID3D11ShaderResourceView*`；OpenGL 后端存储 `GLuint`；SDLGPU 后端存储 `SDL_GPUTextureSamplerBinding*`，等等）。
- 用户可以通过传递该值，将自己的纹理提交给 `ImGui::Image()` 等函数。
- 在渲染循环期间，渲染后端会取出 ImTextureID，它存放在 ImTextureRef 中，而 ImTextureRef 又存放在 ImDrawCmd 中。
- 编译期类型配置：
  - 若要使用 64 位值以外的类型：在你的 imconfig.h 文件中添加 '#define ImTextureID MyTextureType*'。
  - 你想让它是什么就可以是什么！详情请阅读关于纹理的 FAQ 条目。
  - 如果你愿意，也可以决定存储一个包含 texture、sampler、shader 等的高层结构体，并配以各种构造函数。你需要实现 ==/!= 运算符。

**ImTextureRef = 用于标识纹理的高层标识符。**
```cpp
// Store a ImTextureID _or_ a ImTextureData*.
struct ImTextureRef
{
    ImTextureRef()                          { _TexData = nullptr; _TexID = ImTextureID_Invalid; }
    ImTextureRef(ImTextureID tex_id)        { _TexData = nullptr; _TexID = tex_id; }
    inline ImTextureID  GetTexID() const    { return _TexData ? _TexData->TexID : _TexID; }

    // Members (either are set, never both!)
    ImTextureData*      _TexData;           //      A texture, generally owned by a ImFontAtlas. Will convert to ImTextureID during render loop, after texture has been uploaded.
    ImTextureID         _TexID;             // _OR_ Low-level backend texture identifier, if already uploaded or created by user/app. Generally provided to e.g. ImGui::Image() calls.
};
```
- 即使在纹理上传到 GPU/图形系统之前，该标识符也是有效的。
- 这就是传递给 `ImGui::Image()`、`ImDrawList::AddImage()` 等函数的内容。
- 这就是存储在绘制命令（`ImDrawCmd`）中、用于在渲染期间标识纹理的内容。
 - 当纹理由用户代码创建（例如自定义图像）时，我们直接存储低层的 `ImTextureID`。
   - 因此，在显示你自己的纹理时，你很可能只需在自己这一侧管理 ImTextureID 值。
 - 当纹理由后端创建时，我们存储一个 `ImTextureData*`，它充当一个间接层，用于在纹理上传完成后、渲染期间提取 `ImTextureID` 值。
 - 若要从 `ImTextureData*` 创建 `ImTextureRef`，你可以使用 `ImTextureData::GetTexRef()`。
   我们有意不为此提供 `ImTextureRef` 构造函数：我们认为它对最终用户不会经常有用，而且许多遗留代码会错误地调用它。
 - 我们未提供从 `ImTextureData*` 创建 `ImTextureRef` 的构造函数，因为我们认为它对最终用户没有用，而且许多遗留代码会错误地调用它。
 - 如果你想在使用自定义矩形时绑定当前 atlas，可以使用 `io.Fonts->TexRef`。
 - 针对 C 等语言（没有构造函数）的绑定生成器应提供一个辅助函数，例如 `inline ImTextureRef ImTextureRefFromID(ImTextureID tex_id) { ImTextureRef tex_ref = { ._TexData = nullptr, .TexID = tex_id }; return tex_ref; }`

**在继续之前，请先阅读你所使用图形 API 的文档或教程，以了解如何将纹理显示到屏幕上。**

详细解释：
- Dear ImGui 的职责是创建“网格（mesh）”，它们以与渲染器无关的格式定义，由绘制命令和顶点组成。在帧末尾，这些网格（ImDrawList）将由你的渲染函数显示出来。它们由带纹理的多边形组成，而渲染它们的代码通常相当简短（几十行）。在 examples/ 文件夹中，我们为流行的图形 API（OpenGL、DirectX 等）提供了相应函数。
- 每个渲染函数都会决定用某种数据类型来表示“纹理”。“纹理”的概念完全取决于你底层的引擎/图形 API。
我们用 ImTextureID 类型承载标识“纹理”的信息，而 ImTextureID 本身通常又存放在 ImTextureRef 中。
ImTextureID 默认为 ImU64，也就是 8 字节的数据：刚好足以存储你选择的任意一个指针或整数。
Dear ImGui 并不知道、也不理解你在 ImTextureID 中存储了什么，它只是传递这些值，直到它们到达你的渲染函数。
- 在 [examples/](https://github.com/ocornut/imgui/tree/master/examples) 后端中，我们为每种图形 API 选定了一个类型，它很可能能很好地表示从最终用户视角指定一张图像的方式。_examples_ 中的渲染函数使用的正是这些类型：
```cpp
OpenGL:
- ImTextureID should contains 'GLuint' (GL texture identifier).
- See ImGui_ImplOpenGL3_RenderDrawData() function in imgui_impl_opengl3.cpp
```
```cpp
DirectX9:
- ImTextureID should contain a 'LPDIRECT3DTEXTURE9' (pointer).
- See ImGui_ImplDX9_RenderDrawData() function in imgui_impl_dx9.cpp
```
```cpp
DirectX11:
- ImTextureID should contain a 'ID3D11ShaderResourceView*' (pointer)
- See ImGui_ImplDX11_RenderDrawData() function in imgui_impl_dx11.cpp
```
```cpp
DirectX12:
- ImTextureID should contain a 'D3D12_GPU_DESCRIPTOR_HANDLE' (always 64-bits)
- See ImGui_ImplDX12_RenderDrawData() function in imgui_impl_dx12.cpp
```
例如，在 OpenGL 示例后端中，我们在 ImTextureID 里存储原始的 OpenGL 纹理标识符（GLuint）。
而在 DirectX11 示例后端中，我们在 ImTextureID 里存储指向 ID3D11ShaderResourceView 的指针，它是一个更高层的结构体，把纹理本身以及关于其格式和读取方式的信息绑定在一起。

- 如果你有一个基于 OpenGL 等构建的自定义引擎，除了到处传递 GLuint，你也可以决定使用一种高层数据类型，同时携带纹理信息以及如何显示它（shader 等）的信息。只要了解你的代码库是如何设计的，就能更好地决定该用什么作为 ImTextureID。如果你的引擎有用于“纹理”和“材质”的高层数据类型，那你可能就想用它们。
如果你是刚开始接触 OpenGL、DirectX 或 Vulkan，还没有在其上构建多少渲染引擎，那么保留示例后端建议的默认 ImTextureID 表示方式可能是最好的选择。
（高级用户也可以决定在 ImTextureID 中保留低层类型，使用 ImDrawList 回调，并把信息传递给自己的渲染器）

用户代码可以这样写：
```cpp
// Cast our texture type to ImTextureID
MyTexture* texture = g_CoffeeTableTexture;
ImGui::Image((ImTextureID)(intptr_t)texture, ImVec2(texture->Width, texture->Height));
```
在 ImGui::Render() 之后被调用的渲染函数将收到用户代码所传入的同一个值：
```cpp
// Cast ImTextureID stored in the draw command as our texture type
MyTexture* texture = (MyTexture*)(intptr_t)pcmd->GetTexID();
MyEngineBindTexture2D(texture);
```
一旦你理解了这一设计，你就会明白：加载图像文件并将它们转换为可显示的纹理不属于 Dear ImGui 的职责范围。
这是有意为之的，也是一件好事，因为这意味着你的代码可以完全控制自己的数据类型以及显示它们的方式。
如果你想在屏幕上显示某个图像文件（例如 PNG 文件），请参阅你所使用图形 API 的文档和教程。

请参阅 [Wiki](https://github.com/ocornut/imgui/wiki) 上的 [Image Loading and Displaying Examples](https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples)，以查找使用 OpenGL、DirectX9 和 DirectX11 加载纹理的简化示例。

C/C++ 提示：u64 是 8 字节。你可以放心地把任意指针或整数强制转换为 ImTextureID 存进去，反之亦然。
因为两端（用户代码和渲染函数）都在你的掌控之下，所以你完全清楚 ImTextureID 中存储的是什么。
以下是一些示例：
```cpp
GLuint my_tex = XXX;
ImTextureID my_imtexid;
my_imtexid = (ImTextureID)(intptr_t)my_tex;                     // cast a GLuint into a ImTextureID (we don't take its address! we just copy the address)
my_tex = (GLuint)(intptr_t)my_imtexid;                          // cast a ImTextureID into a GLuint

ID3D11ShaderResourceView* my_dx11_srv = XXX;
ImTextureID my_imtexid;
my_imtexid = (ImTextureID)(intptr_t)my_dx11_srv;                // cast a ID3D11ShaderResourceView* into an opaque ImTextureID
my_dx11_srv = (ID3D11ShaderResourceView*)(intptr_t)_my_imtexid; // cast a ImTextureID into a ID3D11ShaderResourceView*
```
最后，你可以调用 `ImGui::ShowMetricsWindow()` 来浏览/可视化/理解 ImDrawList 是如何生成的。

##### [返回索引](#index)

---

### <a id="q-how-can-i-use-maths-operators-with-imvec2"></a>问：如何在 ImVec2 上使用数学运算符？

我们默认不在 imgui.h 中导出数学运算符，以免与你自己的数学类型和数学运算符的使用产生冲突。为方便起见，你可以使用 `#define IMGUI_DEFINE_MATH_OPERATORS` + `#include "imgui.h"` 来访问我们的基本数学运算符。

##### [返回索引](#index)

---

### <a id="q-how-can-i-use-my-own-maths-types-instead-of-imvec2imvec4"></a>问：如何用自己的数学类型替代 ImVec2/ImVec4？

你可以在 [imconfig.h](https://github.com/ocornut/imgui/blob/master/imconfig.h) 文件中用 `IM_VEC2_CLASS_EXTRA`/`IM_VEC4_CLASS_EXTRA` 宏进行设置，为你的数学类型添加隐式类型转换。
这样你就能在任何地方使用自己的类型，例如向 ImGui 函数传递 `MyVector2` 或 `glm::vec2` 而不是 `ImVec2`。

##### [返回索引](#index)

---

### <a id="q-how-can-i-interact-with-standard-c-types-such-as-stdstring-and-stdvector"></a>问：如何与标准 C++ 类型（如 std::string 和 std::vector）交互？

- 出于高度可移植性（面向多种语言、框架、编程风格、冷门或较旧平台/编译器的后端/绑定），并追求适合每个现代实时游戏引擎的兼容性与性能，Dear ImGui 不使用任何 std C++ 类型。我们使用原始类型（例如用 char* 而不是 std::string），因为它们能适应更多的使用场景。
- 若要将 ImGui::InputText() 与 std::string 或任何可调整大小的字符串类一起使用，请参见 [misc/cpp/imgui_stdlib.h](https://github.com/ocornut/imgui/blob/master/misc/cpp/imgui_stdlib.h)。
- 若要将组合框和列表框与 `std::vector` 或任何其他数据结构一起使用：`BeginCombo()/EndCombo()` API
允许你自己遍历并提交项目，`ListBoxHeader()/ListBoxFooter()` API 也是如此。
请优先使用它们，而不是老旧而笨拙的 `Combo()/ListBox()` api。
- 一般来说，对于大多数高层类型，你应该能够访问其底层数据类型。
你可以编写自己的一行式包装函数来方便用户代码（提示：在你自己的代码中向 ImGui:: 命名空间添加新函数）。
- Dear ImGui 应用经常需要密集使用字符串。你传给 API 的许多字符串预计都是原始字面量（在 C/C++ 中无需额外开销），或者以不会给你的应用带来很大开销的方式分配。
请记住，在 UI 数量很大的应用中使用 `std::string` 可能会导致性能不尽如人意。
`std::string` 的现代实现通常包含小字符串优化（往往是一个本地缓冲区），但这些
是不可配置的，且不同实现之间并不相同。
- 如果你发现 UI 遍历开销过大，请确保你的字符串使用不会导致过多的
堆分配。考虑使用字面量、静态大小的缓冲区以及你自己的辅助函数。一种常见的模式
是，你需要即时构建大量字符串，而它们的最大长度可以很容易地提前界定。
一个便于以 printf 风格构建字符串的辅助函数的可能实现：https://github.com/ocornut/Str
这是一个小型辅助工具，可以用可配置的本地缓冲区长度来实例化字符串。许多游戏引擎都会
提供类似或更好的字符串辅助工具。

使用 string view：

- 诸如 `std::string_view` 之类的 string view 类型可用于 [feature/string_view](https://github.com/ocornut/imgui/tree/features/string_view) 分支。该分支正在积极维护和支持。如果你使用了它，请提供反馈。我们正在等待合适的时机将其合并。

##### [返回索引](#index)

---

### <a id="q-how-can-i-display-custom-shapes-using-low-level-imdrawlist-api"></a>问：如何显示自定义形状？（使用低层 ImDrawList API）

- 你可以使用低层 `ImDrawList` api 在窗口内渲染形状。
```cpp
ImGui::Begin("My shapes");

ImDrawList* draw_list = ImGui::GetWindowDrawList();

// Get the current ImGui cursor position
ImVec2 p = ImGui::GetCursorScreenPos();

// Draw a red circle
draw_list->AddCircleFilled(ImVec2(p.x + 50, p.y + 50), 30.0f, IM_COL32(255, 0, 0, 255));

// Draw a 3 pixel thick yellow line
draw_list->AddLine(ImVec2(p.x, p.y), ImVec2(p.x + 100.0f, p.y + 100.0f), IM_COL32(255, 255, 0, 255), 3.0f);

// Advance the ImGui cursor to claim space in the window (otherwise the window will appear small and needs to be resized)
ImGui::Dummy(ImVec2(200, 200));

ImGui::End();
```
![ImDrawList usage](https://raw.githubusercontent.com/wiki/ocornut/imgui/tutorials/CustomRendering01.png)

- 请参考演示窗口中的 "Demo > Examples > Custom Rendering"，并阅读 `imgui_demo.cpp` 中 `ShowExampleAppCustomRendering()` 的代码，以获取更多示例。
- 生成颜色：你可以使用宏 `IM_COL32(255,255,255,255)` 在编译期生成颜色，或者使用 `ImGui::GetColorU32(IM_COL32(255,255,255,255))` 或 `ImGui::GetColorU32(ImVec4(1.0f,1.0f,1.0f,1.0f))` 生成一个会乘以 `style.Alpha` 当前值的颜色。
- 数学运算符：如果你已在 `imconfig.h` 中设置 `IM_VEC2_CLASS_EXTRA` 来绑定你自己的数学类型，那么你可以使用自己的数学类型及其自然运算符，而不是 ImVec2。ImVec2 默认不在公共 API 中导出任何数学运算符。你可以使用 `#define IMGUI_DEFINE_MATH_OPERATORS` `#include "imgui.h"` 来使用我们的数学运算符，但更好的做法是使用你自己的数学库，并在 `imconfig.h` 中设置好。
- 你可以使用 `ImGui::GetBackgroundDrawList()` 或 `ImGui::GetForegroundDrawList()` 访问将显示在每个其他 Dear ImGui 窗口之下和之上的绘制列表（每个 viewport 一个 bg/fg drawlist）。如果你需要快速在屏幕上显示一些与 Dear ImGui 窗口无关的内容，这会非常方便。
- 你也可以创建自己的空窗口并在其中绘制。用 NoBackground | NoDecoration | NoSavedSettings | NoInputs 标志调用 Begin()（`ImGuiWindowFlags_NoDecoration` 标志本身就是 NoTitleBar | NoResize | NoScrollbar | NoCollapse 的快捷方式）。然后你可以通过 `GetWindowDrawList()` 获取 ImDrawList*，并以任意方式向其中绘制。
- 你可以创建自己的 ImDrawList 实例。你需要用 `ImGui::GetDrawListSharedData()` 初始化它们，或者创建你自己的 `ImDrawListSharedData` 实例，然后用你自己的 ImDrawList 或 ImDrawData 数据调用你的渲染函数。
- 想找点乐子？[ImDrawList coding party 2020](https://github.com/ocornut/imgui/issues/3606) 这个话题帖子里充满了 ImDrawList API 的“请勿在家模仿”式极端用法。

##### [返回索引](#index)

---

### <a id="about-multi-threading"></a>关于多线程

同一个 Dear ImGui context 不可被多个线程并行使用。

如果你想在并行任务中使用同一个 context 以进行偶尔的调试，可以考虑使用锁。

如果你想从主/更新线程提交内容，但在专用的渲染线程中渲染 Dear ImGui 输出，你需要暂存 `ImDrawData` 和纹理请求。参见 [imgui_threaded_rendering](https://github.com/ocornut/imgui_club#imgui_threaded_rendering) 中的 `ImDrawDataSnapshot` 和 `ImTextureQueue` 辅助工具。另见 [label: multi-threading](https://github.com/ocornut/imgui/issues?q=label%3Amulti-threading) 相关话题。

如果你使用多个 Dear ImGui context 并想从多个线程中使用它们，你需要 `#define GImGui` 使其成为 TLS 变量（详见 `GImGui` 定义附近的内容）。如果你需要同时显示多个 context，可以考虑使用 [imgui_multicontext_compositor](https://github.com/ocornut/imgui_club#imgui_multicontext_compositor)。另见 [label: multi-contexts](https://github.com/ocornut/imgui/issues?q=label%3Amulti-contexts) 相关话题。

有一个脚本/补丁可以让 API 接受一个显式的 context 指针：[#5856](https://github.com/ocornut/imgui/pull/5856)。它目前无人维护，但可能很容易更新。已决定目前不值得将该改动应用到主线，但也许会在未来版本中应用。

---
# <a id="qa-fonts-text"></a>问答：字体、文本

### <a id="q-how-should-i-handle-dpi-in-my-application"></a>问：我的应用程序中应该如何处理 DPI？

自 1.92 起（2025 年 6 月），字体可以在任意尺寸下动态使用。

**缩放字体**

选择默认尺寸：
```cpp
style.FontSizeBase = 20.0f;
```
缩放所有字体：
```cpp
style.FontScaleDpi = 2.0f;
```

如需更改字号：
```cpp
ImGui::PushFont(nullptr, 42.0f); // This will be multiplied by style.FontScaleDpi
```
如需同时更改字体和字号：
```cpp
ImGui::PushFont(new_font, 42.0f);
```

在 `docking` 分支中，或使用多视口（multi-viewports）时：
```cpp
io.ConfigDpiScaleFonts = true;          // (Docking branch only) Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
io.ConfigDpiScaleViewports = true;      // (Docking branch only) Scale Dear ImGui and Platform Windows when Monitor DPI changes.
```

**缩放样式**（内边距、间距、粗细）

这部分仍处于大规模开发中，预计会有较大变动。
目前样式数值还无法方便地动态缩放。
对于单视口应用程序，可以调用一次：
```cpp
style.ScaleAllSizes(factor); // call once!
```
如果你需要更改缩放系数，目前最实际的做法是重置样式，然后用新的数值再次调用它。

你的 UI 代码应避免对尺寸和定位使用硬编码常量。最好把数值表示为参考值的倍数，例如 `ImGui::GetFontSize()` 或 `ImGui::GetFrameHeight()`。因此，比如对于某个控件/窗口，与其写死高度为 500，不如改用 `30*ImGui::GetFontSize()`。

后续 Dear ImGui 将提供各种标准化的参考值，以便于采用这种做法。这预计会在之后的 1.92.x 版本中实现。

`examples/` 文件夹中的应用程序部分支持 DPI，但它们无法从文件系统加载自定义字体，因此看起来比较粗糙（将来可能会改变这一点）。

自带示例之所以没能自动、神奇地解决 DPI 问题，是因为针对 `ImGuiStyle` 结构体，我们还没有令人满意的方案来解决“multi-dpi”问题（在 `docking` 分支中：当多个视口窗口分布在采用不同 DPI 缩放比例的多个显示器上时）。不过字体现在已可以完美缩放。

**在 Windows 上，你需要告知 Windows 你的应用程序是 DPI 感知的！**
如果不这样做，Windows 会缩放应用程序窗口，UI 文字会变得模糊。在 Windows 上表明 DPI 感知的可行方案包括：
- SDL2：需要把 `SDL_WINDOW_ALLOW_HIGHDPI` 标志传给 `SDL_CreateWindow()`，并调用 `::SetProcessDPIAware()`。
- SDL3：需要把 `SDL_WINDOW_HIGH_PIXEL_DENSITY` 标志传给 `SDL_CreateWindow()`。
- GLFW：会自动完成。
- 对于使用其他后端的其他 Windows 项目，或封装项目（wrapper projects）：
  - 我们在 Win32 后端中提供了 `ImGui_ImplWin32_EnableDpiAwareness()` 辅助方法。
  - 使用[应用程序清单文件](https://learn.microsoft.com/en-us/windows/win32/hidpi/setting-the-default-dpi-awareness-for-a-process)来设置 `<dpiAware>` 属性。

### <a id="q-how-can-i-load-a-different-font-than-the-default"></a>问：如何加载一个不同于默认字体的字体？
使用字体图集（font atlas）加载你想要的 TTF/OTF 文件：

```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels);
io.Fonts->GetTexDataAsRGBA32() or GetTexDataAsAlpha8()
```

默认字体是 ProggyClean.ttf，等宽字体，以 13 号尺寸渲染，内嵌在 dear imgui 的源代码中。

（提示：等宽字体很方便，因为它们便于直接在字符串层面实现水平对齐。）

（有关字体加载的更多细节，请阅读 [docs/FONTS.md](https://github.com/ocornut/imgui/blob/master/docs/FONTS.md) 文件。）

新手程序员请注意：在 C/C++ 以及大多数编程语言中，如果你想在字符串字面量里使用
反斜杠 \，需要把它写成双反斜杠 "\\"：

```cpp
io.Fonts->AddFontFromFileTTF("MyFolder\MyFont.ttf", size);  // WRONG (you are escaping the M here!)
io.Fonts->AddFontFromFileTTF("MyFolder\\MyFont.ttf", size); // CORRECT (Windows only)
io.Fonts->AddFontFromFileTTF("MyFolder/MyFont.ttf", size);  // ALSO CORRECT
```

##### [返回索引](#index)

---

### <a id="q-how-can-i-easily-use-icons-in-my-application"></a>问：如何在我的应用程序中方便地使用图标？
最方便、最实用的做法是把 FontAwesome 之类的图标字体合并进你的
主字体中。这样你就可以在字符串里引用图标了。
有关图标字体加载的更多细节，请阅读 [docs/FONTS.md](https://github.com/ocornut/imgui/blob/master/docs/FONTS.md) 文件。

##### [返回索引](#index)

---

### <a id="q-how-can-i-load-multiple-fonts"></a>问：如何加载多个字体？

使用字体图集把它们打包进单个纹理中。更多细节请阅读 [docs/FONTS.md](https://github.com/ocornut/imgui/blob/master/docs/FONTS.md)。

##### [返回索引](#index)

---

### <a id="q-how-can-i-display-and-input-non-latin-characters-such-as-chinese-japanese-korean-cyrillic"></a>问：如何显示和输入非拉丁字符，例如中文、日文、韩文、西里尔文？

自 1.92 起（2025 年 6 月），配合更新后的后端，已经完全没有必要指定字形范围（glyph ranges）。

在 1.92 之前，加载字体时需要传入自定义的 Unicode 范围来指定要加载的字形。
```cpp
// [BEFORE 1.92]
// Add default Japanese ranges
io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels, nullptr, io.Fonts->GetGlyphRangesJapanese());

// Or create your own custom ranges (e.g. for a game you can feed your entire game script and only build the characters the game need)
ImVector<ImWchar> ranges;
ImFontGlyphRangesBuilder builder;
builder.AddText("Hello world");                        // Add a string (here "Hello world" contains 7 unique characters)
builder.AddChar(0x7262);                               // Add a specific character
builder.AddRanges(io.Fonts->GetGlyphRangesJapanese()); // Add one of the default ranges
builder.BuildRanges(&ranges);                          // Build the final result (ordered ranges with all the unique characters submitted)
io.Fonts->AddFontFromFileTTF("myfontfile.ttf", 16.0f, nullptr, ranges.Data);
```

你所有的字符串都需要使用 UTF-8 编码。
你需要让编译器使用 UTF-8，或者在 C++11 中可以通过 u8"hello" 语法把字符串字面量编码为 UTF-8。
在源代码中使用本地代码页（例如日文的 CP-923 或西里尔文的 CP-1251）指定字面量是不行的！
有关 UTF-8 编码的细节，请参阅
[FONTS.md](https://github.com/ocornut/imgui/blob/master/docs/FONTS.md) 中的 [About UTF-8 Encoding](https://github.com/ocornut/imgui/blob/master/docs/FONTS.md#about-utf-8-encoding) 一节。

文本输入：需要由你的应用程序通过调用 `io.AddInputCharacter()` 来传入正确的字符编码。
examples/ 中的应用程序就是这样做的。
Windows：你可以使用 WM_CHAR 或 WM_UNICHAR 或 WM_IME_CHAR 消息（取决于你的应用是以 Unicode 还是 MultiByte 模式构建的）。
你也可以使用 `MultiByteToWideChar()` 或 `ToUnicode()` 从 MultiByte 字符或键盘状态中取出 Unicode 码点。
Windows：如果你的语言依赖输入法编辑器（IME），可以把你的 HWND 写入 `ImGui::GetMainViewport()->PlatformHandleRaw`，
这样 `GetPlatformIO().Platform_SetImeDataFn()` 的默认实现就能正确设置你的 Microsoft IME 位置。

##### [返回索引](#index)

---

# <a id="qa-concerns"></a>问答：顾虑

### <a id="q-who-uses-dear-imgui"></a>问：有哪些人在使用 Dear ImGui？

你可以看看以下内容：

- [使用者评价](https://github.com/ocornut/imgui/wiki/Quotes)
- [使用 Dear ImGui 的软件](https://github.com/ocornut/imgui/wiki/Software-using-dear-imgui)
- [资助与赞助者](https://github.com/ocornut/imgui/wiki/Funding)
- [作品展示](https://github.com/ocornut/imgui/issues?q=label%3Agallery)

##### [返回索引](#index)

---

### <a id="q-can-you-create-elaborateserious-tools-with-dear-imgui"></a>问：可以用 Dear ImGui 创建精巧/正经的工具吗？

可以。人们已经用这个库编写了游戏编辑器、数据浏览器、调试器、性能分析器以及各种非平凡的工具。以我的经验来看，API 的简洁性非常赋能。你的 UI 运行在贴近实时数据的地方。让这些工具始终保持运行，团队里的每个人都会倾向于创建新工具（相比之下，那些更“离线”的 UI 工具包中，只有一小部分团队成员会真正去创建工具）。下面列出的赞助者也表明，正经的游戏团队一直在使用这个库。

Dear ImGui 非常以程序员为中心，即时模式 GUI（immediate-mode GUI）范式可能需要你先重新调整一些习惯，才能充分发挥它的潜力。Dear ImGui 追求的是让东西简单、高效且强大。

Dear ImGui 在构建时就考虑了高效与可扩展，以满足全天运行的 AAA 级应用的需求。与更典型的 RMGUI 范式相比，IMGUI 范式提供了不同的优化机会。

##### [返回索引](#index)

---

### <a id="q-can-you-reskin-the-look-of-dear-imgui"></a>问：可以给 Dear ImGui 换外观（reskin）吗？

某种程度上可以。你可以在一定程度上改变界面的外观：更改颜色、尺寸、内边距、圆角和字体。不过，由于 Dear ImGui 是为创建调试工具而设计和优化的，你能施加的换肤程度是有限的。你偏离界面默认外观和感觉的空间就那么大。Dear ImGui 并非为创建游戏用户界面而设计，尽管巧妙地使用底层 API 也能做到。

一个经过合理换肤的应用程序可能看起来像这样（截图来自 [#2529](https://github.com/ocornut/imgui/issues/2529#issuecomment-524281119)）：
![minipars](https://user-images.githubusercontent.com/314805/63589441-d9794f00-c5b1-11e9-8d96-cfc1b93702f7.png)

##### [返回索引](#index)

---

### <a id="q-why-using-c-as-opposed-to-c"></a>问：为什么使用 C++（而不是 C）？

Dear ImGui 为了便利而利用了少量 C++ 语言特性，但完全没有 Boost 那种疯狂/泥潭式的东西。Dear ImGui 不使用任何 C++ 头文件。Dear ImGui 只使用极小一部分 C++11 特性。特别是，利用函数重载和默认参数让 API 更易用、代码更简洁。我认为这样做让 API 处在一个恰到好处的位置，而放弃这些特性会让 API 更加笨重。命名空间、构造函数以及模板（就 ImVector<> 类而言）等其他特性也被当作便利手段加以依赖。

Sonoro1234 和 Stephan Dilly 提供了一个自动生成的 [Dear ImGui 的 C API（cimgui）](https://github.com/cimgui/cimgui)。它旨在用于为其他语言创建绑定。如果可能的话，我建议利用目标语言的功能来尝试复刻 C++ 中使用的函数重载和默认参数，否则 API 可能会更难使用。另请参阅 [Bindings](https://github.com/ocornut/imgui/wiki/Bindings) 了解各种第三方绑定。

##### [返回索引](#index)

---

# <a id="qa-community"></a>问答：社区

### <a id="q-how-can-i-help"></a>问：我怎样才能提供帮助？
- 企业：如果你所在的公司在使用 Dear ImGui，请联系 `omar AT dearimgui.com`！我们可以讨论贵公司通过开具发票的技术支持、维护或赞助合作等方式为开发提供资金。这是你能为 Dear ImGui 做的最有用的事情之一。有了更多资金，我们就能聘请更多人来参与这个项目。请参见 [资助](https://github.com/ocornut/imgui/wiki/Funding) 页面。
- 个人：你可以通过 PayPal 捐款来支持持续的维护与开发。请参见 [README](https://github.com/ocornut/imgui/blob/master/docs/README.md)。
- 如果你熟悉 Dear ImGui 和 C++，可以看看 [GitHub Issues](https://github.com/ocornut/imgui/issues)、[GitHub Discussions](https://github.com/ocornut/imgui/discussions)、[Wiki](https://github.com/ocornut/imgui/wiki)，阅读 [docs/TODO.txt](https://github.com/ocornut/imgui/blob/master/docs/TODO.txt)，然后看看你想以何种方式帮忙、又能帮上哪些忙！
- 通过开发博客文章、推文、截图、在某个地方提及等方式公开你正在使用 Dear ImGui。
你可以在 [gallery 帖子](https://github.com/ocornut/imgui/issues?q=label%3Agallery)中发布截图或链接。视觉内容最理想，因为它们能启发其他程序员。公开你对 Dear ImGui 的使用有助于这个库建立可信度，也能帮助其他团队和程序员做出决策。
- 如果你遇到问题，或者需要深入改造这个库，即使你不期望获得任何支持，分享你的问题、乃至有时尚不完整的 PR，也是很有用的。

##### [返回索引](#index)
