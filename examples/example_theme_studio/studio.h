#pragma once
#include "imgui_themes.h"
#include <string>
#include <vector>

namespace ThemeStudio
{
struct Asset { std::string name, type; };
struct State
{
    ImGuiThemes::Theme themes[ImGuiThemes::ThemeCount];
    bool dirty[ImGuiThemes::ThemeCount] = {};
    int selected = static_cast<int>(ImGuiThemes::Id::Bento);
    int applied = -1;
    bool pending = true;
    float scale = 1.0f;
    float render_scale = 1.0f;
    std::string directory;
    std::string message;
    bool error = false;
    int navigation = 1;
    int color_slot = ImGuiCol_WindowBg;
    float intensity = 64;
    int quality = 1;
    bool live = true;
    int apply_count = 0;
    int selected_asset = 0;
    char profile[64] = "Preview profile";
    std::vector<Asset> assets;
    State();
};

void LoadPresets(State& state, const std::string& directory);
void ApplyPending(State& state);
void Draw(State& state);
}
