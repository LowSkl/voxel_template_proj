#include <window/Window.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <utils/Log.h>

unsigned short UI::m_uisCount = 0;

UI::UI(Window* pWindow) : 
    m_pWindow(pWindow), m_pContext(nullptr), m_count(++m_uisCount)
{
    LINFO("[UI {}]\t\tUI::UI(\"{}\")", this->m_count, pWindow->get_title()->c_str());

    int code = this->initialize();
    LINFO("[UI {}]\t\tinit code {} ({})", this->m_count, code, UI::returnsToString(code));
}

int UI::initialize()
{
    LINFO("[UI {}]\t\tUI::initialize()", this->m_count);

    if (UI::m_uisCount == 1) IMGUI_CHECKVERSION(); 

    this->m_pWindow->make_contextActive();

    this->m_pContext = ImGui::CreateContext();
    this->make_contextActive();

    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    io.DisplaySize = ImVec2(this->m_pWindow->get_width(), this->m_pWindow->get_height());

    ImGui_ImplGlfw_InitForOpenGL(this->m_pWindow->get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    return UI::Returns::NO_ERRORS;
}

void UI::updateBegin()
{
    this->m_pWindow->make_contextActive();
    this->make_contextActive();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
}

void UI::updateEnd()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::make_contextActive()
{
    ImGui::SetCurrentContext(this->m_pContext);
}

void UI::finalize()
{
    this->make_contextActive();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
    UI::m_uisCount--;

    LINFO("[UI {}]\t\tDestroyed", this->m_count);
}

UI::~UI() 
{ 
    LINFO("[UI {}]\t\tUI::~UI()", this->m_count);
    this->finalize();
}