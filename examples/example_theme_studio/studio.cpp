#include "studio.h"
#include <algorithm>
#include <cstdio>

namespace ThemeStudio
{
using ImGuiThemes::ButtonRole;
namespace
{
std::string Path(const State& s, int index) { return s.directory+"/"+ImGuiThemes::Slug(static_cast<ImGuiThemes::Id>(index))+".ini"; }
bool Action(State& s, ButtonRole role, const char* label, ImVec2 size=ImVec2(0,0))
{
    return ImGuiThemes::Button(s.themes[s.applied],role,label,size,s.render_scale);
}
void Status(State& s, const std::string& message, bool error=false) { s.message=message; s.error=error; }
void Metric(State& s, const char* id, const char* title, const char* value)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(12*s.render_scale,10*s.render_scale));
    ImGui::BeginChild(id,ImVec2(0,85*s.render_scale),ImGuiChildFlags_Borders);
    ImGui::TextDisabled("%s",title);
    ImGui::PushFont(nullptr,ImGui::GetStyle().FontSizeBase*1.45f);
    ImGui::TextColored(s.themes[s.applied].accent,"%s",value);
    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::PopStyleVar();
}
void Metrics(State& s)
{
    if (ImGui::BeginTable("metrics",3,ImGuiTableFlags_SizingStretchSame))
    {
        char count[32]; std::snprintf(count,sizeof(count),"%d",static_cast<int>(s.assets.size()));
        ImGui::TableNextColumn(); Metric(s,"assets-stat","ASSETS",count);
        std::snprintf(count,sizeof(count),"%02d",ImGuiThemes::ThemeCount);
        ImGui::TableNextColumn(); Metric(s,"theme-stat","THEMES",count);
        ImGui::TableNextColumn(); Metric(s,"render-stat","EFFECT PASSES","00");
        ImGui::EndTable();
    }
}
void Assets(State& s)
{
    ImGui::SeparatorText("Resource library");
    if (ImGui::BeginTable("resources",3,ImGuiTableFlags_RowBg|ImGuiTableFlags_BordersInnerH|ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Name",ImGuiTableColumnFlags_WidthStretch,2);
        ImGui::TableSetupColumn("Type"); ImGui::TableSetupColumn("Status"); ImGui::TableHeadersRow();
        for (int i=0;i<static_cast<int>(s.assets.size());++i)
        {
            ImGui::PushID(i); ImGui::TableNextRow(); ImGui::TableNextColumn();
            if (ImGui::Selectable(s.assets[i].name.c_str(),s.selected_asset==i,ImGuiSelectableFlags_SpanAllColumns)) s.selected_asset=i;
            ImGui::TableNextColumn(); ImGui::TextUnformatted(s.assets[i].type.c_str());
            ImGui::TableNextColumn(); ImGui::TextUnformatted("Ready");
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    if (s.assets.empty()) ImGui::TextDisabled("No assets. Use Reset to restore the sample.");
}
void ResetSample(State& s)
{
    s.intensity=64; s.quality=1; s.live=true; s.apply_count=0; s.selected_asset=0;
    s.assets={{"Environment", "Scene"},{"Character", "Mesh"},{"UI Atlas", "Texture"},{"Stone surface", "Material"}};
}
void Components(State& s)
{
    ImGui::SeparatorText("Component gallery");
    if (ImGui::BeginTabBar("component-tabs"))
    {
        if (ImGui::BeginTabItem("Basic"))
        {
            ImGui::Spacing();
            const float w=std::max(60.0f,(ImGui::GetContentRegionAvail().x-ImGui::GetStyle().ItemSpacing.x*2)/3);
            if (Action(s,ButtonRole::Primary,"Apply",ImVec2(w,0)))
            { ++s.apply_count; Status(s,"Preview applied. Sample data is independent of the theme."); }
            ImGui::SameLine();
            if (Action(s,ButtonRole::Secondary,"Reset",ImVec2(w,0))) ResetSample(s);
            ImGui::SameLine();
            ImGui::BeginDisabled(s.assets.empty());
            if (Action(s,ButtonRole::Danger,"Delete",ImVec2(w,0)))
            {
                s.assets.erase(s.assets.begin()+s.selected_asset);
                s.selected_asset=std::max(0,std::min(s.selected_asset,static_cast<int>(s.assets.size())-1));
                Status(s,"Selected sample asset removed.");
            }
            ImGui::EndDisabled();
            ImGui::Spacing(); ImGui::Checkbox("Live preview",&s.live);
            ImGui::SetNextItemWidth(-100*s.render_scale);
            ImGui::Combo("Quality",&s.quality,"Draft\0Balanced\0High\0");
            ImGui::SetNextItemWidth(-100*s.render_scale);
            ImGui::SliderFloat("Intensity",&s.intensity,0,100,"%.0f%%");
            ImGui::ProgressBar(s.intensity/100,ImVec2(-1,0));
            ImGui::TextDisabled("Applied %d times",s.apply_count);
            ImGui::BeginDisabled();
            Action(s,ButtonRole::Secondary,"Disabled action");
            ImGui::EndDisabled();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Data"))
        {
            ImGui::InputText("Profile",s.profile,sizeof(s.profile));
            ImGui::TextWrapped("Change these values, then switch themes. The same data and selection remain active.");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Layout"))
        {
            ImGui::TextWrapped("All themes share this layout and the same standard widgets. Custom appearance settings live in independent files.");
            ImGui::BulletText("No glow or blur"); ImGui::BulletText("No custom shader or effect pass");
            ImGui::BulletText("Hard shadows use solid rectangles");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}
void FileActions(State& s)
{
    ImGuiThemes::Theme& t=s.themes[s.applied];
    if (ImGui::Button("Save theme"))
    {
        std::string error;
        if (ImGuiThemes::Save(Path(s,s.applied).c_str(),t,error))
        { s.dirty[s.applied]=false; Status(s,"Saved "+Path(s,s.applied)); }
        else Status(s,error,true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Reload"))
    {
        ImGuiThemes::Theme candidate=t; std::string error;
        if (ImGuiThemes::Load(Path(s,s.applied).c_str(),candidate,error))
        {
            if (candidate.id==t.id)
            { t=candidate; s.dirty[s.applied]=false; s.pending=true; Status(s,"Reloaded "+Path(s,s.applied)); }
            else Status(s,"Preset in file does not match this theme slot.",true);
        }
        else Status(s,error,true);
    }
    if (ImGui::Button("Reset to built-in preset"))
    { t=ImGuiThemes::Make(t.id); s.dirty[s.applied]=true; s.pending=true; Status(s,"Built-in appearance restored. Save to keep it."); }
    ImGui::TextDisabled("%s.ini",ImGuiThemes::Slug(t.id));
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",Path(s,s.applied).c_str());
}
void Editor(State& s)
{
    ImGuiThemes::Theme& t=s.themes[s.applied];
    bool changed=false;
    ImGui::SeparatorText("Theme editor");
    if (t.id==ImGuiThemes::Id::Default)
    {
        ImGui::TextUnformatted("Read-only baseline");
        ImGui::TextWrapped("Native ImGui dimensions, dark colors and buttons. This baseline cannot be edited or saved.");
        ImGui::SeparatorText("Performance comparison");
        ImGui::TextWrapped("Use --benchmark results.csv for a fixed scene without the editor. It measures UI CPU construction and geometry, not GPU time.");
        return;
    }
    ImGui::TextDisabled("%s",s.dirty[s.applied]?"Unsaved changes":"Saved / preset values");
    FileActions(s);
    ImGui::SeparatorText("Shape & spacing");
    ImGui::SetNextItemWidth(-1);
    changed |= ImGui::SliderFloat("##rounding",&t.style.FrameRounding,0,12,"Control radius: %.0f");
    ImGui::SetNextItemWidth(-1);
    changed |= ImGui::SliderFloat("##panelround",&t.style.ChildRounding,0,16,"Panel radius: %.0f");
    ImGui::SetNextItemWidth(-1);
    changed |= ImGui::SliderFloat("##border",&t.style.FrameBorderSize,0,3,"Control border: %.0f");
    ImGui::SetNextItemWidth(-1);
    changed |= ImGui::SliderFloat("##shadow",&t.shadow_offset,0,6,"Hard shadow: %.0f");
    changed |= ImGui::Checkbox("Outline buttons",&t.outline_buttons);
    ImGui::SetNextItemWidth(-1);
    changed |= ImGui::DragFloat2("##padding",&t.style.FramePadding.x,0.2f,0,24,"Padding: %.0f");
    ImGui::SetNextItemWidth(-1);
    changed |= ImGui::DragFloat2("##spacing",&t.style.ItemSpacing.x,0.2f,0,24,"Spacing: %.0f");
    ImGui::SeparatorText("Semantic colors");
    const ImGuiColorEditFlags flags=ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_AlphaBar;
    changed |= ImGui::ColorEdit4("Accent",&t.accent.x,flags);
    ImGui::SameLine(); changed |= ImGui::ColorEdit4("Text##accent",&t.accent_text.x,flags);
    changed |= ImGui::ColorEdit4("Danger",&t.danger.x,flags);
    ImGui::SameLine(); changed |= ImGui::ColorEdit4("Text##danger",&t.danger_text.x,flags);
    ImGui::SeparatorText("Widget colors");
    ImGui::SetNextItemWidth(-1);
    if (ImGui::BeginCombo("##color-slot",ImGui::GetStyleColorName(s.color_slot)))
    {
        for (int i=0;i<ImGuiCol_COUNT;++i)
        {
            if (ImGui::Selectable(ImGui::GetStyleColorName(i),i==s.color_slot)) s.color_slot=i;
            if (i==s.color_slot) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    changed |= ImGui::ColorEdit4("Selected color",&t.style.Colors[s.color_slot].x,flags);
    if (changed) { s.dirty[s.applied]=true; s.pending=true; }
}
}

State::State()
{
    for (int i=0;i<ImGuiThemes::ThemeCount;++i) themes[i]=ImGuiThemes::Make(static_cast<ImGuiThemes::Id>(i));
    ResetSample(*this);
}
void LoadPresets(State& s, const std::string& directory)
{
    s.directory=directory;
    for (int i=0;i<ImGuiThemes::ThemeCount;++i)
    {
        if (static_cast<ImGuiThemes::Id>(i)==ImGuiThemes::Id::Default) continue;
        std::string error; ImGuiThemes::Theme candidate=s.themes[i];
        if (ImGuiThemes::Load(Path(s,i).c_str(),candidate,error) && candidate.id==s.themes[i].id) s.themes[i]=candidate;
        else Status(s,error.empty()?"Preset identity mismatch: "+Path(s,i):error,true);
    }
    s.pending=true;
}
void ApplyPending(State& s)
{
    if (s.selected<0 || s.selected>=ImGuiThemes::ThemeCount) { Status(s,"Invalid theme selection",true); return; }
    if (s.pending || s.applied!=s.selected || s.scale!=s.render_scale)
    {
        if (ImGuiThemes::Apply(s.themes[s.selected],s.scale))
        {
            s.applied=s.selected; s.render_scale=s.scale; s.pending=false;
            ImGui::GetStyle().FontScaleDpi=s.render_scale;
        }
        else Status(s,"Invalid appearance or scale; previous theme retained.",true);
    }
}
void Draw(State& s)
{
    if (s.applied<0) return;
    ImGuiViewport* viewport=ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos); ImGui::SetNextWindowSize(viewport->WorkSize);
    const ImGuiWindowFlags flags=ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings;
    if (!ImGui::Begin("Theme Studio",nullptr,flags)) { ImGui::End(); return; }
    ImGui::PushFont(nullptr,ImGui::GetStyle().FontSizeBase*1.6f);
    ImGui::TextUnformatted("STYLE STUDIO"); ImGui::PopFont();
    ImGui::SameLine(); ImGui::TextDisabled(" /  %s",ImGuiThemes::Name(s.themes[s.applied].id));
    ImGui::TextDisabled("Appearance lives here. Your application state stays yours.");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(230*s.render_scale);
    if (ImGui::BeginCombo("Theme",ImGuiThemes::Name(static_cast<ImGuiThemes::Id>(s.selected))))
    {
        for (int i=0;i<ImGuiThemes::ThemeCount;++i)
            if (ImGui::Selectable(ImGuiThemes::Name(static_cast<ImGuiThemes::Id>(i)),s.selected==i)) s.selected=i;
        ImGui::EndCombo();
    }
    ImGui::SameLine(); ImGui::SetNextItemWidth(140*s.render_scale);
    ImGui::SliderFloat("UI scale",&s.scale,0.75f,1.5f,"%.2fx");
    ImGui::Separator();
    float body=std::max(200.0f,ImGui::GetContentRegionAvail().y-ImGui::GetFrameHeightWithSpacing()*1.8f);
    if (ImGui::BeginTable("workspace",3,ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("navigation",ImGuiTableColumnFlags_WidthFixed,155*s.render_scale);
        ImGui::TableSetupColumn("preview",ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("editor",ImGuiTableColumnFlags_WidthFixed,300*s.render_scale);
        ImGui::TableNextColumn();
        ImGui::BeginChild("navigation",ImVec2(0,body),ImGuiChildFlags_Borders);
        ImGui::TextDisabled("WORKSPACE"); ImGui::Spacing();
        const char* items[]={"Overview","Components","Assets","Settings"};
        for (int i=0;i<4;++i)
            if (ImGui::Selectable(items[i],s.navigation==i,0,ImVec2(0,ImGui::GetFrameHeight()))) s.navigation=i;
        ImGui::Spacing(); ImGui::Separator(); ImGui::TextDisabled("%d themes\n1 component set",ImGuiThemes::ThemeCount);
        ImGui::EndChild();
        ImGui::TableNextColumn();
        ImGui::BeginChild("preview",ImVec2(0,body),ImGuiChildFlags_Borders);
        Metrics(s);
        if (s.navigation==0)
        {
            ImGui::SeparatorText("A shared workspace");
            ImGui::TextWrapped("Choose a theme above, then tune its appearance on the right. Your component values and selected asset remain unchanged.");
            ImGui::Spacing(); ImGui::TextWrapped("The five presets use ordinary ImGui geometry. Cyberpunk is color only. Neo Brutalism adds a solid offset rectangle behind action buttons.");
            Assets(s);
        }
        else if (s.navigation==2) Assets(s);
        else if (s.navigation==3)
        {
            ImGui::SeparatorText("Profile settings");
            ImGui::InputText("Name",s.profile,sizeof(s.profile)); ImGui::Checkbox("Live preview",&s.live);
            ImGui::TextWrapped("Theme files can be edited in a text editor. Reload applies valid changes; malformed files keep the current appearance intact.");
            ImGui::TextWrapped("Save stores the active theme only. Reset restores its built-in appearance, and does not change your component data.");
        }
        else { Components(s); ImGui::Spacing(); Assets(s); }
        ImGui::EndChild();
        ImGui::TableNextColumn();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(14*s.render_scale,12*s.render_scale));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(8*s.render_scale,6*s.render_scale));
        ImGui::BeginChild("editor",ImVec2(0,body),ImGuiChildFlags_Borders); Editor(s); ImGui::EndChild();
        ImGui::PopStyleVar(2);
        ImGui::EndTable();
    }
    ImGui::Separator();
    ImGui::TextDisabled("%.1f FPS  |  %d vertices  |  %d indices",ImGui::GetIO().Framerate,ImGui::GetIO().MetricsRenderVertices,ImGui::GetIO().MetricsRenderIndices);
    if (!s.message.empty())
    {
        if (s.error) ImGui::PushStyleColor(ImGuiCol_Text,s.themes[s.applied].danger);
        ImGui::TextWrapped("%s",s.message.c_str());
        if (s.error) ImGui::PopStyleColor();
    }
    ImGui::End();
}
}
