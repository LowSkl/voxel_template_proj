#pragma once

#include <string>
#include "Events.h"

struct GLFWwindow;
class Window : public Events {
public:
	Window(int width, int height, std::string title);
	virtual ~Window();

	GLFWwindow* get_window() const { return this->m_pWindow; }

	int get_width() const { return this->m_data.width; }
	int get_height() const { return this->m_data.width; }

	std::string get_title() const { return this->m_data.title; }

	bool is_shouldClose();
	void w_update();

private:
	int w_initialize();
	void w_finalize();

	struct WindowData {
		int width;
		int height;
		std::string title;
	} m_data;

	GLFWwindow* m_pWindow;
};

