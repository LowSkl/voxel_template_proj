#include "Events.h"

#include <utils/Log.h>
#include "Window.h"

#include <GLFW/glfw3.h>

bool* Events::m_keysPressed;
bool* Events::m_mouseButtonsPressed;

int Events::m_mousePosX;
int Events::m_mousePosY;

bool Events::is_keyPressed(const KeyCode keyCode) {
    return Events::m_keysPressed[static_cast<size_t>(keyCode)];
}

bool Events::is_mouseButtonPressed(const MouseButton mouse_button) {
    return Events::m_mouseButtonsPressed[static_cast<size_t>(mouse_button)];
}

int Events::initialize() {
	LINFO("Init events");

	Events::m_keysPressed         = new bool[static_cast<size_t>(KeyCode::KEY_LAST) + 1];
	Events::m_mouseButtonsPressed = new bool[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1];

	memset(Events::m_keysPressed,         false, (static_cast<size_t>(KeyCode::KEY_LAST) + 1) * sizeof(bool));
	memset(Events::m_mouseButtonsPressed, false, (static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1) * sizeof(bool));

	glfwSetKeyCallback(Window::get_window(), [](GLFWwindow* window, int key, int scancode, int action, int mode) {
			if (action == GLFW_PRESS) {
				LINFO("Key {0} pressed", static_cast<char>(key));
				Events::get_keysPressed()[static_cast<size_t>(key)] = true;
			} else if (action == GLFW_RELEASE) {
				LINFO("Key {0} released", static_cast<char>(key));
				Events::get_keysPressed()[static_cast<size_t>(key)] = false;
			}
		}
	);

	glfwSetMouseButtonCallback(Window::get_window(), [](GLFWwindow* window, int button, int action, int mode) {
			if (action == GLFW_PRESS) {
				LINFO("Mouse button {0} pressed", button);
				Events::get_mouseButtonsPressed()[static_cast<size_t>(button)] = true;
			} else if (action == GLFW_RELEASE) {
				LINFO("Mouse button {0} released", button);
				Events::get_mouseButtonsPressed()[static_cast<size_t>(button)] = false;
			}
		}
	);

	glfwSetCursorPosCallback(Window::get_window(), [](GLFWwindow* window, double xpos, double ypos) {
			LINFO("Cursor pos {0}x{1}", xpos, ypos);
			Events::set_mousePosX(xpos);
			Events::set_mousePosY(ypos);
		}
	);

	glfwSetWindowSizeCallback(Window::get_window(), [](GLFWwindow* window, int width, int height) {
			LINFO("Resize {0}x{1}", width, height);
			Window::set_width(width);
			Window::set_height(height);
			glViewport(0, 0, width, height);
		}
	);

	return 0;
}

void Events::update() {
	glfwPollEvents();
}
