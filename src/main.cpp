#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utils/Log.h>
#include <window/Window.h>
#include <window/Events.h>

#define WIDTH 1024
#define HEIGHT 768

const char* vertexShaderSource{
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n\0"
};

const char* fragmentShaderSource{
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0"
};

int main(int argc, char* argv[]) {
    Window::initialize(WIDTH, HEIGHT, "Window 2.0");
    Events::initialize();
    
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };

    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER); {
        glShaderSource(vertexShaderID, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShaderID);
    }

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER); {
        glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShaderID);
    }

    GLuint shaderProgramID = glCreateProgram(); {
        glAttachShader(shaderProgramID, vertexShaderID);
        glAttachShader(shaderProgramID, fragmentShaderID);
        glLinkProgram(shaderProgramID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!Window::is_shouldClose()) {
        Events::update();

        glClearColor(.3f, .4f, .5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgramID);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        Window::update();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgramID);

    Window::finalize();
    return 0;
}
