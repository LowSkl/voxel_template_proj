#include "Window.h"

#include <utils/Log.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Window::Window(int width, int height, std::string title) :
	m_data
	({
		width,
		height,
		title
	})
{
	LINFO("Init window, code {0}", this->w_initialize());
}

int Window::w_initialize() {
	LINFO("Window::w_initialize()");

	glfwSetErrorCallback([](int code, const char* description) {
		LCRITICAL("Error {0}", code);
		LCRITICAL(description);
	});

	if (!glfwInit()) {
		LCRITICAL("Can't init glfw");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	this->m_pWindow = glfwCreateWindow(this->m_data.width, this->m_data.height, this->m_data.title.c_str(), nullptr, nullptr);
	if (this->m_pWindow == nullptr) {
		LCRITICAL("Can't init window");
		glfwTerminate();
		return -2;
	} glfwMakeContextCurrent(this->m_pWindow);
	this->e_initialize(this->m_pWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		LCRITICAL("Can't init glew");
		return -3;
	}

	glfwGetFramebufferSize(this->m_pWindow, &this->m_data.width, &this->m_data.height);
	glViewport(0, 0, this->m_data.width, this->m_data.height);

	return 0;
}

void Window::w_update() {
	//LINFO("Window::w_update()");
	glfwSwapBuffers(this->m_pWindow);
}

Window::~Window() {
	this->w_finalize();
}

void Window::w_finalize() {
	LINFO("Kill window");

	glfwDestroyWindow(this->m_pWindow);
	glfwTerminate();
}

bool Window::is_shouldClose() {
	return glfwWindowShouldClose(this->m_pWindow);
}