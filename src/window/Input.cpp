#include <window/Window.h>

#include "Input.h"
#include "RenderOpenGL.h"

#include <GLFW/glfw3.h>
#include <utils/Log.h>

#include <imgui_impl_glfw.h>

unsigned short Input::m_inputsCount = 0;

Input::Input(Window* pWindow) :
	m_pWindow(pWindow), m_count(++Input::m_inputsCount)
{
	LINFO("[Input {}]\tInput::Input(\"{}\")", this->m_count, pWindow->get_title()->c_str());

	int code = this->initialize();
	LINFO("[Input {}]\tinit code {} ({})", this->m_count, code, Input::returnsToString(code));
}

int Input::initialize()
{
    LINFO("[Input {}]\tInput::initialize()", this->m_count);

    memset(this->m_keysPressed,         false, (static_cast<size_t>(    KeyCode::KEY_LAST) + 1)          * sizeof(bool));
    memset(this->m_mouseButtonsPressed, false, (static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1) * sizeof(bool));

    glfwSetKeyCallback(this->m_pWindow->get_window(),
        [](GLFWwindow* pWindow, int key, int scancode, int action, int mods)
        {
            Window* pMyWindow = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
            Input* pMyInput = pMyWindow->get_input();
            UI* pMyUI = pMyWindow->get_ui();

            if (pMyUI != nullptr) {
                pMyUI->make_contextActive();
                ImGui_ImplGlfw_KeyCallback(pWindow, key, scancode, action, mods);
            }

            switch (action)
            {
            case GLFW_PRESS:
            {
                if (key <= static_cast<int>(KeyCode::KEY_Z)) LINFO("[Input {}]\tkey {} pressed", pMyInput->get_count(), static_cast<char>(key));
                pMyInput->get_keysPressed()[key] = true;
                break;
            }
            case GLFW_RELEASE:
            {
                if (key <= static_cast<int>(KeyCode::KEY_Z)) LINFO("[Input {}]\tkey {} released", pMyInput->get_count(), static_cast<char>(key));
                pMyInput->get_keysPressed()[key] = false;
                break;
            }
            }

            pMyInput->get_keyFrames()[key] = pMyInput->get_currentFrame();
        }
    );

    glfwSetMouseButtonCallback(this->m_pWindow->get_window(),
        [](GLFWwindow* pWindow, int button, int action, int mods)
        {
            Window* pMyWindow = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
            Input* pMyInput = pMyWindow->get_input();
            UI* pMyUI = pMyWindow->get_ui();

            if (pMyUI != nullptr) {
                pMyUI->make_contextActive();
                ImGui_ImplGlfw_MouseButtonCallback(pWindow, button, action, mods);
            }

            switch (action)
            {
            case GLFW_PRESS:
            {
                LINFO("[Input {}]\tmouse button {} pressed", pMyInput->get_count(), button);
                pMyInput->get_mouseButtonsPressed()[button] = true;
                break;
            }
            case GLFW_RELEASE:
            {
                LINFO("[Input {}]\tmouse button {} released", pMyInput->get_count(), button);
                pMyInput->get_mouseButtonsPressed()[button] = false;
                break;
            }
            }

            pMyInput->get_mouseButtonFrames()[button] = pMyInput->get_currentFrame();
        }
    );

    glfwSetWindowSizeCallback(this->m_pWindow->get_window(),
        [](GLFWwindow* pWindow, int width, int height)
        {
            Window* pMyWindow = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
            Input* pMyInput = pMyWindow->get_input();

            LINFO("[Input {}]\twindow resized {}x{}", pMyInput->get_count(), width, height);
            pMyWindow->set_size(width, height);
        }
    );

    glfwSetFramebufferSizeCallback(this->m_pWindow->get_window(),
        [](GLFWwindow* pWindow, int width, int height)
        {
            Window* pMyWindow = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
            RenderOpenGL* pMyRender = pMyWindow->get_render();
            Input* pMyInput = pMyWindow->get_input();

            pMyRender->set_viewport(width, height, 0, 0);
            LINFO("[Input {}]\tbuffer resized {}x{}", pMyInput->get_count(), width, height);
        }
    );

    glfwSetCursorPosCallback(this->m_pWindow->get_window(),
        [](GLFWwindow* pWindow, double x, double y)
        {
            Window* pMyWindow = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
            Input* pMyInput = pMyWindow->get_input();
            UI* pMyUI = pMyWindow->get_ui();

            if (pMyUI != nullptr) {
                pMyUI->make_contextActive();
                ImGui_ImplGlfw_CursorPosCallback(pWindow, x, y);
            }

            float deltaX = pMyInput->get_mousePosX() - x;
            float deltaY = pMyInput->get_mousePosY() - y;

            pMyInput->set_mousePosDelta(deltaX, deltaY);
            pMyInput->set_mousePos(x, y);

            LINFO("[Input {}]\tmouse moved {}x{}, delta: {}x{}", pMyInput->get_count(), x, y, deltaX, deltaY);
        }
    );

    glfwSetWindowCloseCallback(this->m_pWindow->get_window(),
        [](GLFWwindow* pWindow)
        {
            Window* pMyWindow = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
            Input* pMyInput = pMyWindow->get_input();

            pMyWindow->set_shouldClose(true);
            LINFO("[Input {}]\twindow closing", pMyInput->get_count());
        }
    );

    glfwSetScrollCallback(this->m_pWindow->get_window(),
        [](GLFWwindow* pWindow, double xoffset, double yoffset)
        {
            Window* pMyWindow = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
            Input* pMyInput = pMyWindow->get_input();
            UI* pMyUI = pMyWindow->get_ui();

            if (pMyUI != nullptr) {
                pMyUI->make_contextActive();
                ImGui_ImplGlfw_ScrollCallback(pWindow, xoffset, yoffset);
            }

            pMyInput->set_scroll(yoffset);
            LINFO("[Input {}]\tscroll offset {}", pMyInput->get_count(), yoffset);
        }
    );

	return Input::Returns::NO_ERRORS;
}

void Input::update() 
{
    this->m_currentFrame++;
    
    this->m_mousePosDeltaX = 0;
    this->m_mousePosDeltaY = 0;

    this->m_scroll = 0;

	glfwPollEvents(); 
}

void Input::finalize()
{
    LINFO("[Input {}]\tunbinding callbacks", this->m_count);

    glfwSetKeyCallback            (this->m_pWindow->get_window(), nullptr);
    glfwSetMouseButtonCallback    (this->m_pWindow->get_window(), nullptr);
    glfwSetWindowSizeCallback     (this->m_pWindow->get_window(), nullptr);
    glfwSetFramebufferSizeCallback(this->m_pWindow->get_window(), nullptr);
    glfwSetCursorPosCallback      (this->m_pWindow->get_window(), nullptr);
    glfwSetWindowCloseCallback    (this->m_pWindow->get_window(), nullptr);
    glfwSetScrollCallback         (this->m_pWindow->get_window(), nullptr);
}

Input::~Input()
{
    LINFO("[Input {}]\tInput::~Input()", this->m_count);
    this->finalize();
}