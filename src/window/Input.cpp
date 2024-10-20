#include "Input.h"

bool Input::m_keys_pressed[static_cast<size_t>(Keyboard::Codes::KEY_LAST) + 1] = {};
bool Input::m_mouse_buttons_pressed[static_cast<size_t>(Mouse::Codes::MOUSE_BUTTON_LAST) + 1] = {};

bool Input::IsKeyPressed(const Keyboard::Codes key_code) {
    return m_keys_pressed[static_cast<size_t>(key_code)];
}

void Input::PressKey(const Keyboard::Codes key_code) {
    m_keys_pressed[static_cast<size_t>(key_code)] = true;
}

void Input::ReleaseKey(const Keyboard::Codes key_code) {
    m_keys_pressed[static_cast<size_t>(key_code)] = false;
}

bool Input::IsMouseButtonPressed(const Mouse::Codes mouse_button) {
    return m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)];
}

void Input::PressMouseButton(const Mouse::Codes mouse_button) {
    m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = true;
}

void Input::ReleaseMouseButton(const Mouse::Codes mouse_button) {
    m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = false;
}