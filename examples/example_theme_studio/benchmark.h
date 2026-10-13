#pragma once
#include "imgui_themes.h"
#include <string>
#include <vector>

namespace ThemeBenchmark
{
struct Settings { int warmup = 120; int samples = 600; };
struct Layout { int submitted = 0; int visible = 0; bool fits = true; };
struct Geometry
{
    int vertices = 0, indices = 0, commands = 0;
    bool operator==(const Geometry& other) const
    { return vertices==other.vertices && indices==other.indices && commands==other.commands; }
};
struct Result
{
    ImGuiThemes::Id id = ImGuiThemes::Id::Default;
    double median_us = 0, p95_us = 0;
    Geometry geometry;
    int samples = 0, submitted = 0;
};
struct Metadata
{
    int window_width = 1440, window_height = 960;
    int framebuffer_width = 1440, framebuffer_height = 960;
    std::string font, renderer, build;
};
class Session
{
public:
    explicit Session(Settings settings);
    int ThemeIndex() const { return index_; }
    bool Done() const { return index_==ImGuiThemes::ThemeCount; }
    const std::vector<Result>& Results() const { return results_; }
    const Settings& Config() const { return settings_; }
    const std::string& Error() const { return error_; }
    bool Record(double cpu_us, Geometry geometry, Layout layout);
private:
    Settings settings_;
    int index_ = 0, frame_ = 0, items_ = 0;
    Geometry geometry_;
    std::vector<double> times_;
    std::vector<Result> results_;
    std::string error_;
};

Layout DrawScene(const ImGuiThemes::Theme& theme);
Geometry MeasureGeometry(const ImDrawData& data);
bool WriteCsv(const char* path, const Session& session, const Metadata& metadata, std::string& error);
}
