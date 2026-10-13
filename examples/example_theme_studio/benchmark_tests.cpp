#include "benchmark.h"
#include "imgui_impl_null.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>

static void Require(bool ok,const char* message) { if (!ok) throw std::runtime_error(message); }
static ThemeBenchmark::Geometry Geometry()
{ ThemeBenchmark::Geometry g; g.vertices=100; g.indices=150; g.commands=2; return g; }
static ThemeBenchmark::Layout Layout()
{ ThemeBenchmark::Layout l; l.submitted=8; l.visible=8; return l; }

static void SamplingAndExport()
{
    ThemeBenchmark::Settings config; config.warmup=2; config.samples=4;
    ThemeBenchmark::Session session(config);
    ThemeBenchmark::Metadata metadata; metadata.font="font,quoted\".ttf"; metadata.renderer="test"; metadata.build="test";
    std::string error;
    Require(!ThemeBenchmark::WriteCsv("incomplete.csv",session,metadata,error),"partial sessions cannot be exported");
    for (int theme=0;theme<ImGuiThemes::ThemeCount;++theme)
    {
        Require(session.ThemeIndex()==theme,"sample schedule must advance once per complete theme");
        Require(session.Record(9000,Geometry(),Layout()) && session.Record(8000,Geometry(),Layout()),"warmup must be accepted");
        const double times[]={10,20,30,40};
        for (double t:times) Require(session.Record(t+theme,Geometry(),Layout()),"measured sample must be accepted");
        const ThemeBenchmark::Result& result=session.Results().back();
        Require(result.samples==4,"sample count excludes warmup");
        Require(result.median_us==25+theme && result.p95_us==40+theme,"median and nearest-rank P95 must exclude warmup");
    }
    Require(session.Done(),"all themes must complete");
    Require(ThemeBenchmark::WriteCsv("benchmark-test.csv",session,metadata,error),"complete session must export");
    std::ifstream input("benchmark-test.csv"); std::stringstream contents; contents<<input.rdbuf();
    Require(contents.str().find("default,25.000,40.000,0.000,0.000")!=std::string::npos,"baseline deltas must be zero");
    Require(contents.str().find("cyberpunk,26.000,41.000,1.000,4.000")!=std::string::npos,"relative median must use baseline");
    Require(contents.str().find("\"font,quoted\"\".ttf\"")!=std::string::npos,"CSV metadata must be escaped");
    std::remove("benchmark-test.csv");
}
static void InvalidMeasurementsAreRejected()
{
    ThemeBenchmark::Settings config; config.warmup=1; config.samples=2;
    ThemeBenchmark::Session clipped(config);
    Require(clipped.Record(10,Geometry(),Layout()),"warmup succeeds");
    ThemeBenchmark::Layout bad=Layout(); --bad.visible;
    Require(!clipped.Record(10,Geometry(),bad) && !clipped.Error().empty(),"clipped samples invalidate a run");
    ThemeBenchmark::Session changing(config);
    Require(changing.Record(10,Geometry(),Layout()) && changing.Record(10,Geometry(),Layout()),"initial samples succeed");
    ThemeBenchmark::Geometry g=Geometry(); ++g.vertices;
    Require(!changing.Record(10,g,Layout()),"changing measured geometry must be rejected");
    ThemeBenchmark::Session nan(config);
    Require(!nan.Record(std::numeric_limits<double>::quiet_NaN(),Geometry(),Layout()),"invalid timings must fail");
    config.samples=0; bool threw=false;
    try { ThemeBenchmark::Session invalid(config); } catch (const std::invalid_argument&) { threw=true; }
    Require(threw,"invalid sample count must fail before running");
}
static void SceneIsComparable()
{
    ImGui::CreateContext(); ImGui::GetIO().IniFilename=nullptr;
    ImGui::GetStyle().FontSizeBase=17;
    ImGui_ImplNull_Init();
    int items=-1;
    for (int t=0;t<ImGuiThemes::ThemeCount;++t)
    {
        const ImGuiThemes::Theme theme=ImGuiThemes::Make(static_cast<ImGuiThemes::Id>(t));
        Require(ImGuiThemes::Apply(theme,1),"apply comparison theme");
        ThemeBenchmark::Layout layout;
        for (int frame=0;frame<4;++frame)
        {
            ImGui_ImplNull_NewFrame(); ImGui::GetIO().DisplaySize=ImVec2(1440,960); ImGui::NewFrame();
            layout=ThemeBenchmark::DrawScene(theme);
            ImGui::Render(); ImGui_ImplNullRender_RenderDrawData(ImGui::GetDrawData());
        }
        Require(layout.submitted>0 && layout.visible==layout.submitted && layout.fits,"entire fixed workload must be visible for every theme");
        if (items<0) items=layout.submitted;
        Require(layout.submitted==items,"all themes must submit identical widget count");
        const ThemeBenchmark::Geometry g=ThemeBenchmark::MeasureGeometry(*ImGui::GetDrawData());
        Require(g.vertices>0 && g.commands>0,"scene must produce geometry");
    }
    // Oversized custom spacing must fail visibility checks rather than look artificially cheap.
    ImGuiThemes::Theme large=ImGuiThemes::Make(ImGuiThemes::Id::Bento);
    large.style.FramePadding.y=40; large.style.CellPadding.y=40;
    Require(ImGuiThemes::Apply(large),"apply oversized valid configuration");
    ThemeBenchmark::Layout layout;
    for (int frame=0;frame<3;++frame)
    {
        ImGui_ImplNull_NewFrame(); ImGui::GetIO().DisplaySize=ImVec2(1440,960); ImGui::NewFrame();
        layout=ThemeBenchmark::DrawScene(large); ImGui::Render();
        ImGui_ImplNullRender_RenderDrawData(ImGui::GetDrawData());
    }
    Require(!layout.fits || layout.visible!=layout.submitted,"clipped oversized configurations must be detected");
    ImGui_ImplNull_Shutdown(); ImGui::DestroyContext();
}
int main()
{
    try { SamplingAndExport(); InvalidMeasurementsAreRejected(); SceneIsComparable(); }
    catch(const std::exception& e) { std::fprintf(stderr,"FAIL: %s\n",e.what()); return 1; }
    std::puts("PASS: warmup/statistics/export contracts and fully visible equal workload");
    return 0;
}
