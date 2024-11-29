#pragma once

#include <glm/glm.hpp>

class Camera
{
	glm::vec3 m_position;
	glm::vec3 m_rotation; // X - Roll, Y - Pitch, Z - Yaw

	glm::mat4 m_rotation_mat = glm::mat4(1.0f);

	glm::vec3 m_front = glm::vec3(0, 0, -1);
	glm::vec3 m_up    = glm::vec3(0, 1,  0);
	glm::vec3 m_right = glm::vec3(1, 0,  0);

	float m_far_clip_plane  = 100.f;
	float m_near_clip_plane = 0.1f;
	float m_viewport_width  = 800.f;
	float m_viewport_height = 600.f;
	float m_field_of_view   = 45.f;

	glm::mat4 m_view_matrix;
	glm::mat4 m_projection_matrix;

	void update_vectors();
	void update_view_matrix();
	void update_projection_matrix();

	bool m_update_view_matrix = false;
	bool m_update_up = false;

public:
	Camera(const glm::vec3& position = { 0, 0, 0 },
		   const glm::vec3& rotation = { 0, 0, 0 },
		   const float fov = 45.f);

	const glm::vec3& get_position() const { return m_position; }
	const glm::vec3& get_rotation() const { return m_rotation; }

	void set_position(const glm::vec3& position);

	void set_far_clip_plane(const float far);
	void set_near_clip_plane(const float near);
	void set_viewport_size(const float width, const float height);
	void set_field_of_view(const float fov);
	
	void set_update_up(bool flag) { this->m_update_up = flag; }

	const glm::mat4& get_view_matrix();
	const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }

	const float get_far_clip_plane() const { return m_far_clip_plane; }
	const float get_near_clip_plane() const { return m_near_clip_plane; }
	const float get_field_of_view() const { return m_field_of_view; }

	void move_front(const float delta);
	void move_right(const float delta);
	void move_up   (const float delta);

	void set_up(const glm::vec3 new_data) {this->m_up = new_data; }

	void rotate(float x, float y, float z);
};

