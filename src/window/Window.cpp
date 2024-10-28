#include "Window.h"

#include <utils/Log.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <window/Events.h>
#include <modules/UI.h>

GLFWwindow* Window::m_pWindow = nullptr;

unsigned int Window::m_width = 0;
unsigned int Window::m_height = 0;

ImGuiIO* Window::io = nullptr;

int Window::initialize(unsigned int width, unsigned int height, const char* title) {
    LINFO("Initializing window {0}x{1}, {2}", width, height, title);
    if (glfwInit() != GL_TRUE) {
        LCRITICAL("Can't init glfw");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    if ((Window::m_pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr)) == nullptr) {
        fprintf(stderr, "Can't create window\n");
        glfwTerminate();
        return -2;
    } glfwMakeContextCurrent(Window::m_pWindow);

    gladLoadGL();
    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Events::initialize();

    Window::m_width = width;
    Window::m_height = height;

    UI::initialize();

    return 0;
}

void Window::update(std::function<void()> between) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Events::update();

    UI::update_begin();
    between();
    UI::update_end();

    glfwSwapBuffers(Window::m_pWindow);
}

void Window::finalize() {
    LINFO("Killing window");
    UI::finalize();

    glfwDestroyWindow(Window::m_pWindow);
    glfwTerminate();
}

bool Window::is_shouldClose() {
    return glfwWindowShouldClose(Window::m_pWindow);
}

void Window::set_shouldClose(bool flag) {
    glfwSetWindowShouldClose(Window::m_pWindow, flag);
}