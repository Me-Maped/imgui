_（你可以在 https://github.com/ocornut/imgui/blob/master/docs/FONTS.md 浏览本文档，或用任意 Markdown 阅读器查看本文件）_

## Dear ImGui：使用字体

Dear ImGui 的代码内嵌了一份由 Tristan Grimmer 制作的 [ProggyClean.ttf](https://github.com/bluescan/proggyfonts)，
这是一款 13 像素高、像素级精确的字体，默认使用。ProggyClean 的缩放效果并不理想。

Dear ImGui 的代码内嵌了一份由 Disco Hello 与 Tristan Grimmer 制作的 [ProggyForever.ttf](https://github.com/ocornut/proggyforever) 的部分副本，
这是一款模仿 ProggyClean 的新字体，缩放效果很好。

我们将字体内嵌在代码中，这样你无需任何文件系统访问即可使用 Dear ImGui。
如果你不使用它们，可以在 [imconfig.h](https://github.com/ocornut/imgui/blob/master/imconfig.h) 文件中设置 `IMGUI_DISABLE_DEFAULT_FONT`，从而发布不含这些字体的二进制文件，并节省约 26 KB。

调用 io.Fonts->AddFontDefaultVector() 会加载 ProggyForever。
调用 io.Fonts->AddFontDefaultBitmap() 会加载 ProggyClean。
调用 io.Fonts->AddFontDefault() 会根据预期的默认字体大小选择其中之一（当 `style.FontSizeBase * style.FontScaleMain * style.FontSizeDpi >= 15` 时，我们使用 ProggyForever）。

你也可以加载外部的 .TTF/.OTF 文件，参见本页的说明。
在 [misc/fonts/](https://github.com/ocornut/imgui/tree/master/misc/fonts) 文件夹中，你可以找到一些推荐的字体，谨供取用。

**另请阅读 FAQ：** https://www.dearimgui.com/faq （其中有 Fonts 章节！）

## <a id="index"></a>目录
- [疑难解答](#troubleshooting)
- [新特性！1.92（2025 年 6 月）的动态字体系统](#new-dynamic-fonts-system-in-192-june-2025)
- [我应该在应用程序中如何处理 DPI？](#how-should-i-handle-dpi-in-my-application)
- [字体加载说明](#fonts-loading-instructions)
- [从内存加载字体数据](#loading-font-data-from-memory)
- [加载内嵌在源代码中的字体数据](#loading-font-data-embedded-in-source-code)
- [使用图标字体](#using-icon-fonts)
  - [排除重叠的范围](#excluding-overlapping-ranges)
- [使用 FreeType 光栅化器 (imgui_freetype)](#using-freetype-rasterizer-imgui_freetype)
- [使用彩色字形/表情符号](#using-colorful-glyphsemojis)
- [使用自定义字形范围](#using-custom-glyph-ranges)
- [使用自定义彩色图标](#using-custom-colorful-icons)
- [关于文件名](#about-filenames)
- [关于 UTF-8 编码](#about-utf-8-encoding)
- [调试工具](#debug-tools)
- [仓库中所含字体的鸣谢/许可证](#creditslicenses-for-fonts-included-in-repository)
- [字体链接](#font-links)

---------------------------------------

## <a id="troubleshooting"></a>疑难解答

**绝大多数与字体和文本相关的问题都源于以下 4 个方面：**

### (1) 文件名无效，原因是使用了 `\` 或工作目录不符合预期。

参见 [关于文件名](#about-filenames)。如果文件名不正确，AddFontXXX() 函数应当会触发断言。

### (2) 非 ASCII 字符串的 UTF-8 编码无效。

参见 [关于 UTF-8 编码](#about-utf-8-encoding)。使用编码查看器确认你源代码中字符串字面量的编码是否正确。

### (3) 缺少字形范围。

🆕 **自 1.92 起，配合最新的后端：不再需要指定字形范围。**

⏪ 在 1.92 之前：如果你想使用非 ASCII 字符，就需要加载带有显式字形范围的字体。参见 [字体加载说明](#fonts-loading-instructions)。使用 [调试工具](#debug-tools) 确认已加载的字体和已加载的字形范围。

这是 Dear ImGui 以前的一项限制（在 1.92 中解除）：加载字体时，你需要指定要加载哪些字符的字形。
所有已加载字体的字形都会预先渲染到一个单独的纹理图集中。调用 `io.Fonts->GetTexDataAsAlpha8()`、`io.Fonts->GetTexDataAsRGBA32()` 或 `io.Fonts->Build()` 中的任意一个都会构建图集。这通常由渲染器后端调用，例如 `ImGui_ImplDX11_NewFrame()` 会调用它。**如果你使用自定义字形范围，请确保该数组是持久的**，并在调用 `GetTexDataAsAlpha8()/GetTexDataAsRGBA32()/Build()` 期间可用。

### (4) 字体图集纹理无法上传到 GPU。

🆕 **自 1.92 起，配合最新的后端：图集会增量构建并动态调整大小，这种情况不太可能发生**

:rewind: 这通常是第 3 点的副产品。如果你有大量的字形或多种字体，纹理可能会对你的图形 API 来说变得过大。**纹理上传失败的典型表现是每个字形或所有内容都显示为空的白色矩形。** 请注意，某些图形驱动存在纹理大小限制。如果你在开发 PC 应用程序，请注意你的用户可能使用限制比你更低的硬件。

![empty squares](https://github.com/user-attachments/assets/68b50fb5-8b9d-4c38-baec-6ac384f06d26)

一些解决方案：
- 你可以降低过采样，例如 `font_config.OversampleH = 1`，这会使你的纹理尺寸减半，但会损失一些质量。
  注意，在大多数情况下 OversampleH = 2 在视觉上与 3 非常接近，但使用 OversampleH = 1 时质量下降会很明显。关于过采样，请阅读[此处](https://github.com/nothings/stb/blob/master/tests/oversample)。
- 根据源语言的本地化数据计算字形范围，从而缩减范围。
  你可以为此使用 `ImFontGlyphRangesBuilder`，并在需要新字符时于帧之间重建图集。这将带来最大的收益！
- 设置 `io.Fonts.Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;` 以禁用将纹理高度向上取整到下一个 2 的幂。

### (5) 减少启动时的纹理调整大小/复制

🆕 自 1.92 起，ImFontAtlas 初始时使用 512x128 的纹理创建，随后随着字形和字体的使用而增长。图集增长会导致分配+复制，并且新旧两种尺寸的纹理会短暂地（通常一帧）同时存在于内存中。如果你使用已知的字体并希望减少初始增长，可以在初始化期间设置 `TexMinWidth` 和 `TexMinHeight`。

```cpp
ImFontAtlas* atlas = io.Fonts;
atlas->TexMinWidth = 1024;
atlas->TexMinHeight = 1024;
atlas->AddFont(...);
```

##### [返回目录](#index)

---------------------------------------

## <a id="new-dynamic-fonts-system-in-192-june-2025"></a>新特性！1.92（2025 年 6 月）的动态字体系统

v1.92 引入了更新的动态字体系统。它要求后端支持 `ImGuiBackendFlags_HasTextures` 特性：
- 使用图标、亚洲语言和非英语语言的用户不再需要预先构建所有字形。这样可以节省加载时间、内存，并减少字形缺失的问题。不再需要指定字形范围。
- 可以随时使用 `PushFont(nullptr, new_size)` 更改字体大小。
- 打包自定义矩形更加方便，因为可以立即写入像素。
- 以前对字体的任何更新都需要调用特定于后端的函数来重新上传纹理，而这些调用无法跨后端移植。现在可以缩放字体等，而无需进行特定于后端的调用。
- 可以为任何字体源接入自定义的加载器/后端。

更多详情请参见 [#8465](https://github.com/ocornut/imgui/issues/8465)。


##### [返回目录](#index)

---------------------------------------

## <a id="how-should-i-handle-dpi-in-my-application"></a>我应该在应用程序中如何处理 DPI？

自 1.92 起，配合更新过的后端，你可以设置 `style.FontScaleDpi = your_content_scale;` 来缩放所有字体。
<BR>你可以在初始化时调用 `style.ScaleAllSizes(xxx)`，或在主循环开头每帧调用一次，以缩放尺寸/内边距。
<BR>自 1.92 起，配合更新过的后端，macOS 风格的像素/后备缩放会被自动处理。
<BR>更多详情请参见 [FAQ 条目](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-how-should-i-handle-dpi-in-my-application)。

##### [返回目录](#index)

---------------------------------------

## <a id="fonts-loading-instructions"></a>字体加载说明

**选择基础大小**
```cpp
ImGuiStyle& style = ImGui::GetStyle();
style.FontSizeBase = 20.0f;
```

**加载默认字体：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefaultVector();  // 加载内嵌的可缩放字体。
```
```cpp
io.Fonts->AddFontDefaultBitmap();  // 加载内嵌的位图字体（旧版）。
```
```cpp
io.Fonts->AddFontDefault();        // 加载内嵌字体（旧版：在上述两者之间自动选择）。
```

**加载 .TTF/.OTF 文件：**

🆕 **自 1.92 起，配合最新的后端：无需传入大小**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("font.ttf");
```
:rewind: **在 1.92 之前，或没有最新的后端时：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels);
```
如果你遇到提示 "Could not load font file!" 的断言，很可能是你的字体文件名不正确。请仔细阅读 [关于文件名](#about-filenames)。

**加载多个字体：**
```cpp
// 初始化
ImGuiIO& io = ImGui::GetIO();
ImFont* font1 = io.Fonts->AddFontFromFileTTF("font.ttf",);
ImFont* font2 = io.Fonts->AddFontFromFileTTF("anotherfont.otf");
```

在你的应用程序循环中，选择要使用的字体：
```cpp
ImGui::Text("Hello"); // 使用默认字体（即第一个加载的字体）
ImGui::PushFont(font2, 0.0f); // 更换字体，保持当前大小
ImGui::Text("Hello with another font");
ImGui::PopFont();
```

**如需高级选项，请创建 ImFontConfig 结构体并将其传给 AddFont() 函数（它会在内部被复制）：**
```cpp
ImFontConfig config;
config.OversampleH = 1.0f;
ImFont* font = io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, &config);
```

**将多个字体合并为一个：**

🆕 **自 1.92 起，配合最新的后端：不再需要指定字形范围。**
```cpp
// 加载第一个字体
ImFont* font = io.Fonts->AddFontDefaultVector();
ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 0.0f, &config);           // 合并到第一个字体中，例如添加亚洲字符
io.Fonts->AddFontFromFileTTF("fontawesome-webfont.ttf", 0.0f, &config); // 合并到第一个字体中以添加图标
```
:rewind: **在 1.92 之前，或没有最新的后端时：**
```cpp
// 加载第一个字体
ImFont* font = io.Fonts->AddFontDefault();

// 添加字符范围并合并到前一个字体中
// ranges 数组不会被 AddFont* 函数复制，而是延迟使用，
// 因此请确保在构建或调用 GetTexDataAsRGBA32() 时它仍然可用。
static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 }; // 不会被 AddFont* 复制，因此请保持其作用域。
ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese()); // 合并到第一个字体中
io.Fonts->AddFontFromFileTTF("fontawesome-webfont.ttf", 18.0f, &config, icons_ranges);             // 合并到第一个字体中
io.Fonts->Build();
```

**添加第四个参数以仅烘焙特定的字体范围：**

🆕 **自 1.92 起，配合最新的后端：不再需要指定字形范围。所有 GetGlyphRangesXXX() 函数都已标记为过时。**
```cpp
// 基本拉丁文、扩展拉丁文
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesDefault());

// 默认范围 + 简体中文使用的 2500 个表意文字选集
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

// 默认范围 + 平假名、片假名、半角字符、1946 个表意文字选集
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesJapanese());
```
有关创建你自己的范围，请参见 [使用自定义字形范围](#using-custom-glyph-ranges) 一节。

**加载并使用日文字体的示例：**

🆕 **自 1.92 起，配合最新的后端：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf");
```

:rewind: **在 1.92 之前，或没有最新的后端时：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
```

```cpp
ImGui::Text(u8"こんにちは！テスト %d", 123);
if (ImGui::Button(u8"ロード"))
{
    // 执行一些操作
}
ImGui::InputText("string", buf, IM_COUNTOF(buf));
ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
```

![sample code output](https://raw.githubusercontent.com/wiki/ocornut/imgui/web/v160/code_sample_02_jp.png)
<br>_（设置：Dark 风格（左）、Light 风格（右）/ 字体：NotoSansCJKjp-Medium, 20px / 圆角：5）_

##### [返回目录](#index)

---------------------------------------

## <a id="loading-font-data-from-memory"></a>从内存加载字体数据

```cpp
ImFont* font = io.Fonts->AddFontFromMemoryTTF(data, data_size, size_pixels, ...);
```

重要：`AddFontFromMemoryTTF()` 默认会把数据缓冲区的所有权转移给字体图集，图集将在析构时尝试释放它。
这样做是为了避免不必要的复制，但或许并不是一个好的 API（未来的版本会重新设计它）。
如果你想保留数据的所有权并自行释放它，你需要清除 `FontDataOwnedByAtlas` 字段：

```cpp
ImFontConfig font_cfg;
font_cfg.FontDataOwnedByAtlas = false;
ImFont* font = io.Fonts->AddFontFromMemoryTTF(data, data_size, size_pixels, &font_cfg);
```
重要：自 1.92 起，使用 `FontDataOwnedByAtlas = false` 时，字体数据需要一直可用，直到 `atlas->RemoveFont()`，或者更常见的是直到拥有它的上下文或字体图集关闭。由于 1.92.0 中处理 `FontDataOwnedByAtlas = false` 的一个 bug，这个问题当时并未立刻显现，该 bug 已在 1.92.6 中修复。

##### [返回目录](#index)

---------------------------------------

## <a id="loading-font-data-embedded-in-source-code"></a>加载内嵌在源代码中的字体数据

- 编译并使用 [binary_to_compressed_c.cpp](https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp)，生成可内嵌到源代码中的压缩 C 风格数组。
- 有关如何使用该工具的说明，请参见 [binary_to_compressed_c.cpp](https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp) 中的文档。
- 你可以在演示程序的二进制包中找到适用于 Windows 的预编译版本 binary_to_compressed_c.exe（参见 [README](https://github.com/ocornut/imgui/blob/master/docs/README.md)）。
- 该工具可以选择输出 Base85 编码以减小 _源代码_ 的大小，但实际二进制文件中的只读数组会大约增大 20%。

然后使用以下方式加载字体：
```cpp
ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(compressed_data, compressed_data_size, size_pixels, ...);
```
或
```cpp
ImFont* font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(compressed_data_base85, size_pixels, ...);
```

##### [返回目录](#index)

---------------------------------------

## <a id="using-icon-fonts"></a>使用图标字体

使用图标字体（例如 [FontAwesome](http://fontawesome.io) 或 [OpenFontIcons](https://github.com/traverseda/OpenFontIcons)）是在你的 Dear ImGui 应用程序中使用图标的一种简单而实用的方式。
一种常见做法是把图标字体合并到你的主字体中，这样你就可以在字符串中直接嵌入图标，而无需反复来回切换字体。

要在你的 C++ 代码中引用图标的 UTF-8 码点，你可以使用由 Juliette Foucaut 创建的那些头文件：https://github.com/juliettef/IconFontCppHeaders 。

这样你就可以使用 `ICON_FA_SEARCH` 作为字符串，它会渲染为一个 "Search" 图标。

🆕 **自 1.92 起，配合最新的后端：不再需要指定字形范围。你可以省略该参数。**
示例设置：
```cpp
// 将图标合并到默认的工具字体中
#include "IconsFontAwesome.h"
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefaultVector();
ImFontConfig config;
config.MergeMode = true;
config.GlyphMinAdvanceX = 13.0f; // 如果你想让图标等宽，可以使用这一项
io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &config);
```
:rewind: **在 1.92 之前：**
```cpp
// 将图标合并到默认的工具字体中
#include "IconsFontAwesome.h"
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();

ImFontConfig config;
config.MergeMode = true;
config.GlyphMinAdvanceX = 13.0f; // 如果你想让图标等宽，可以使用这一项
static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &config, icon_ranges);
```
示例用法：
```cpp
// 用法，例如
ImGui::Text("%s among %d items", ICON_FA_SEARCH, count);
ImGui::Button(ICON_FA_SEARCH " Search");
// C 字符串_字面量_可以在编译期拼接，例如 "hello" " world"
// ICON_FA_SEARCH 被定义为字符串字面量，所以这等同于 "A" "B" 变成 "AB"
```
有关其他图标字体和相关工具，请参见下方的链接。

**等宽图标？**

为了让你的图标看起来更等宽并便于对齐，你可能希望在加载图标字体时设置 `ImFontConfig::GlyphMinAdvanceX` 的值。
如果你使用 `GlyphMinAdvanceX`，你需要向 `AddFontXXX()` 调用传入 `font_size`，因为 MinAdvanceX 值是针对给定大小指定的，否则会按比例缩放。

**截图**

下面是一个使用了图标的应用程序（"Avoyd"，https://www.avoyd.com ） ：
![avoyd](https://user-images.githubusercontent.com/8225057/81696852-c15d9e80-9464-11ea-9cab-2a4d4fc84396.jpg)

##### [返回目录](#index)

---------------------------------------

### <a id="excluding-overlapping-ranges"></a>排除重叠的范围

🆕 **自 1.92 起，配合最新的后端：字形范围会被忽略**：加载某个字形时，会按顺序查询合并列表中的输入字体。第一个拥有该字形的字体负责加载它。
<BR>‼️ **如果你合并了多个字体，可能会出现不希望的重叠范围。** 你可以使用 `ImFontConfig::GlyphExcludeRanges[] ` 来指定在某个输入中要忽略的范围。

```cpp
// 添加字体源 1，但忽略 ICON_MIN_FA..ICON_MAX_FA 范围
static ImWchar exclude_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
ImFontConfig cfg1;
cfg1.GlyphExcludeRanges = exclude_ranges;
io.Fonts->AddFontFromFileTTF("segoeui.ttf", 0.0f, &cfg1);

// 添加字体源 2，它期望使用上面的范围
ImFontConfig cfg2;
cfg2.MergeMode = true;
io.Fonts->AddFontFromFileTTF("FontAwesome4.ttf", 0.0f, &cfg2);
```
另一个（有点无聊的）例子：
```cpp
// 从第一个字体中移除 'A'-'Z'
static ImWchar exclude_ranges[] = { 'A', 'Z', 0 };
ImFontConfig cfg1;
cfg1.GlyphExcludeRanges = exclude_ranges;
io.Fonts->AddFontFromFileTTF("segoeui.ttf", 0.0f, &cfg1);

// 加载另一个字体来填补空缺
ImFontConfig cfg2;
cfg2.MergeMode = true;
io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 0.0f, &cfg2);
```
![image](https://github.com/user-attachments/assets/f3d751d3-1aee-4698-bd9b-f511902f56bb)

你可以使用 `Metrics/Debugger->Fonts->Font->Input Glyphs Overlap Detection Tool` 查看在多个字体源中都可用的字形列表。这有助于理解哪个字体输入源提供哪个字形。

##### [返回目录](#index)

---------------------------------------

## <a id="using-freetype-rasterizer-imgui_freetype"></a>使用 FreeType 光栅化器 (imgui_freetype)

- Dear ImGui 使用 [stb_truetype.h](https://github.com/nothings/stb/) 来光栅化字体（可选过采样）。这种技术及其实现对于以小尺寸渲染的字体并不理想，可能会显得有点模糊或难以辨认。
- 不过你可以使用 [misc/freetype/](https://github.com/ocornut/imgui/tree/master/misc/freetype) 文件夹中的 `imgui_freetype.cpp`。编译该文件，并在你的 imconfig.h 文件或构建系统中添加 `#define IMGUI_ENABLE_FREETYPE` 即可自动启用它。
- FreeType 支持自动微调（auto-hinting），这通常能改善小字体的可读性。在较小的分辨率下效果尤为显著。
- 请阅读 [misc/freetype/](https://github.com/ocornut/imgui/tree/master/misc/freetype) 文件夹中的文档。
- 正确的 sRGB 空间混合会对你的字体渲染质量产生重要影响。

##### [返回目录](#index)

---------------------------------------

## <a id="using-colorful-glyphsemojis"></a>使用彩色字形/表情符号

- imgui_freetype 配合 FreeType 2.10+ 支持彩色表情符号的渲染。
- 你需要使用 `ImGuiFreeTypeBuilderFlags_LoadColor` 标志加载字体。
- 表情符号通常编码在 Unicode 的较高层（字符码 >0x10000），需要以 `IMGUI_USE_WCHAR32` 编译 dear imgui。
- 目前 FreeType 并不支持所有类型的彩色字体。
- 文本渲染器不支持肤色修饰符等有状态的 Unicode 特性。

![colored glyphs](https://user-images.githubusercontent.com/8225057/106171241-9dc4ba80-6191-11eb-8a69-ca1467b206d1.png)

```cpp
io.Fonts->AddFontFromFileTTF("../../../imgui_dev/data/fonts/NotoSans-Regular.ttf", 16.0f);
static ImWchar ranges[] = { 0x1, 0x1FFFF, 0 };
static ImFontConfig cfg;
cfg.MergeMode = true;
cfg.FontLoaderFlags |= ImGuiFreeTypeLoaderFlags_LoadColor;
io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\seguiemj.ttf", 16.0f, &cfg);
```

##### [返回目录](#index)

---------------------------------------

## <a id="using-custom-glyph-ranges"></a>使用自定义字形范围

🆕 **自 1.92 起，配合最新的后端：不再需要指定字形范围。因此这部分内容其实已不再有用。**

:rewind: 你可以使用 `ImFontGlyphRangesBuilder` 辅助工具，根据文本输入创建字形范围。例如：对于脚本已知的游戏，如果你能把整个脚本喂给它，就只构建游戏需要的字符。
```cpp
ImVector<ImWchar> ranges;
ImFontGlyphRangesBuilder builder;
builder.AddText("Hello world");                        // 添加一个字符串（这里的 "Hello world" 包含 7 个唯一字符）
builder.AddChar(0x7262);                               // 添加一个特定字符
builder.AddRanges(io.Fonts->GetGlyphRangesJapanese()); // 添加一个默认范围
builder.BuildRanges(&ranges);                          // 构建最终结果（包含所有已提交唯一字符的有序范围）

io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels, nullptr, ranges.Data);
io.Fonts->Build();                                     // 在 'ranges' 仍然在作用域内且未被删除时构建图集。
```

##### [返回目录](#index)

---------------------------------------

## <a id="using-custom-colorful-icons"></a>使用自定义彩色图标

🆕 **自 1.92 起，配合最新的后端：该系统已被重新设计。**

TL;DR; 在新系统中，建议你创建一个自定义的 `ImFontLoader` 并向其注册你的字体。
`AddCustomRectFontGlyph()` 已被废弃，因为它的 API 对于可调整大小的字体来说意义不大。

你可以在 [#8466](https://github.com/ocornut/imgui/issues/8466) 中提问。

:rewind: **在 1.92 之前：**

作为使用 imgui_freetype 配合 `ImGuiFreeTypeBuilderFlags_LoadColor` 渲染彩色字形的替代方案，你可以在纹理图集中分配你自己的空间并自行写入。**（这是一个 BETA API，如果你熟悉 dear imgui 和你的渲染后端，再使用它）**

- 你可以使用 `ImFontAtlas::AddCustomRect()` 和 `ImFontAtlas::AddCustomRectFontGlyph()` API 注册将被打包到字体图集纹理中的矩形。在构建图集之前注册它们，然后调用 `Build()`。
- 然后你可以使用 `ImFontAtlas::GetCustomRect(int)` 查询你的矩形在纹理中的位置/大小，并将你选择的任意图形数据 blit/复制到这些矩形中。
- 该 API 仍处于 beta 阶段，因为它很可能会为了支持多 DPI（多个显示器上的多个视口，且 DPI 缩放各不相同）而发生变化。

#### 伪代码：
```cpp
// 添加字体，然后注册两个自定义的 13x13 矩形，分别映射到该字体的字形 'a' 和 'b'
ImFont* font = io.Fonts->AddFontDefaultVector();
int rect_ids[2];
rect_ids[0] = io.Fonts->AddCustomRectFontGlyph(font, 'a', 13, 13, 13+1);
rect_ids[1] = io.Fonts->AddCustomRectFontGlyph(font, 'b', 13, 13, 13+1);

// 构建图集
io.Fonts->Build();

// 以 RGBA 格式获取纹理
unsigned char* tex_pixels = nullptr;
int tex_width, tex_height;
io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_width, &tex_height);

for (int rect_n = 0; rect_n < IM_COUNTOF(rect_ids); rect_n++)
    if (const ImTextureRect* rect = io.Fonts->GetCustomRect(rect_ids[rect_n]))
    {
        // 用红色像素填充自定义矩形（实际上你会在这里绘制/复制你的位图数据！）
        for (int y = 0; y < rect->Height; y++)
        {
            ImU32* p = (ImU32*)tex_pixels + (rect->Y + y) * tex_width + (rect->X);
            for (int x = rect->Width; x > 0; x--)
                *p++ = IM_COL32(255, 0, 0, 255);
        }
    }
```

##### [返回目录](#index)

---------------------------------------

## <a id="about-filenames"></a>关于文件名

**请注意，许多 C/C++ 新手在加载文件时遇到问题，_是因为他们提供的文件名是错误的_，根源在于对当前目录的错误假设。**

需要注意两点：

(1) 在 C/C++ 和大多数编程语言中，如果你想在字符串字面量中使用反斜杠 `\`，你需要写成双反斜杠 `\\`。而 Windows 恰好使用反斜杠作为路径分隔符，因此请务必留意。
```cpp
io.Fonts->AddFontFromFileTTF("MyFiles\MyImage01.jpg", ...);   // 这是错误的！！
io.Fonts->AddFontFromFileTTF("MyFiles\\MyImage01.jpg", ...);  // 这是正确的
```
在某些情况下，你在 Windows 下也可以使用 `/` 路径分隔符。

(2) 确保你的 IDE/调试器设置从正确的工作（当前）目录启动你的可执行文件。在 Visual Studio 中，你可以在项目的 `Properties > General > Debugging > Working Directory` 中更改工作目录。人们常以为程序会从项目的根文件夹开始运行，但默认情况下它往往从存放目标文件或可执行文件的文件夹开始运行。
```cpp
io.Fonts->AddFontFromFileTTF("MyImage01.jpg", ...);       // 相对文件名取决于你运行程序时的工作目录！
io.Fonts->AddFontFromFileTTF("../MyImage01.jpg", ...);    // 从工作目录的上一级文件夹加载
```
##### [返回目录](#index)

---------------------------------------

## <a id="about-utf-8-encoding"></a>关于 UTF-8 编码

**在显示非 ASCII 字符时，一个常见的用户问题是传入的字符串没有正确地以 UTF-8 编码。**

(1) 我们提供了函数 `ImGui::DebugTextEncoding(const char* text)`，你可以调用它来验证 UTF-8 字符串的内容。
这是确认你的编码是否正确的一种便捷方式。

```cpp
ImGui::SeparatorText("CORRECT");
ImGui::DebugTextEncoding(u8"こんにちは");

ImGui::SeparatorText("INCORRECT");
ImGui::DebugTextEncoding("こんにちは");
```
![UTF-8 Encoding viewer](https://github.com/ocornut/imgui/assets/8225057/61c1696a-9a94-46c5-9627-cf91211111f0)

如果你想从剪贴板粘贴，也可以在这个位置找到该工具：`Metrics/Debuggers->Tools->UTF-8 Encoding viewer`，但它不会验证由你的编译器完成的 UTF-8 编码。

(2) 以 UTF-8 编码：

也有一些特定于编译器的方法可以默认强制使用 UTF-8 编码：

- Visual Studio 编译器：`/utf-8` 命令行标志。
- Visual Studio 编译器：在你的代码中使用 `#pragma execution_character_set("utf-8")`。
- 自 2023 年 5 月起，我们已将我们所有示例的 Visual Studio 项目改为使用 `/utf-8`（[参见提交](https://github.com/ocornut/imgui/commit/513af1efc9080857bbd10000d98f98f2a0c96803)）。

或者，自 C++11 起，你可以使用 `u8"my text"` 语法将字面量字符串编码为 UTF-8。例如：
```cpp
ImGui::Text(u8"hello");
ImGui::Text(u8"こんにちは");   // 这总是会被编码为 UTF-8
ImGui::Text("こんにちは");     // 它的编码取决于编译器设置/标志，可能是错误的。
```

自 C++20 起，由于 C++ 委员会讨厌它的用户，他们决定修改 `u8""` 语法，使其不再返回 `const char*`，而是返回一个新类型 `const char8_t*`，该类型不会转换为 `const char*`。
因此在 C++20 中，`u8""` 的类型使用会稍微麻烦一些：
```cpp
ImGui::Text((const char*)u8"こんにちは");
```
不过，你可以分别使用 MSVC 的编译器选项 [`/Zc:char8_t-`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-char8-t?view=msvc-170) 以及 Clang 和 GCC 的 [`-fno-char8_t`](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html) 来完全禁用这一行为。
##### [返回目录](#index)

---------------------------------------

## <a id="debug-tools"></a>调试工具

#### Metrics/Debugger->Fonts
你可以使用 `Metrics/Debugger` 窗口（位于 `Demo>Tools` 中）浏览你的字体，并在遇到问题时了解发生了什么。你也可以在 `Demo->Tools->Style Editor->Fonts` 中找到它。相同的信息在 Style Editor 的 Fonts 下也可查看。

![Fonts debugging](https://user-images.githubusercontent.com/8225057/135429892-0e41ef8d-33c5-4991-bcf6-f997a0bcfd6b.png)

#### UTF-8 编码查看器**
你可以使用 `Metrics/Debugger` 中的 `UTF-8 Encoding viewer` 验证 UTF-8 字符串的内容。在 C/C++ 代码中，你可以调用 `ImGui::DebugTextEncoding("my string");` 函数来验证你的 UTF-8 编码是否正确。

![UTF-8 Encoding viewer](https://user-images.githubusercontent.com/8225057/166505963-8a0d7899-8ee8-4558-abb2-1ae523dc02f9.png)

##### [返回目录](#index)

---------------------------------------

## <a id="creditslicenses-for-fonts-included-in-repository"></a>仓库中所含字体的鸣谢/许可证

内嵌在源代码中：

**ProggyClean.ttf**，作者 Tristan Grimmer
<br>MIT License
<br>（推荐的加载设置：Size = 13.0, GlyphOffset.y = +1, PixelSnapH = true）
<br>https://github.com/bluescan/proggyfonts

**ProggyForever.ttf**，作者 Disco Hello、Tristan Grimmer
<BR>MIT License
<BR>https://github.com/ocornut/proggyforever

`misc/fonts/` 文件夹中还提供了额外的字体文件。
与 2014 年首次引入它们时相比，我们现在有了更好的字体支持，并且内嵌了 ProggyForever。
我认为如今这里的文件都已不再必要。你可以自己寻找字体。它们最终可能会被移除。

**Roboto-Medium.ttf**，作者 Christian Robetson
<br>Apache License 2.0
<br>https://fonts.google.com/specimen/Roboto

**Cousine-Regular.ttf**，作者 Steve Matteson
<br>数字化数据版权 (c) 2010 Google Corporation。
<br>依据 SIL Open Font License 1.1 版授权
<br>https://fonts.google.com/specimen/Cousine

**DroidSans.ttf**，作者 Steve Matteson
<br>Apache License 2.0
<br>https://www.fontsquirrel.com/fonts/droid-sans

**ProggyTiny.ttf**，作者 Tristan Grimmer
<br>MIT License
<br>（推荐的加载设置：Size = 10.0, GlyphOffset.y = +1）
<br>https://github.com/bluescan/proggyfonts

**Karla-Regular.ttf**，作者 Jonathan Pinhorn
<br>SIL OPEN FONT LICENSE Version 1.1

##### [返回目录](#index)

## <a id="font-links"></a>字体链接

#### 图标字体

- 图标字体的 C/C++ 头文件（用 #define 定义码点，以便在源代码字符串字面量中使用）https://github.com/juliettef/IconFontCppHeaders
- FontAwesome https://fortawesome.github.io/Font-Awesome
- OpenFontIcons https://github.com/traverseda/OpenFontIcons
- Google Icon Fonts https://design.google.com/icons/
- Kenney Icon Font（游戏手柄图标）https://github.com/nicodinh/kenney-icon-font
- IcoMoon - 自定义图标字体构建器 https://icomoon.io/app

#### 常规字体

- Google Noto Fonts（面向世界各地的语言）https://www.google.com/get/noto/
- Open Sans Fonts https://fonts.google.com/specimen/Open+Sans
- （日文）由 Coji Morishita 制作的 M+ 字体 http://mplus-fonts.sourceforge.jp/mplus-outline-fonts/index-en.html

#### 等宽字体

<img width="1172" height="715" alt="image" src="https://github.com/user-attachments/assets/c9702534-4877-41c9-ae0d-252933c26ced" />

- Proggy Fonts，作者 Tristan Grimmer https://github.com/bluescan/proggyfonts
- Sweet16、Sweet16 Mono，作者 Martin Sedlak（拉丁文 + 补充字符 + 扩展 A）https://github.com/kmar/Sweet16Font
- Google Noto Mono Fonts：https://www.google.com/get/noto/
- 用于美化源代码的字体：https://github.com/chrissimpkins/codeface
- 编程字体：http://s9w.github.io/font_compare/
- Inconsolata：http://www.levien.com/type/myfonts/inconsolata.html
- Adobe Source Code Pro：适用于 UI 和编码环境的等宽字体家族 https://github.com/adobe-fonts/source-code-pro
- 等宽/固定宽度程序员字体 http://www.lowing.org/fonts/

或者使用 Windows 自带的 Arial Unicode 或其他 Unicode 字体以获得完整的字符覆盖（其许可证情况不太确定）。

##### [返回目录](#index)
