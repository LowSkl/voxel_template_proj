#include "Events.h"

#include <utils/Log.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Events::Events() {
    LINFO("Init events");
}

int Events::e_initialize(GLFWwindow* pWindow) {
    LINFO("Events::e_initialize()");

    this->m_pWindow = pWindow;
	glfwSetKeyCallback(m_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		switch (action) {
            case GLFW_REPEAT:
            case GLFW_PRESS: {
                LINFO("key pressed {0}", key);
                PressKey(static_cast<Keyboard::Codes>(key));
                break;
            }
            case GLFW_RELEASE: {
                LINFO("key released {0}", key);
                ReleaseKey(static_cast<Keyboard::Codes>(key));
                break;
            }
		}
	});

    glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* window, int button, int action, int mods) {
        switch (action) {
            case GLFW_PRESS: {
                LINFO("mouse pressed {0}", button);
                PressMouseButton(static_cast<Mouse::Codes>(button));
                break;
            }
            case GLFW_RELEASE: {
                LINFO("mouse released {0}", button);
                ReleaseMouseButton(static_cast<Mouse::Codes>(button));
                break;
            }
		}
    });

    return 0;
}

void Events::e_update() {
    //LINFO("Events::e_update()");
    glfwPollEvents();
}

