
# 如何构建

## 在 Windows 上使用 Visual Studio IDE

使用提供的项目文件（.vcxproj）。如有必要，将其添加到解决方案（imgui_examples.sln）中。

## 在 Windows 上使用 Visual Studio 命令行

使用 build_win32.bat，或直接执行：
```
set SDL2_DIR=path_to_your_sdl2_folder
cl /Zi /MD /utf-8 /I.. /I..\.. /I%SDL2_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl2.cpp ..\..\backends\imgui_impl_opengl3.cpp ..\..\imgui*.cpp /FeDebug/example_sdl2_opengl3.exe /FoDebug/ /link /libpath:%SDL2_DIR%\lib\x86 SDL2.lib SDL2main.lib opengl32.lib /subsystem:console
#                 ^^ 包含路径                  ^^ 源文件                                                                                    ^^ 输出 exe                    ^^ 输出目录   ^^ 库
# 或用于 64 位：
cl /Zi /MD /utf-8 /I.. /I..\.. /I%SDL2_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl2.cpp ..\..\backends\imgui_impl_opengl3.cpp ..\..\imgui*.cpp /FeDebug/example_sdl2_opengl3.exe /FoDebug/ /link /libpath:%SDL2_DIR%\lib\x64 SDL2.lib SDL2main.lib opengl32.lib /subsystem:console
```

## Linux 及类似 Unix 系统

使用我们的 Makefile，或直接执行：
```
c++ `sdl2-config --cflags` -I .. -I ../.. -I ../../backends
  main.cpp ../../backends/imgui_impl_sdl2.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp
  `sdl2-config --libs` -lGL -ldl
```

## macOS

使用我们的 Makefile，或直接执行：
```
brew install sdl2
c++ `sdl2-config --cflags` -I .. -I ../.. -I ../../backends
  main.cpp ../../backends/imgui_impl_sdl2.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp
  `sdl2-config --libs` -framework OpenGl -framework CoreFoundation
```

## Emscripten

**构建**

你需要从 https://emscripten.org/docs/getting_started/downloads.html 安装 Emscripten，并按照 https://emscripten.org/docs/getting_started/downloads.html#installation-instructions 中的说明设置环境变量。

- 根据你的配置，在 Windows 上你可能需要在控制台中运行 `emsdk/emsdk_env.bat`，才能访问 Emscripten 命令行工具。
- 你也可以参考我们的[持续集成配置](https://github.com/ocornut/imgui/tree/master/.github/workflows)来设置 Emscripten。
- 然后在当前目录中使用 `make -f Makefile.emscripten` 进行构建。

**运行 Emscripten 项目**

在本地机器上运行：
- `make -f Makefile.emscripten serve` 将使用 Python3 启动本地 Web 服务器，然后你可以浏览 http://localhost:8000 来访问构建结果。
- 否则，通常你需要一个本地 Web 服务器。引用 [https://emscripten.org/docs/getting_started](https://emscripten.org/docs/getting_started/Tutorial.html#generating-html) 的说法：<br>
_"遗憾的是，若干浏览器（包括 Chrome、Safari 和 Internet Explorer）不支持 file:// [XHR](https://emscripten.org/docs/site/glossary.html#term-xhr) 请求，也无法加载 HTML 所需的额外文件（例如 .wasm 文件，或下文提到的打包文件数据）。对于这些浏览器，你需要使用[本地 Web 服务器](https://emscripten.org/docs/getting_started/FAQ.html#faq-local-webserver)来提供这些文件，然后打开 http://localhost:8000/hello.html 。"_
- Emscripten SDK 提供了一个方便的 `emrun` 命令：`emrun web/index.html --browser firefox`，它会启动一个临时的本地 Web 服务器（使用 Firefox）。有关详细信息，请参见 https://emscripten.org/docs/compiling/Running-html-files-with-emrun.html 。
- 你可以使用 Python 3 内置的 Web 服务器：`python -m http.server -d web`（`make serve` 使用的就是这个）。
- 你可以使用 Python 2 内置的 Web 服务器：`cd web && python -m SimpleHTTPServer`。
- 如果你通过网络访问这些文件，某些浏览器（例如 Firefox）会将 Gamepad API 访问限制为仅在安全上下文中可用（例如仅 HTTPS）。
