## 如何构建


---

### 使用 CMake
#### 使用 Google Dawn 为桌面端（WebGPU 原生）构建：
 1. `git clone https://github.com/google/dawn dawn`
 2. `cmake -B build -DIMGUI_DAWN_DIR=dawn`
 3. `cmake --build build`
生成的二进制文件将位于以下位置之一：
  * build/Debug/example_sdl2_wgpu[.exe]
  * build/example_sdl2_wgpu[.exe]

#### 使用 WGPU 为桌面端（WebGPU 原生）构建：
 1. 从以下地址下载适用于你的平台/编译器的 WGPU-Native 自动生成二进制模块：https://github.com/gfx-rs/wgpu-native/releases
 2. 将下载的文件解压到 `your_preferred_folder`
 3. `cmake -B build -DIMGUI_WGPU_DIR=your_preferred_folder`（“完整路径”或从当前目录开始的“相对路径”）
 4. `cmake --build build`
生成的二进制文件将位于以下位置之一：
  * build/Debug/example_sdl2_wgpu[.exe]
  * build/example_sdl2_wgpu[.exe]

#### 为 Emscripten 构建：
 1. 按照以下说明安装 Emscripten SDK：https://emscripten.org/docs/getting_started/downloads.html
 2. 安装 Ninja 构建系统
 3. `emcmake cmake -G Ninja -B build`
    - （可选）`-DIMGUI_EMSCRIPTEN_WEBGPU_FLAG="--use-port=path/to/emdawnwebgpu_package/emdawnwebgpu.port.py"`，见下文
 4. `cmake --build build`

#### 将 Emscripten 与最新 Google Dawn 同步：
如果你希望将 Emscripten 与最新的 DAWN 版本同步，需要从以下地址下载 `port-emdawnwgpu-package`（由 Google 每日发布）：
https://github.com/google/dawn/releases
将其解压到你偏好的文件夹，并将第 3 步替换为：

3. `emcmake cmake -DIMGUI_EMSCRIPTEN_WEBGPU_FLAG="--use-port=path/to/emdawnwebgpu_package/emdawnwebgpu.port.py" -G Ninja -B build`

**注意**
为了让 Emscripten 生成的 WASM 代码正常运行，还需要有包含最新更改的“对应”（或更新）版本的 Google Canary（面向开发者的每夜构建版）


---

### 分步使用 CMake

#### 生成 Dawn 原生版：

- `cmake -G Ninja -DIMGUI_DAWN_DIR=path_to_sdk_dir -B where_to_build_dir`
   - 使用 `IMGUI_DAWN_DIR` 会设置 `IMGUI_IMPL_WEBGPU_BACKEND_DAWN` 编译器定义

#### 生成 WGPU 原生版：

- `cmake -G Ninja -DIMGUI_WGPU_DIR=path_to_sdk_dir -B where_to_build_dir`

   - 使用 `IMGUI_WGPU_DIR` 会设置 `IMGUI_IMPL_WEBGPU_BACKEND_WGPU` 编译器定义

#### 生成 Emscripten 构建：

- `emcmake cmake -G Ninja -B where_to_build_dir`\
CMake 会检查 EMSCRIPEN 版本，然后：
   - 如果 EMS >= 4.0.10，则使用 `--use-port=emdawnwebgpu` 标志进行构建
      - 它会设置 `IMGUI_IMPL_WEBGPU_BACKEND_DAWN` 编译器定义
   - 如果 EMS < 4.0.10，构建将中止（我们的示例和 WGPU 后端不再支持 `-sUSE_WEBGPU=1`）

#### 使用外部 WebGPU 库（emdawnwebgpu_pkg）生成 Emscripten 构建

- `emcmake cmake -G Ninja -DIMGUI_EMSCRIPTEN_WEBGPU_FLAG="--use-port=path_to_emdawnwebgpu_pkg" -B where_to_build_dir`
   - 它会设置 `IMGUI_IMPL_WEBGPU_BACKEND_DAWN` 编译器定义
   - *使用外部 WebGPU 库需要 EMS >= 4.0.10，或满足该包指定的最低要求：*
      - https://github.com/google/dawn/releases

#### 构建过程

一旦为特定构建器生成了构建流程，构建命令就**始终相同**：

- 使用 CMake 构建
   - `cmake --build where_to_build_dir`
      - 它将使用选定的构建器来构建示例。

- 显式构建：
   - `cd where_to_build_dir`
   - `ninja`
      - 这是生成阶段所选的构建器

---

### CMake 实用选项
#### 生成器类型（**ninja** 构建器的替代方案）：
- `-G Ninja` 使用 __ninja__ 构建器进行构建
- `-G "Unix Makefiles"` 使用 __make__ 构建器进行构建
- `-G "Visual Studio 17 2022" -A x64` 创建 VS 2022 解决方案（.sln）文件，仅限 Windows
   - **仅限原生构建**
   - **官方**并不支持用于构建 Google Dawn

示例：
- 使用 **make** 替代 **ninja**：
   - `cmake -G "Unix Makefiles" -DIMGUI_DAWN_DIR=path_to_sdk_dir -B where_to_build_dir`

**语法区分大小写，当单词之间有空格时必须使用 ""*

#### 目录
- 目录路径可以是绝对路径，也可以是相对路径（从当前目录开始）
- 不同的 CMake 生成必须使用不同的 `where_to_build_dir`


#### 构建类型
默认构建类型是 **Debug**
可以使用以下方式指定不同的构建类型：
- `-DCMAKE_BUILD_TYPE=Release`
- `-DCMAKE_BUILD_TYPE=MinSizeRel`
- `-DCMAKE_BUILD_TYPE=RelWithDebInfo`

示例：
- 构建 **Release**：
   - `cmake -G ninja -DIMGUI_WGPU_DIR=path_to_sdk_dir -DCMAKE_BUILD_TYPE=Release -B where_to_build_dir `

#### GLFW / SDL2 / SDL3 头文件、库、搜索路径和包管理器

默认情况下，头文件和库会在系统/编译器路径（环境变量）中搜索：你可以将开发工具的路径添加到环境变量中，而无需修改 `CMakeLists.txt` 文件。
- 例如，CLang 会在以下环境变量指定的路径中搜索：
    - 头文件：CPATH, C_INCLUDE_PATH, CPLUS_INCLUDE_PATH
    - 库文件：LIBRARY_PATH

如果你使用包管理器（**vcpkg** / **conan** / ...），则需要/可以在 cmake 命令中添加以下内容来指定它：
- `-DCMAKE_TOOLCHAIN_FILE=path/to/package_manager.cmake`

示例：

- 使用 **vcpkg** 包管理器时，需要添加：
   - `-DCMAKE_TOOLCHAIN_FILE=<vcpkg_root_dir>/scripts/buildsystems/vcpkg.cmake`

- 使用 **vcpkg** 包管理器的完整 cmake 命令：
   - `cmake -G Ninja -DIMGUI_DAWN_DIR=path_to_sdk_dir -DCMAKE_TOOLCHAIN_FILE=<vcpkg_root_dir>/scripts/buildsystems/vcpkg.cmake -B where_to_build_dir`

---


### 使用 makefile

- 你需要从 https://emscripten.org/docs/getting_started/downloads.html 安装 Emscripten，并按照 https://emscripten.org/docs/getting_started/downloads.html#installation-instructions 所述设置环境变量

- 根据你的配置，在 Windows 上可能需要在控制台中运行 `emsdk/emsdk_env.bat` 才能访问 Emscripten 命令行工具。

- 你也可以参考我们的 [持续集成配置](https://github.com/ocornut/imgui/tree/master/.github/workflows) 来设置 Emscripten。

- 然后使用 `make -f Makefile.emscripten` 在 `example_glfw_wgpu/` 目录中进行构建。

- 需要较新的 Emscripten，因为 WGPU 仍是一个开发中的 API。

---

## 如何运行

在本地机器上运行：
- 确保你的浏览器支持 WGPU 且已启用。WGPU 仍处于开发中，在大多数浏览器中默认未启用。
- `make serve` 将使用 Python3 启动本地 Web 服务器，然后你可以浏览 http://localhost:8000 访问你的构建结果。
- 否则，通常你需要一个本地 Web 服务器：
  - 引用 [https://emscripten.org/docs/getting_started](https://emscripten.org/docs/getting_started/Tutorial.html#generating-html)：<br>
_"遗憾的是，若干浏览器（包括 Chrome、Safari 和 Internet Explorer）不支持 file:// [XHR](https://emscripten.org/docs/site/glossary.html#term-xhr) 请求，也无法加载 HTML 所需的额外文件（例如 .wasm 文件，或下文提到的打包文件数据）。对于这些浏览器，你需要使用[本地 Web 服务器](https://emscripten.org/docs/getting_started/FAQ.html#faq-local-webserver)来提供这些文件，然后打开 http://localhost:8000/hello.html 。"_
  - Emscripten SDK 提供了一个便捷的 `emrun` 命令：`emrun web/example_glfw_wgpu.html --browser firefox`，它会在 Firefox 中启动一个临时的本地 Web 服务器。详情请参阅 https://emscripten.org/docs/compiling/Running-html-files-with-emrun.html 。
  - 你可以使用 Python 3 内置 Web 服务器：`python -m http.server -d web`（这正是 `make serve` 所使用的）。
  - 你可以使用 Python 2 内置 Web 服务器：`cd web && python -m SimpleHTTPServer`。
  - 如果你通过网络访问这些文件，某些浏览器（如 Firefox）会把 Gamepad API 访问限制为仅安全上下文（例如仅 https）。
