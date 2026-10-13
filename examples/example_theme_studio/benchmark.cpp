#include "benchmark.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <locale>
#include <stdexcept>

namespace ThemeBenchmark
{
Session::Session(Settings settings) : settings_(settings)
{
    if (settings.warmup<1 || settings.warmup>100000 || settings.samples<2 || settings.samples>1000000)
        throw std::invalid_argument("warmup must be 1..100000; samples must be 2..1000000");
    times_.reserve(static_cast<std::size_t>(settings.samples));
    results_.reserve(ImGuiThemes::ThemeCount);
}
bool Session::Record(double cpu_us, Geometry geometry, Layout layout)
{
    if (!error_.empty()) return false;
    if (Done()) { error_="Session already complete"; return false; }
    if (!std::isfinite(cpu_us) || cpu_us<=0) { error_="Non-positive or non-finite CPU timing"; return false; }
    if (frame_>=settings_.warmup)
    {
        if (!layout.fits || layout.submitted<=0 || layout.visible!=layout.submitted)
        { error_="Benchmark content is clipped; enlarge the scene or reduce configured spacing"; return false; }
        if (items_ && items_!=layout.submitted) { error_="Widget workload changed between themes"; return false; }
        items_=layout.submitted;
        if (geometry.vertices<=0 || geometry.indices<=0 || geometry.commands<=0)
        { error_="Empty draw data"; return false; }
        if (times_.empty()) geometry_=geometry;
        else if (!(geometry==geometry_)) { error_="Geometry changed during measured samples; run is not comparable"; return false; }
        times_.push_back(cpu_us);
    }
    ++frame_;
    if (frame_==settings_.warmup+settings_.samples)
    {
        std::sort(times_.begin(),times_.end());
        const std::size_t count=times_.size();
        Result result;
        result.id=static_cast<ImGuiThemes::Id>(index_);
        result.median_us=count%2 ? times_[count/2] : (times_[count/2-1]+times_[count/2])*0.5;
        result.p95_us=times_[static_cast<std::size_t>(std::ceil(0.95*count))-1];
        result.geometry=geometry_; result.samples=static_cast<int>(count); result.submitted=items_;
        results_.push_back(result);
        ++index_; frame_=0; times_.clear();
    }
    return true;
}

namespace
{
// Visibility diagnostics run after each widget. The runner includes this identical
// instrumentation for every theme; it does not infer visibility from vertex counts.
void Track(Layout& layout)
{
    ++layout.submitted;
    const ImVec2 lo=ImGui::GetItemRectMin(), hi=ImGui::GetItemRectMax();
    const ImVec2 clip_lo=ImGui::GetWindowDrawList()->GetClipRectMin();
    const ImVec2 clip_hi=ImGui::GetWindowDrawList()->GetClipRectMax();
    if (ImGui::IsItemVisible() && lo.x>=clip_lo.x-0.5f && lo.y>=clip_lo.y-0.5f && hi.x<=clip_hi.x+0.5f && hi.y<=clip_hi.y+0.5f)
        ++layout.visible;
}
std::string Quote(const std::string& value)
{
    std::string out="\"";
    for (char c:value) { if (c=='\"') out+='\"'; out+=c; }
    return out+'\"';
}
}

Layout DrawScene(const ImGuiThemes::Theme& theme)
{
    Layout layout;
    const ImGuiViewport* viewport=ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos); ImGui::SetNextWindowSize(viewport->WorkSize);
    const ImGuiWindowFlags flags=ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoInputs;
    if (!ImGui::Begin("Fixed component workload",nullptr,flags))
    { ImGui::End(); layout.fits=false; return layout; }
    ImGui::TextUnformatted("UI component benchmark"); Track(layout);
    ImGui::TextUnformatted("Identical content, data, font and DPI. No editor or live statistics."); Track(layout);
    if (ImGui::BeginChild("component-preview",ImVec2(0,780),ImGuiChildFlags_Borders,ImGuiWindowFlags_NoInputs))
    {
        ImGui::SeparatorText("Actions");
        ImGuiThemes::Button(theme,ImGuiThemes::ButtonRole::Primary,"Apply",ImVec2(180,0)); Track(layout);
        ImGui::SameLine();
        ImGuiThemes::Button(theme,ImGuiThemes::ButtonRole::Secondary,"Reset",ImVec2(180,0)); Track(layout);
        ImGui::SameLine();
        ImGuiThemes::Button(theme,ImGuiThemes::ButtonRole::Danger,"Delete",ImVec2(180,0)); Track(layout);
        ImGui::BeginDisabled();
        ImGuiThemes::Button(theme,ImGuiThemes::ButtonRole::Secondary,"Disabled action",ImVec2(180,0)); Track(layout);
        ImGui::EndDisabled();
        bool live=true;
        ImGui::Checkbox("Live preview",&live); Track(layout);
        int quality=1;
        ImGui::SetNextItemWidth(500);
        ImGui::Combo("Quality",&quality,"Draft\0Balanced\0High\0"); Track(layout);
        float intensity=64;
        ImGui::SetNextItemWidth(500);
        ImGui::SliderFloat("Intensity",&intensity,0,100,"%.0f%%"); Track(layout);
        ImGui::ProgressBar(0.64f,ImVec2(600,0),"64%"); Track(layout);
        ImGui::SeparatorText("Properties");
        char name[32]="Preview profile";
        ImGui::SetNextItemWidth(500);
        ImGui::InputText("Name",name,sizeof(name)); Track(layout);
        float gain=1.0f;
        ImGui::SetNextItemWidth(220);
        ImGui::InputFloat("Gain",&gain,0.1f,1.0f,"%.2f"); Track(layout);
        ImGui::SeparatorText("Resource library");
        if (ImGui::BeginTable("benchmark-assets",3,ImGuiTableFlags_BordersInnerH|ImGuiTableFlags_RowBg))
        {
            ImGui::TableSetupColumn("Name"); ImGui::TableSetupColumn("Type"); ImGui::TableSetupColumn("Status");
            ImGui::TableHeadersRow();
            const char* rows[][3]={{"Environment","Scene","Ready"},{"Character","Mesh","Ready"},{"UI Atlas","Texture","Ready"},{"Stone surface","Material","Ready"}};
            for (const auto& row:rows)
            {
                ImGui::TableNextRow();
                for (const char* cell:row) { ImGui::TableNextColumn(); ImGui::TextUnformatted(cell); Track(layout); }
            }
            ImGui::EndTable();
        }
        else layout.fits=false;
        ImGui::TextUnformatted("All sample resources are loaded."); Track(layout);
        layout.fits=layout.fits && ImGui::GetScrollMaxY()<0.5f && ImGui::GetScrollMaxX()<0.5f;
    }
    else layout.fits=false;
    ImGui::EndChild();
    layout.fits=layout.fits && ImGui::GetScrollMaxY()<0.5f && ImGui::GetScrollMaxX()<0.5f;
    ImGui::End();
    return layout;
}

Geometry MeasureGeometry(const ImDrawData& data)
{
    Geometry geometry;
    geometry.vertices=data.TotalVtxCount; geometry.indices=data.TotalIdxCount;
    for (const ImDrawList* list:data.CmdLists)
        for (const ImDrawCmd& command:list->CmdBuffer)
        {
            const ImVec4& r=command.ClipRect;
            if (!command.UserCallback && command.ElemCount>0 && r.z>r.x && r.w>r.y
                && r.z>data.DisplayPos.x && r.w>data.DisplayPos.y
                && r.x<data.DisplayPos.x+data.DisplaySize.x && r.y<data.DisplayPos.y+data.DisplaySize.y)
                ++geometry.commands;
        }
    return geometry;
}

bool WriteCsv(const char* path, const Session& session, const Metadata& metadata, std::string& error)
{
    error.clear();
    if (!session.Done() || !session.Error().empty()) { error="Only a complete successful session can be exported"; return false; }
    const std::string temporary=std::string(path)+".tmp";
    std::ofstream out(temporary.c_str(),std::ios::trunc);
    if (!out) { error="Cannot open output: "+temporary; return false; }
    out.imbue(std::locale::classic()); out<<std::fixed<<std::setprecision(3);
    out<<"theme,ui_cpu_median_us,ui_cpu_p95_us,delta_median_us,delta_median_pct,vertices,indices,draw_commands,delta_vertices,delta_indices,delta_draw_commands,samples,warmup_frames,visible_items,window_width,window_height,framebuffer_width,framebuffer_height,font_size,ui_scale,vsync_requested,font,renderer,imgui_version,build_mode\n";
    const Result& baseline=session.Results().front();
    for (const Result& r:session.Results())
    {
        const double delta=r.median_us-baseline.median_us;
        out<<ImGuiThemes::Slug(r.id)<<','<<r.median_us<<','<<r.p95_us<<','<<delta<<','<<delta/baseline.median_us*100
           <<','<<r.geometry.vertices<<','<<r.geometry.indices<<','<<r.geometry.commands
           <<','<<r.geometry.vertices-baseline.geometry.vertices<<','<<r.geometry.indices-baseline.geometry.indices
           <<','<<r.geometry.commands-baseline.geometry.commands<<','<<r.samples<<','<<session.Config().warmup<<','<<r.submitted
           <<','<<metadata.window_width<<','<<metadata.window_height<<','<<metadata.framebuffer_width<<','<<metadata.framebuffer_height
           <<",17,1,0,"<<Quote(metadata.font)<<','<<Quote(metadata.renderer)<<','<<Quote(IMGUI_VERSION)<<','<<Quote(metadata.build)<<'\n';
    }
    out.flush(); bool ok=out.good(); out.close(); ok=ok && !out.fail();
    if (!ok || std::rename(temporary.c_str(),path)!=0)
    { std::remove(temporary.c_str()); error=std::string("Cannot replace output: ")+path; return false; }
    return true;
}
}
