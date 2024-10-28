#pragma once

#include<glad/glad.h>

/* Element Buffer Object
 * ������������������ ��������� ������
 */
class EBO {
	GLuint m_UUID;

public:
	EBO(const GLuint* const indices, const GLsizeiptr size, const GLenum usage, bool unbind = true);
	virtual ~EBO();

	// ��������� EBO
	void bind();

	// ��������� EBO
	static void unbind();

	// ������� EBO
	void finalize();

	// �������� ������, �� ����� ������
	void reload(const GLuint* const indices, const GLsizeiptr size, const GLenum usage, bool unbind = true);

	GLuint get_UUID() const { return this->m_UUID; }
};