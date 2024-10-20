#pragma once

#include "Keys.h"

class Input {
public:
    static bool IsKeyPressed(const Keyboard::Codes key_code);
    static bool IsMouseButtonPressed(const Mouse::Codes mouse_button);

protected:
    static void ReleaseKey(const Keyboard::Codes key_code);
    static void ReleaseMouseButton(const Mouse::Codes mouse_button);

    static void PressKey(const Keyboard::Codes key_code);
    static void PressMouseButton(const Mouse::Codes mouse_button);

private:
    static bool m_keys_pressed[];
    static bool m_mouse_buttons_pressed[];
};