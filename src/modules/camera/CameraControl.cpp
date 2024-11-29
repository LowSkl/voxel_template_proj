#include "CameraControl.h"

CameraControl::CameraControl(Window* window, Camera* camera) : m_pWindow(window), m_pCamera(camera), m_lastTime(this->m_pWindow->get_render()->get_time())
{
	this->update();
}

void CameraControl::update()
{
    float currentTime = this->m_pWindow->get_render()->get_time();
    this->m_delta = currentTime - this->m_lastTime;
    this->m_lastTime = currentTime;

    Input* input = this->m_pWindow->get_input();

    if (this->m_pWindow->is_cursorLocked())
    {
        unsigned short height = this->m_pWindow->get_height();

        float rotX = input->get_mousePosDeltaY() / height * 2;
        float rotY = input->get_mousePosDeltaX() / height * 2;

        this->m_pCamera->rotate(rotX * this->m_sens * this->m_delta * 100, rotY * this->m_sens * this->m_delta * 100, 0.f);
    }

    if (input->is_keyPressed(KeyCode::KEY_W)) this->m_pCamera->move_front(this->m_delta *  this->m_speed);
    if (input->is_keyPressed(KeyCode::KEY_A)) this->m_pCamera->move_right(this->m_delta * -this->m_speed);
    if (input->is_keyPressed(KeyCode::KEY_S)) this->m_pCamera->move_front(this->m_delta * -this->m_speed);
    if (input->is_keyPressed(KeyCode::KEY_D)) this->m_pCamera->move_right(this->m_delta *  this->m_speed);

    if (input->is_keyPressed(KeyCode::KEY_LEFT_SHIFT)) this->m_pCamera->move_up(this->m_delta * -this->m_speed);
    if (input->is_keyPressed(KeyCode::KEY_SPACE))      this->m_pCamera->move_up(this->m_delta *  this->m_speed);
}
