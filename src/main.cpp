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

#include <graphics/Shader.h>
#include <graphics/VAO.h>
#include <graphics/EBO.h>
#include <graphics/VBO.h>

#define WIDTH 800
#define HEIGHT 800

int main(int argc, char* argv[]) {
    Window::initialize(WIDTH, HEIGHT, "Window 2.0");

    static float background_color_test[4]{0.f, 0.f, 0.f, 0.f};

    static const GLfloat points[]{
        0.f, 1.f, 0.f,
        1.f, -1.f, 0.f,
        -1.f, -1.f, 0.f,
    };

    static const GLfloat colors[]{
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
    };

    std::unique_ptr<Shader> shader(Shader::load_shader("resources/default.vert", "resources/default.frag"));
    shader->initialize();

    VBO points_vbo(points, sizeof(points), GL_STATIC_DRAW);
    VBO colors_vbo(colors, sizeof(colors), GL_STATIC_DRAW);

    VAO vao;
    vao.Bind();
    vao.LinkAttrib(points_vbo, 0, 3, GL_FLOAT, 0, nullptr);
    vao.LinkAttrib(colors_vbo, 1, 3, GL_FLOAT, 0, nullptr);

    while (!Window::is_shouldClose()) {
        Window::update([&shader, &vao]() {
            ImGui::Begin("test color");
            ImGui::ColorEdit4("background", background_color_test);
            ImGui::NewLine();
            ImGui::Text("mouse pos: %dx%d", Events::get_mousePosX(), Events::get_mousePosY());
            ImGui::Text("window size: %dx%d", Window::get_width(), Window::get_height());
            ImGui::End();

            glUseProgram(shader->get_id());
            glBindVertexArray(vao.get_id());
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glClearColor(background_color_test[0], background_color_test[1], background_color_test[2], background_color_test[3]);
        });
    } Window::finalize();

    return 0;
}
