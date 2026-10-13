#include "studio.h"
#include "benchmark.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cfloat>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

static void Error(int code,const char* message) { std::fprintf(stderr,"GLFW %d: %s\n",code,message); }
static bool Capture(const std::string& path,int width,int height)
{
    if (width<=0 || height<=0) return false;
    std::vector<unsigned char> pixels(static_cast<std::size_t>(width)*height*3);
    glPixelStorei(GL_PACK_ALIGNMENT,1); glReadBuffer(GL_BACK);
    glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,pixels.data());
    std::ofstream out(path.c_str(),std::ios::binary);
    out << "P6\n" << width << ' ' << height << "\n255\n";
    for (int y=height-1;y>=0;--y) out.write(reinterpret_cast<const char*>(pixels.data()+y*width*3),width*3);
    out.close(); return !out.fail();
}

static int Compare(GLFWwindow* window,ThemeStudio::State& state,const ThemeBenchmark::Settings& settings,const std::string& output,const std::string& font)
{
    if (state.error) { std::fprintf(stderr,"Benchmark requires valid custom preset files.\n"); return 1; }
    ThemeBenchmark::Session session(settings);
    ThemeBenchmark::Metadata metadata;
    glfwGetWindowSize(window,&metadata.window_width,&metadata.window_height);
    glfwGetFramebufferSize(window,&metadata.framebuffer_width,&metadata.framebuffer_height);
    if (metadata.window_width!=1440 || metadata.window_height!=960)
    { std::fprintf(stderr,"Benchmark requires a 1440x960 logical window.\n"); return 1; }
    metadata.font=font; metadata.renderer=reinterpret_cast<const char*>(glGetString(GL_RENDERER));
#ifdef NDEBUG
    metadata.build="NDEBUG";
#else
    metadata.build="assertions enabled";
    std::fprintf(stderr,"Note: use a Release build for representative CPU measurements.\n");
#endif
    std::printf("Benchmark: %d warmup + %d samples per theme; UI CPU only; VSync requested off.\n",settings.warmup,settings.samples);
    state.scale=1;
    ImGuiIO& io=ImGui::GetIO();
    io.ConfigFlags&=~ImGuiConfigFlags_NavEnableKeyboard;
    while (!session.Done() && !glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        state.selected=session.ThemeIndex();
        ThemeStudio::ApplyPending(state);
        if (state.error || state.applied!=state.selected)
        { std::fprintf(stderr,"Cannot apply comparison theme.\n"); return 1; }
        ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame();
        int width,height,window_width,window_height;
        glfwGetFramebufferSize(window,&width,&height); glfwGetWindowSize(window,&window_width,&window_height);
        if (width!=metadata.framebuffer_width || height!=metadata.framebuffer_height
            || window_width!=metadata.window_width || window_height!=metadata.window_height)
        { std::fprintf(stderr,"Window/framebuffer changed during comparison; no CSV exported.\n"); return 1; }
        io.DeltaTime=1.0f/60.0f;
        io.AddMousePosEvent(-FLT_MAX,-FLT_MAX);
        const auto begin=std::chrono::steady_clock::now();
        ImGui::NewFrame();
        const ThemeBenchmark::Layout layout=ThemeBenchmark::DrawScene(state.themes[state.applied]);
        ImGui::Render();
        const auto end=std::chrono::steady_clock::now();
        const double cpu_us=std::chrono::duration<double,std::micro>(end-begin).count();
        const ImDrawData* draw=ImGui::GetDrawData();
        if (!session.Record(cpu_us,ThemeBenchmark::MeasureGeometry(*draw),layout))
        { std::fprintf(stderr,"Benchmark rejected: %s\n",session.Error().c_str()); return 1; }
        glViewport(0,0,width,height);
        const ImVec4 bg=ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
        glClearColor(bg.x,bg.y,bg.z,1); glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    std::string error;
    if (!ThemeBenchmark::WriteCsv(output.c_str(),session,metadata,error))
    { std::fprintf(stderr,"Benchmark export failed: %s\n",error.c_str()); return 1; }
    const double baseline=session.Results().front().median_us;
    for (const ThemeBenchmark::Result& result:session.Results())
        std::printf("%-20s median %8.3f us  p95 %8.3f us  delta %+6.1f%%  vertices %d  indices %d  commands %d\n",
            ImGuiThemes::Name(result.id),result.median_us,result.p95_us,(result.median_us/baseline-1)*100,
            result.geometry.vertices,result.geometry.indices,result.geometry.commands);
    std::printf("CSV: %s\n",output.c_str());
    return 0;
}

int main(int argc,char** argv)
{
    std::string directory=THEME_STUDIO_PRESET_DIR, capture_directory, font=THEME_STUDIO_FONT_PATH, benchmark_file;
    ThemeBenchmark::Settings settings;
    bool sample_options=false, theme_option=false;
    int selected=static_cast<int>(ImGuiThemes::Id::Bento), frames=0;
    for (int i=1;i<argc;++i)
    {
        const std::string arg=argv[i];
        if ((arg=="--themes" || arg=="--capture-dir" || arg=="--font" || arg=="--theme" || arg=="--frames"
            || arg=="--benchmark" || arg=="--warmup" || arg=="--samples") && i+1<argc)
        {
            const char* value=argv[++i];
            if (arg=="--themes") directory=value;
            else if (arg=="--capture-dir") capture_directory=value;
            else if (arg=="--font") font=value;
            else if (arg=="--benchmark") benchmark_file=value;
            else if (arg=="--frames" || arg=="--warmup" || arg=="--samples")
            {
                char* end=nullptr; long n=std::strtol(value,&end,10);
                const int lower=arg=="--samples"?2:1;
                const int upper=arg=="--warmup"?100000:1000000;
                if (end==value || *end || n<lower || n>upper)
                { std::fprintf(stderr,"Invalid count for %s\n",arg.c_str()); return 1; }
                if (arg=="--frames") frames=static_cast<int>(n);
                else if (arg=="--warmup") { settings.warmup=static_cast<int>(n); sample_options=true; }
                else { settings.samples=static_cast<int>(n); sample_options=true; }
            }
            else
            {
                selected=-1;
                theme_option=true;
                for (int t=0;t<ImGuiThemes::ThemeCount;++t) if (std::strcmp(value,ImGuiThemes::Slug(static_cast<ImGuiThemes::Id>(t)))==0) selected=t;
                if (selected<0) { std::fprintf(stderr,"Unknown theme: %s\n",value); return 1; }
            }
        }
        else
        {
            std::printf("Usage: theme_studio [--theme default|cyberpunk|minimal|wireframe|brutalist|bento] [--themes DIRECTORY] [--font TTF] [--frames N] [--capture-dir EXISTING_DIRECTORY]\n"
                        "       theme_studio --benchmark RESULTS.csv [--warmup 120] [--samples 600] [--themes DIRECTORY] [--font TTF]\n");
            return arg=="--help"?0:1;
        }
    }
    if ((!benchmark_file.empty() && (!capture_directory.empty() || frames!=0 || theme_option))
        || (benchmark_file.empty() && sample_options))
    { std::fprintf(stderr,"Benchmark compares all themes; do not combine it with --theme, --frames or --capture-dir. Sample options require --benchmark.\n"); return 1; }
    glfwSetErrorCallback(Error);
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    const char* glsl="#version 150";
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,0);
    const char* glsl="#version 130";
#endif
    if (!capture_directory.empty() || !benchmark_file.empty()) glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    if (!benchmark_file.empty()) glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
    GLFWwindow* window=glfwCreateWindow(1440,960,"ImGui Theme Studio",nullptr,nullptr);
    if (!window) { glfwTerminate(); return 1; }
    glfwSetWindowSizeLimits(window,1100,720,GLFW_DONT_CARE,GLFW_DONT_CARE);
    glfwMakeContextCurrent(window); glfwSwapInterval(capture_directory.empty() && benchmark_file.empty()?1:0);
    IMGUI_CHECKVERSION(); ImGui::CreateContext();
    ImGuiIO& io=ImGui::GetIO(); io.IniFilename=nullptr; io.ConfigFlags|=ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetStyle().FontSizeBase=17;
    if (!font.empty())
    {
        std::ifstream check(font.c_str(),std::ios::binary);
        if (!check || !io.Fonts->AddFontFromFileTTF(font.c_str(),17))
        {
            std::fprintf(stderr,"Cannot load font: %s\n",font.c_str());
            ImGui::DestroyContext(); glfwDestroyWindow(window); glfwTerminate(); return 1;
        }
    }
    if (!ImGui_ImplGlfw_InitForOpenGL(window,benchmark_file.empty()))
    { ImGui::DestroyContext(); glfwDestroyWindow(window); glfwTerminate(); return 1; }
    if (!ImGui_ImplOpenGL3_Init(glsl))
    { ImGui_ImplGlfw_Shutdown(); ImGui::DestroyContext(); glfwDestroyWindow(window); glfwTerminate(); return 1; }
    ThemeStudio::State state; state.selected=selected;
    ThemeStudio::LoadPresets(state,directory);
    if (state.error) std::fprintf(stderr,"Theme config: %s\n",state.message.c_str());
    std::printf("Renderer: %s\n",reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    int frame=0, result=0;
    if (!benchmark_file.empty()) result=Compare(window,state,settings,benchmark_file,font);
    if (!capture_directory.empty()) frames=ImGuiThemes::ThemeCount*8;
    while (benchmark_file.empty() && !glfwWindowShouldClose(window) && (frames==0 || frame<frames))
    {
        glfwPollEvents();
        if (!capture_directory.empty()) state.selected=frame/8;
        ThemeStudio::ApplyPending(state);
        ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); ImGui::NewFrame();
        ThemeStudio::Draw(state); ImGui::Render();
        int width=0,height=0; glfwGetFramebufferSize(window,&width,&height);
        glViewport(0,0,width,height);
        const ImVec4 bg=ImGui::GetStyle().Colors[ImGuiCol_WindowBg]; glClearColor(bg.x,bg.y,bg.z,1); glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (!capture_directory.empty() && frame%8==7)
        {
            const std::string path=capture_directory+"/"+ImGuiThemes::Slug(static_cast<ImGuiThemes::Id>(state.selected))+".ppm";
            if (!Capture(path,width,height)) { std::fprintf(stderr,"Capture failed: %s\n",path.c_str()); result=1; break; }
            std::printf("Captured %s (%d x %d)\n",path.c_str(),width,height);
        }
        glfwSwapBuffers(window); ++frame;
    }
    ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplGlfw_Shutdown(); ImGui::DestroyContext();
    glfwDestroyWindow(window); glfwTerminate();
    return result;
}
