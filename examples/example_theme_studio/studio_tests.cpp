#include "studio.h"
#include "imgui_impl_null.h"
#include <cstdio>
#include <stdexcept>

static void Require(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
static void BeginFrame()
{
    ImGui_ImplNull_NewFrame();
    ImGui::GetIO().DisplaySize=ImVec2(1440,960);
    ImGui::NewFrame();
}
static void EndFrame()
{
    ImGui::Render();
    ImGui_ImplNullRender_RenderDrawData(ImGui::GetDrawData());
}
static void StudioSwitchPreservesData()
{
    ThemeStudio::State state;
    state.intensity=37; state.quality=2; state.live=false; state.apply_count=8;
    state.selected_asset=1;
    state.assets[1].name="Edited asset";
    for (int theme=0;theme<ImGuiThemes::ThemeCount;++theme)
    {
        state.selected=theme;
        state.scale=theme%2==0?1.0f:1.25f;
        for (int frame=0;frame<3;++frame)
        {
            ThemeStudio::ApplyPending(state);
            BeginFrame(); ThemeStudio::Draw(state); EndFrame();
        }
        Require(state.applied==theme,"selected theme must become active");
        Require(state.intensity==37 && state.quality==2 && !state.live && state.apply_count==8,"switch must preserve component values");
        Require(state.selected_asset==1 && state.assets[1].name=="Edited asset","switch must preserve edited data and selection");
        const ImDrawData* draw=ImGui::GetDrawData();
        Require(draw->TotalVtxCount>0,"studio must emit real geometry");
        int commands=0;
        for (const ImDrawList* list : draw->CmdLists)
            for (const ImDrawCmd& cmd : list->CmdBuffer)
            {
                Require(cmd.UserCallback==nullptr,"themes must not inject custom renderer callbacks");
                commands += cmd.ElemCount>0 ? 1 : 0;
            }
        std::printf("%s: %d vertices, %d indices, %d draw commands\n",ImGuiThemes::Name(state.themes[theme].id),draw->TotalVtxCount,draw->TotalIdxCount,commands);
    }
}

// Exercises actual mouse events against the semantic wrapper; styling must not swallow clicks.
static void SemanticButtonKeepsInputAndRestoresStyle()
{
    for (int i=0;i<ImGuiThemes::ThemeCount;++i)
    {
        ImGuiThemes::Theme theme=ImGuiThemes::Make(static_cast<ImGuiThemes::Id>(i));
        Require(ImGuiThemes::Apply(theme),"apply button theme");
        int clicks=0;
        for (int frame=0;frame<5;++frame)
        {
            ImGuiIO& io=ImGui::GetIO();
            io.AddMousePosEvent(160,160);
            if (frame==2) io.AddMouseButtonEvent(0,true);
            if (frame==3) io.AddMouseButtonEvent(0,false);
            BeginFrame();
            ImGui::SetNextWindowPos(ImVec2(100,100)); ImGui::SetNextWindowSize(ImVec2(400,250));
            ImGui::Begin("Button input test",nullptr,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoSavedSettings);
            ImGui::SetCursorScreenPos(ImVec2(150,150));
            const ImVec4 before=ImGui::GetStyle().Colors[ImGuiCol_Button];
            const float border=ImGui::GetStyle().FrameBorderSize;
            if (ImGuiThemes::Button(theme,ImGuiThemes::ButtonRole::Primary,"Action",ImVec2(120,40))) ++clicks;
            const ImVec4 after=ImGui::GetStyle().Colors[ImGuiCol_Button];
            Require(before.x==after.x && before.y==after.y && before.z==after.z && before.w==after.w
                && border==ImGui::GetStyle().FrameBorderSize,"button must restore style stack");
            ImGui::End(); EndFrame();
        }
        Require(clicks==1,"styled button must deliver exactly one click");
    }
}

static void SemanticButtonKeepsSizingAndClipping()
{
    ImGuiThemes::Theme theme=ImGuiThemes::Make(ImGuiThemes::Id::Brutalist);
    Require(ImGuiThemes::Apply(theme),"apply geometry theme");
    for (int frame=0;frame<2;++frame)
    {
        BeginFrame();
        ImGui::SetNextWindowPos(ImVec2(100,100)); ImGui::SetNextWindowSize(ImVec2(400,300));
        ImGui::Begin("Sizing test",nullptr,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoSavedSettings);
        const ImVec2 pos(150,150);
        const ImVec2 sizes[]={ImVec2(0,0),ImVec2(120,40),ImVec2(-10,-10)};
        for (int i=0;i<3;++i)
        {
            ImGui::PushID(i);
            ImGui::SetCursorScreenPos(pos); ImGui::Button("Action",sizes[i]);
            const ImVec2 expected=ImGui::GetItemRectSize();
            ImGui::SetCursorScreenPos(pos);
            ImGuiThemes::Button(theme,ImGuiThemes::ButtonRole::Primary,"Action##styled",sizes[i]);
            const ImVec2 actual=ImGui::GetItemRectSize();
            Require(expected.x==actual.x && expected.y==actual.y,"semantic button sizing must match native Button");
            ImGui::PopID();
        }
        ImGui::SetCursorScreenPos(ImVec2(150,2000));
        ImDrawList* list=ImGui::GetWindowDrawList(); const int vertices=list->VtxBuffer.Size;
        for (int i=0;i<100;++i)
        {
            ImGui::PushID(i);
            ImGuiThemes::Button(theme,ImGuiThemes::ButtonRole::Primary,"Clipped",ImVec2(120,40));
            ImGui::PopID();
        }
        Require(list->VtxBuffer.Size==vertices,"fully clipped buttons must not emit shadow geometry");
        ImGui::End(); EndFrame();
    }
}
static void DefaultButtonMatchesNativeGeometry()
{
    ImGuiThemes::Theme baseline=ImGuiThemes::Make(ImGuiThemes::Id::Default);
    baseline.shadow_offset=8; baseline.outline_buttons=true; baseline.accent=ImVec4(1,0,0,1);
    Require(ImGuiThemes::Apply(baseline),"apply immutable native baseline");
    for (int frame=0;frame<2;++frame)
    {
        BeginFrame();
        ImGui::SetNextWindowPos(ImVec2(100,100)); ImGui::SetNextWindowSize(ImVec2(400,300));
        ImGui::Begin("Native geometry test",nullptr,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs);
        ImDrawList* draw=ImGui::GetWindowDrawList();
        ImGui::SetCursorScreenPos(ImVec2(150,150));
        const int begin=draw->VtxBuffer.Size;
        ImGui::Button("Native##reference",ImVec2(120,40));
        const int split=draw->VtxBuffer.Size;
        ImGui::SetCursorScreenPos(ImVec2(150,150));
        ImGuiThemes::Button(baseline,ImGuiThemes::ButtonRole::Danger,"Native##baseline",ImVec2(120,40));
        Require(draw->VtxBuffer.Size-split==split-begin,"baseline must not add shadow/border geometry");
        for (int i=0;i<split-begin;++i)
        {
            const ImDrawVert& a=draw->VtxBuffer[begin+i]; const ImDrawVert& b=draw->VtxBuffer[split+i];
            Require(a.col==b.col && a.pos.x==b.pos.x && a.pos.y==b.pos.y,"baseline must match native button colors and positions");
        }
        ImGui::End(); EndFrame();
    }
}
int main()
{
    ImGui::CreateContext(); ImGui::GetIO().IniFilename=nullptr;
    ImGui_ImplNull_Init();
    int result=0;
    try { StudioSwitchPreservesData(); SemanticButtonKeepsInputAndRestoresStyle(); SemanticButtonKeepsSizingAndClipping(); DefaultButtonMatchesNativeGeometry(); }
    catch (const std::exception& e) { std::fprintf(stderr,"FAIL: %s\n",e.what()); result=1; }
    ImGui_ImplNull_Shutdown(); ImGui::DestroyContext();
    return result;
}
