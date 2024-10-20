#pragma once

#include <iostream>
#include "Input.h"

struct GLFWwindow;
class Events : protected Input {
public:
	Events();

	GLFWwindow* get_window() const { return m_pWindow; }
	void set_window(GLFWwindow* n_pWindow) { m_pWindow = n_pWindow; }

	void e_update();

protected:
	int e_initialize(GLFWwindow* pWindow);

private:
	GLFWwindow* m_pWindow;
};

