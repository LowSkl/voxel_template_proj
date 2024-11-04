#pragma once

#include "Keys.h"

class Window;

class Input
{
	static unsigned short m_inputsCount;
	       unsigned short m_count = 0;

	Window* m_pWindow = nullptr;

	bool m_keysPressed        [static_cast<size_t>(    KeyCode::KEY_LAST         ) + 1] = {};
	bool m_mouseButtonsPressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};

	float m_mousePosX = 0;
	float m_mousePosY = 0;

	float m_mousePosDeltaX = 0;
	float m_mousePosDeltaY = 0;

	int initialize();

public:
	enum Returns
	{
		NO_ERRORS = 0,
	};

	static constexpr std::string returnsToString(int code)
	{
		switch (code)
		{
		case Input::Returns::NO_ERRORS:
			return "NO_ERRORS";
			break;
		} return "UNKNOWN_TYPE";
	}

	Input(Window* pWindow);
	~Input();

	Input(const Input& ) = delete;
	Input(      Input&&) = delete;

	Input& operator=(const Input& ) = delete;
	Input& operator=(      Input&&) = delete;

	bool is_keyPressed        (const KeyCode     keyCode    ) const { return this->m_keysPressed        [static_cast<size_t>(keyCode    )]; }
	bool is_mouseButtonPressed(const MouseButton mouseButton) const { return this->m_mouseButtonsPressed[static_cast<size_t>(mouseButton)]; }

	float get_mousePosX() const { return this->m_mousePosX; }
	float get_mousePosY() const { return this->m_mousePosY; }

	void set_mousePosX(float mousePosX) { this->m_mousePosX = mousePosX; }
	void set_mousePosY(float mousePosY) { this->m_mousePosY = mousePosY; }

	void set_mousePos(float mousePosX, float mousePosY) 
	{ 
		this->m_mousePosX = mousePosX;
		this->m_mousePosY = mousePosY;
	}

	float get_mousePosDeltaX() const { return this->m_mousePosDeltaX; }
	float get_mousePosDeltaY() const { return this->m_mousePosDeltaY; }

	void set_mousePosDeltaX(float mousePosDeltaX) { this->m_mousePosDeltaX = mousePosDeltaX; }
	void set_mousePosDeltaY(float mousePosDeltaY) { this->m_mousePosDeltaY = mousePosDeltaY; }

	void set_mousePosDelta(float mousePosDeltaX, float mousePosDeltaY) 
	{ 
		this->m_mousePosDeltaX = mousePosDeltaX;
		this->m_mousePosDeltaY = mousePosDeltaY;
	}

	bool* get_keysPressed()         { return this->m_keysPressed;         }
	bool* get_mouseButtonsPressed() { return this->m_mouseButtonsPressed; }

	static void update();

	unsigned short get_count() const { return this->m_count; }

    Window* const get_window() const { return this->m_pWindow; }
};
