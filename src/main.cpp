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

#include <imgui_internal.h>

#include <graphics/Shader.h>
#include <graphics/buffers/VAO.h>
#include <graphics/buffers/EBO.h>
#include <graphics/buffers/VBO.h>

#include <math.h>

#define WIDTH 800
#define HEIGHT 800

int main(int argc, char* argv[]) {
    Window::initialize(WIDTH, HEIGHT, "Window 2.0");

    static float background_color_test[4]{0.f, 0.f, 0.f, 0.f};

    static const GLfloat verts[]{
        //               COORDINATES                /     COLORS     //
        -0.5f,  -0.5f * (float)sqrt(3) / 3,     1.f,  0.8f, 0.3f,  0.02f, // Lower left corner
        0.5f,   -0.5f * (float)sqrt(3) / 3,     1.f,  0.8f, 0.3f,  0.02f, // Lower right corner
        0.f,     0.5f * (float)sqrt(3) * 2 / 3, 0.f,  1.f,  0.6f,  0.32f, // Upper corner
        -0.25f,  0.5f * (float)sqrt(3) / 6,     0.5f, 0.9f, 0.45f, 0.17f, // Inner left
        0.25f,   0.5f * (float)sqrt(3) / 6,     0.5f, 0.9f, 0.45f, 0.17f, // Inner right
        0.f,    -0.5f * (float)sqrt(3) / 3,     1.f,  0.8f, 0.3f,  0.02f, // Inner down
    };

    static const GLuint indices[]{
        0, 3, 5,
        3, 2, 4,
        5, 4, 1
    };

    std::unique_ptr<Shader> shader(Shader::load_shader("resources/default.vert", "resources/default.frag"));

    VAO vao;
    vao.bind();

    VBO vbo(verts, sizeof(verts), GL_STATIC_DRAW);
    EBO ebo(indices, sizeof(indices), GL_STATIC_DRAW);

    vao.linkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.linkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    GLuint uniID1 = glGetUniformLocation(shader->get_id(), "scale");
    GLuint uniID2 = glGetUniformLocation(shader->get_id(), "addVec");

    float scale = 1.f;
    glm::vec3 addVec{ 0.f, 0.f, 0.f };

    while (!Window::is_shouldClose()) {
        Window::update([&]() {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                ImGui::Begin("test color");
                ImGui::ColorEdit4("background", background_color_test);
                ImGui::NewLine();
                ImGui::Text("mouse pos: %dx%d", Events::get_mousePosX(), Events::get_mousePosY());
                ImGui::Text("window size: %dx%d", Window::get_width(), Window::get_height());
                ImGui::NewLine();
                ImGui::SliderFloat("scale", &scale, -5.f, 5.f);
                ImGui::SliderFloat("add x", &addVec.x, -10.f, 10.f);
                ImGui::SliderFloat("add y", &addVec.y, -10.f, 10.f);
                ImGui::SliderFloat("add z", &addVec.z, -10.f, 10.f);
                ImGui::End();

                shader->initialize();

                glUniform1f(uniID1, scale);
                glUniform3f(uniID2, addVec.x, addVec.y, addVec.z);

                vao.bind();

                glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
                glClearColor(background_color_test[0], background_color_test[1], background_color_test[2], background_color_test[3]);
            }
        );
    } Window::finalize();

    return 0;
}
