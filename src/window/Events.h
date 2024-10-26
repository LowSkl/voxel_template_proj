#pragma once

#include "Keys.h"

class Events {
    static bool* m_keysPressed;
    static bool* m_mouseButtonsPressed;

    static int m_mousePosX;
    static int m_mousePosY;

public:
    static int initialize();
    static void update();

    static bool is_keyPressed(const KeyCode keyCode);
    static bool is_mouseButtonPressed(const MouseButton mouseButton);

    static int get_mousePosX() { return Events::m_mousePosX; }
    static int get_mousePosY() { return Events::m_mousePosY; }

    static void set_mousePosX(int x) { Events::m_mousePosX = x; }
    static void set_mousePosY(int y) { Events::m_mousePosY = y; }

    static bool* get_keysPressed() { return Events::m_keysPressed; }
    static bool* get_mouseButtonsPressed() { return Events::m_mouseButtonsPressed; }
};