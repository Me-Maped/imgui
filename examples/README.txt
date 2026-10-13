刚接触 Dear ImGui？
- 查看入门指南：https://github.com/ocornut/imgui/wiki/Getting-Started
- 关于 Backends：docs/BACKENDS.md 文件，或访问网页：https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md
- 关于 Examples：docs/EXAMPLES.md 文件，或访问网页：https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md

Backends = 辅助代码，用于简化与平台/graphics API 的集成（供 Examples 使用，你的应用也应使用）。
Examples = 展示与平台/graphics API 集成的独立应用程序。

部分 Examples 在其各自目录中还有额外的 README 文件，请一并查看！

本仓库新增的 Theme Studio：
- example_theme_studio/README.md：构建、主题切换、外观编辑、INI 保存/重载与性能比较。
- ../docs/themes/README.md：00 原生基准及五款自定义主题的实际验证截图。
- ../misc/themes/：可独立接入应用的主题模块，不修改 ImGui 核心。

当 Dear ImGui 运行起来后（无论是在 examples 中，还是在你自己的应用/游戏/引擎中），
运行并参考 imgui_demo.cpp 中的 ImGui::ShowDemoWindow()，了解面向最终用户的 API。
