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
#include <graphics/Texture.h>

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
        //     COORDINATES     /        COLORS      /   TexCoord  //
           -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
           -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
            0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
            0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
    };

    static const GLuint indices[]{
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
    };

    std::unique_ptr<Shader> shader(Shader::load_shader("resources/shaders/default.vert", "resources/shaders/default.frag"));
    std::unique_ptr<Texture> texture(Texture::load_texture("resources/textures/pop_cat.png", GL_TEXTURE_2D, -1, GL_RGBA, GL_UNSIGNED_BYTE));

    VAO vao(true);
    VBO vbo(verts, sizeof(verts), GL_STATIC_DRAW, false);
    EBO ebo(indices, sizeof(indices), GL_STATIC_DRAW, false);

    vao.linkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    vao.linkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    vao.linkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    GLuint uniID1 = glGetUniformLocation(shader->get_id(), "scale");
    GLuint uniID2 = glGetUniformLocation(shader->get_id(), "addVec");
    GLuint uniID3 = glGetUniformLocation(shader->get_id(), "tex0");

    float scale = 1.f;
    glm::vec3 addVec{ 0.f, 0.f, 0.f };

    while (!Window::is_shouldClose()) {
        Window::update([&]() {
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
                glUniform1i(uniID3, 0);

                texture->bind();
                vao.bind();

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glClearColor(background_color_test[0], background_color_test[1], background_color_test[2], background_color_test[3]);
            }
        );
    } Window::finalize();

    return 0;
}
