# 主题预览与验证资料

[工程 README](../README.md#themes--styles) · [构建、编辑与接入说明](../../examples/example_theme_studio/README.md) · [主题模块](../../misc/themes/)

下面是 Theme Studio 经真实 OpenGL 后端渲染的验证截图，包含只读原生基准和五套自定义主题。六款使用同一套应用布局和组件，切换主题保留示例数据与选中状态。

![00、01、03、04、07、10 实际运行总览](images/overview.png)

| 编号 | 主题与高清截图 | 配置 | 实现范围 |
| --- | --- | --- | --- |
| 00 | [ImGui Default](images/00-default.png) | 内置，只读 | 默认尺寸、深色配色、原生按钮 |
| 01 | [赛博朋克](images/01-cyberpunk.png) | [cyberpunk.ini](../../examples/example_theme_studio/themes/cyberpunk.ini) | 深蓝黑、青色和品红，无发光 |
| 03 | [简约留白](images/03-minimal.png) | [minimal.ini](../../examples/example_theme_studio/themes/minimal.ini) | 暖白、灰阶、低饱和绿 |
| 04 | [精密线条](images/04-wireframe.png) | [wireframe.ini](../../examples/example_theme_studio/themes/wireframe.ini) | 石墨灰、细边框、轮廓按钮 |
| 07 | [新粗野主义](images/07-brutalist.png) | [brutalist.ini](../../examples/example_theme_studio/themes/brutalist.ini) | 黑色描边、蓝黄撞色、实色硬阴影 |
| 10 | [深色模块卡片](images/10-bento.png) | [bento.ini](../../examples/example_theme_studio/themes/bento.ini) | 石墨色卡片、青柠强调、小圆角 |

## 验证条件

- Dear ImGui 1.93.0 WIP，Linux / GLFW / OpenGL3，AMD Radeon 780M。
- 原始单款截图为 1440×960，UI scale=1，仓库自带 DroidSans 字体。
- [截图来源记录](capture-info.json)保留采集方式与主题清单。
- 图片内 FPS 是短时截图过程中的动态值，不能作为主题性能排名。
- 性能比较使用另一个固定组件场景，隐藏编辑器；具体条件、CPU/P95 与几何量见[性能验证记录](benchmark.md)和[原始 CSV](benchmark.csv)。

## 重新生成验证资料

从仓库根目录执行：

```sh
cmake -S examples/example_theme_studio -B examples/example_theme_studio/build-release -DCMAKE_BUILD_TYPE=Release
cmake --build examples/example_theme_studio/build-release -j 4
ctest --test-dir examples/example_theme_studio/build-release --output-on-failure
mkdir -p /tmp/imgui-theme-captures
examples/example_theme_studio/build-release/theme_studio --capture-dir /tmp/imgui-theme-captures
examples/example_theme_studio/build-release/theme_studio --benchmark /tmp/imgui-theme-benchmark.csv
```

截图模式需要可连接的图形会话，输出原始 PPM；本目录的 PNG 是从对应 PPM 无损转换的，`overview.png` 是缩小后的对比拼图。自动测试还覆盖只读基准、配置事务性、DPI、主题切换数据保持、按钮行为及采样统计。

这些文件是已完成验证的记录。重新运行的输出放在 `/tmp`，核对后再更新归档图片和 CSV，避免把一次新测量误当成旧记录。
