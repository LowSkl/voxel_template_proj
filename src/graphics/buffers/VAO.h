#pragma once

#include<glad/glad.h>

#include"VBO.h"

/* Vertex Array Object
 * ������������� �������
 */
class VAO {
	GLuint m_UUID;

public:
	VAO(bool bind = false);
	virtual ~VAO();

	// ������������ �������
	void linkAttrib(VBO& const VBO, const GLuint layout, const GLuint numComponents, const GLenum type, const GLsizeiptr stride, const void* const offset, bool unbind = true);

	// ��������� VAO
	void bind();

	// ��������� VAO
	static void unbind();

	// ������� VAO
	void finalize();

	GLuint get_UUID() const { return this->m_UUID; }
};