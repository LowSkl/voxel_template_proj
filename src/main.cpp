#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utils/Log.h>
#include <window/Window.h>
#include <window/Events.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define WIDTH 800
#define HEIGHT 800

int main(int argc, char* argv[]) {
    Window::initialize(WIDTH, HEIGHT, "Window 2.0");

    static float background_color_test[4]{0.f, 0.f, 0.f, 0.f};

    ImGuiIO& io = ImGui::GetIO();
    while (!Window::is_shouldClose()) {
        Window::update([]() {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(Window::get_width(), Window::get_height());

            ImGui::Begin("test color");
            ImGui::ColorEdit4("background", background_color_test);
            ImGui::End();

            glClearColor(background_color_test[0], background_color_test[1], background_color_test[2], background_color_test[3]);
        });
    } Window::finalize();

    return 0;
}
