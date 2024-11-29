#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, const float fov) : m_position(position), m_rotation(rotation), m_field_of_view(fov)
{
    this->update_vectors();
    this->update_view_matrix();
    this->update_projection_matrix();
}

const glm::mat4& Camera::get_view_matrix()
{
    if (this->m_update_view_matrix)
    {
        this->update_view_matrix();
        this->m_update_view_matrix = false;
        this->m_rotation_mat = glm::mat4(1.0f);
    }
    return this->m_view_matrix;
}

void Camera::update_projection_matrix()
{
    float aspect = this->m_viewport_width / this->m_viewport_height;
	this->m_projection_matrix = glm::perspective(glm::radians(this->m_field_of_view), aspect, this->m_near_clip_plane, this->m_far_clip_plane);
}

void Camera::update_vectors()
{
                           this->m_front = glm::vec3(this->m_rotation_mat * glm::vec4(0, 0, -1, 1));
    if (this->m_update_up) this->m_up    = glm::vec3(this->m_rotation_mat * glm::vec4(0, 1,  0, 1));
                           this->m_right = glm::vec3(this->m_rotation_mat * glm::vec4(1, 0,  0, 1));
}

void Camera::update_view_matrix()
{
    this->update_vectors();
    this->m_view_matrix = glm::lookAt(this->m_position, this->m_position + this->m_front, this->m_up);
}

void Camera::set_position(const glm::vec3& position)
{
    this->m_position = position;
    this->m_update_view_matrix = true;
}

void Camera::set_far_clip_plane(const float far)
{
    this->m_far_clip_plane = far;
    update_projection_matrix();
}

void Camera::set_near_clip_plane(const float near)
{
    this->m_near_clip_plane = near;
    update_projection_matrix();
}

void Camera::set_viewport_size(const float width, const float height)
{
    this->m_viewport_width = width;
    this->m_viewport_height = height;
    update_projection_matrix();
}

void Camera::set_field_of_view(const float fov)
{
    this->m_field_of_view = fov;
    update_projection_matrix();
}

void Camera::move_front(const float delta)
{
    this->m_position += this->m_front * delta;
    this->m_update_view_matrix = true;
}

void Camera::move_right(const float delta)
{
    this->m_position += this->m_right * delta;
    this->m_update_view_matrix = true;
}

void Camera::move_up(const float delta)
{
    this->m_position += this->m_up * delta;
    this->m_update_view_matrix = true;
}

void Camera::rotate(float x, float y, float z) {
    this->m_rotation += glm::vec3(x, y, z);
    if (this->m_rotation.x < -glm::radians(89.0f))
        this->m_rotation.x = -glm::radians(89.0f);
    
    if (this->m_rotation.x > glm::radians(89.0f))
        this->m_rotation.x = glm::radians(89.0f);

    this->m_rotation_mat = glm::rotate(this->m_rotation_mat, this->m_rotation.z, glm::vec3(0, 0, 1));
    this->m_rotation_mat = glm::rotate(this->m_rotation_mat, this->m_rotation.y, glm::vec3(0, 1, 0));
    this->m_rotation_mat = glm::rotate(this->m_rotation_mat, this->m_rotation.x, glm::vec3(1, 0, 0));

    this->m_update_view_matrix = true;
}