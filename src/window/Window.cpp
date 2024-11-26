#include "Window.h"

#include <GLFW/glfw3.h>
#include <utils/Log.h>

unsigned short Window::m_windowsCount = 0;

Window::Window(const std::string title, const unsigned short width, const unsigned short height) :
	m_title(title), m_width(width), m_height(height), m_count(++Window::m_windowsCount)
{
	LINFO("[Window {}]\tWindow::Window(\"{}\", {}, {})", this->m_count, this->m_title, this->m_width, this->m_height);

	int code = this->initialize();
	LINFO("[Window {}]\tinit code {} ({})", this->m_count, code, Window::returnsToString(code));
}

int Window::initialize()
{
	LINFO("[Window {}]\tWindow::initialize()", this->m_count);

	if (Window::m_windowsCount == 1)
	{
		LINFO("First window, initializing GLFW");

		glfwSetErrorCallback(
			[](int error_code, const char* description) {
				LCRITICAL("GLFW error {}: {}", error_code, description);
			}
		);

		if (!glfwInit()) {
			LCRITICAL("[Window {}]\tCan't init GLFW", this->m_count);
			return Window::Returns::CANT_INIT_GLFW;
		}
	}

	if (Window::m_windowsCount > 1) glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);

	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // убирает кнопки сверху
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	if ((this->m_pWindow = glfwCreateWindow(this->m_width, this->m_height, this->m_title.c_str(), nullptr, nullptr)) == nullptr) {
		LCRITICAL("[Window {}]\tCan't init window", this->m_count);
		return Window::Returns::CANT_INIT_WINDOW;
	} this->make_contextActive();

	glfwSetWindowUserPointer(this->m_pWindow, this);

	this->m_render = std::make_shared<RenderOpenGL>(this);
	this->m_ui     = std::make_shared<UI          >(this);
	this->m_input  = std::make_shared<Input       >(this);

	return Window::Returns::NO_ERRORS;
}

void Window::update() const
{
	glfwSwapBuffers(this->m_pWindow);
}

void Window::set_cursorMode(int flag) const { glfwSetInputMode(this->m_pWindow, GLFW_CURSOR, flag); }

void Window::make_contextActive()
{ 
	glfwMakeContextCurrent(this->m_pWindow);
}

void Window::set_width(unsigned short width)
{ 
	this->m_width = width;
	glfwSetWindowSize(this->m_pWindow, width, this->m_height);
}

void Window::set_height(unsigned short height)
{ 
	this->m_height = height;
	glfwSetWindowSize(this->m_pWindow, this->m_width, height);
}

void Window::set_size(unsigned short width, unsigned short height)
{
	this->m_width = width;
	this->m_height = height;
	glfwSetWindowSize(this->m_pWindow, width, height);
}

void Window::toggleCursor()
{
	this->m_cursorLocked = !this->m_cursorLocked;
	this->set_cursorMode(this->m_cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool Window::is_maximized() const
{
	return glfwGetWindowAttrib(this->m_pWindow, GLFW_MAXIMIZED);
}

bool Window::is_iconified() const
{
	return glfwGetWindowAttrib(this->m_pWindow, GLFW_ICONIFIED);
}

bool Window::is_focused() const
{
	return glfwGetWindowAttrib(this->m_pWindow, GLFW_FOCUSED);
}

glm::vec2 Window::get_pos() const
{
	int x, y;
	glfwGetWindowPos(this->m_pWindow, &x, &y);
	return glm::vec2(x, y);
}

void Window::finalize()
{
	glfwDestroyWindow(this->m_pWindow);

	this->m_input = nullptr;
	this->m_ui    = nullptr;

	if (Window::m_windowsCount-- == 1) {
		LINFO("Last window, terminating GLFW");
		glfwTerminate();
		return;
	} LINFO("[Window {}]\tDestroyed", this->m_count);
}

Window::~Window()
{
	LINFO("[Window {}]\tWindow::~Window()", this->m_count);
	this->finalize();
}
