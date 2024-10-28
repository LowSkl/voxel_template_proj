#pragma once

#include<glad/glad.h>

/* Vertex Buffer Object
 * Вершины объекта
 */
class VBO {
	GLuint m_UUID;

public:
	VBO(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind = true);
	virtual ~VBO();

	// Загрузить VBO
	void bind();

	// Выгрузить VBO
	static void unbind();

	// Удалить VBO
	void finalize();

	// Поменять данные, не меняя объект
	void reload(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind = true);

	GLuint get_UUID() const { return this->m_UUID; }
};

