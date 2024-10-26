#pragma once

struct GLFWwindow;
struct ImGuiIO;

class Window {
	static GLFWwindow* m_pWindow;

	static unsigned int m_width;
	static unsigned int m_height;

public:
	static int initialize(unsigned int width, unsigned int height, const char* title);
	static void finalize();
	static void update(void (*between)());

	static GLFWwindow* get_window() { return Window::m_pWindow; }

	static bool is_shouldClose();
	static void set_shouldClose(bool flag);

	static unsigned int get_width() { return Window::m_width; }
	static unsigned int get_height() { return Window::m_height; }

	static void set_width(unsigned int width) { Window::m_width = width; }
	static void set_height(unsigned int height) { Window::m_height = height; }
};