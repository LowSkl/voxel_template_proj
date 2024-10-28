#pragma once

#include<glad/glad.h>

#include"VBO.h"

/* Vertex Array Object
 * Упорядоченные вершины
 */
class VAO {
	GLuint m_UUID;

public:
	VAO(bool bind = false);
	virtual ~VAO();

	// Прилинковать вершины
	void linkAttrib(VBO& const VBO, const GLuint layout, const GLuint numComponents, const GLenum type, const GLsizeiptr stride, const void* const offset, bool unbind = true);

	// Загрузить VAO
	void bind();

	// Выгрузить VAO
	static void unbind();

	// Удалить VAO
	void finalize();

	GLuint get_UUID() const { return this->m_UUID; }
};