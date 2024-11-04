#pragma once

#include <iostream>
#include <string>
#include <functional>

#include "Input.h"
#include "RenderOpenGL.h"
#include <modules/UI.h>

struct GLFWwindow;

class Window
{
	static unsigned short m_windowsCount;

	GLFWwindow* m_pWindow       = nullptr;

	bool m_cursorLocked   = false;
	bool m_is_shouldClose = false;

	std::string m_title = nullptr;

	std::shared_ptr<RenderOpenGL> m_render = nullptr;
	std::shared_ptr<Input>        m_input  = nullptr;
	std::shared_ptr<UI>           m_ui     = nullptr;

	unsigned short m_width  = 0;
	unsigned short m_height = 0;
	unsigned short m_count  = 0;

	int initialize();
	void finalize();

public:
	enum Returns
	{
		NO_ERRORS        =  0,
		CANT_INIT_GLFW   = -1,
		CANT_INIT_WINDOW = -2,
	};

	static constexpr std::string returnsToString(int code)
	{
		switch (code)
		{
		case Window::Returns::NO_ERRORS:
			return "NO_ERRORS";
			break;
		case Window::Returns::CANT_INIT_GLFW:
			return "CANT_INIT_GLFW";
			break;
		case Window::Returns::CANT_INIT_WINDOW:
			return "CANT_INIT_WINDOW";
			break;
		} return "UNKNOWN_TYPE";
	}

	Window(const std::string title, const unsigned short width, const unsigned short height);
	~Window();

	Window(const Window& ) = delete;
	Window(      Window&&) = delete;

	Window& operator=(const Window& ) = delete;
	Window& operator=(      Window&&) = delete;

	void update() const;

	bool is_shouldClose() const { return this->m_is_shouldClose; };
	void set_shouldClose(bool flag) { this->m_is_shouldClose = flag; };

	void set_cursorMode(int flag)   const;

	void make_contextActive();

	       GLFWwindow* const get_window() const { return  this->m_pWindow; }
	const std::string* const get_title()  const { return &this->m_title; }

	RenderOpenGL* const get_render() const { return this->m_render.get(); }
	Input*        const get_input()  const { return this->m_input.get();  }
	UI*           const get_ui()     const { return this->m_ui.get();     }

	unsigned short get_width()  const { return this->m_width;  }
	unsigned short get_height() const { return this->m_height; }
	unsigned short get_count()  const { return this->m_count;  }

	void set_width(unsigned short width);
	void set_height(unsigned short height);
	void set_size(unsigned short width, unsigned short height);

	bool is_cursorLocked() const { return this->m_cursorLocked; }
	void toggleCursor();

	static unsigned short get_windowsCount() { return Window::m_windowsCount; }
};
