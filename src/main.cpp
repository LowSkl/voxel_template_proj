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

const char* vertex_shader{
    "#version 460\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_color;"
    "out vec3 color;"
    "void main() {"
    "   color = vertex_color;"
    "   gl_Position = vec4(vertex_position, 1.0);"
    "}"
};

const char* fragment_shader{
    "#version 460\n"
    "in vec3 color;"
    "out vec4 frag_color;"
    "void main() {"
    "   frag_color = vec4(color, 1.0);"
    "}"
};

GLuint shaderProgram, VAO;
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

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER); {
        glShaderSource(vertex, 1, &vertex_shader, nullptr);
        glCompileShader(vertex);
    }

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER); {
        glShaderSource(fragment, 1, &fragment_shader, nullptr);
        glCompileShader(fragment);
    }

    shaderProgram = glCreateProgram(); {
        glAttachShader(shaderProgram, vertex);
        glAttachShader(shaderProgram, fragment);

        glLinkProgram(shaderProgram);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    GLuint points_vbo = 0; {
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    }

    GLuint colors_vbo = 0; {
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    ImGuiIO& io = ImGui::GetIO();
    while (!Window::is_shouldClose()) {
        Window::update([]() {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(Window::get_width(), Window::get_height());

            ImGui::Begin("test color");
            ImGui::ColorEdit4("background", background_color_test);
            ImGui::End();

            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glClearColor(background_color_test[0], background_color_test[1], background_color_test[2], background_color_test[3]);
        });
    } Window::finalize();

    return 0;
}
