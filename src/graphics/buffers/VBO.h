#pragma once

#include<glad/glad.h>

/* Vertex Buffer Object
 * ������� �������
 */
class VBO {
	GLuint m_UUID;

public:
	VBO(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind = true);
	virtual ~VBO();

	// ��������� VBO
	void bind();

	// ��������� VBO
	static void unbind();

	// ������� VBO
	void finalize();

	// �������� ������, �� ����� ������
	void reload(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind = true);

	GLuint get_UUID() const { return this->m_UUID; }
};

