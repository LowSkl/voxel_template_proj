#include "UI.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include <window/Window.h>

void UI::initialize() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(Window::get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    Window::set_io(&ImGui::GetIO());

    //reserved
    /*Window::get_io()->ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
    Window::get_io()->ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;*/

    Window::get_io()->IniFilename = nullptr;
    Window::get_io()->LogFilename = nullptr;

    Window::get_io()->DisplaySize = ImVec2(Window::get_width(), Window::get_height());
}

void UI::update_begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::update_end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //reserved
    /*if (Window::get_io()->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }*/
}

void UI::finalize() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}