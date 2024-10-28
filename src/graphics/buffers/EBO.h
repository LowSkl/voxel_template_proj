#pragma once

#include<glad/glad.h>

/* Element Buffer Object
 * Последовательность отрисовки вершин
 */
class EBO {
	GLuint m_UUID;

public:
	EBO(const GLuint* const indices, const GLsizeiptr size, const GLenum usage, bool unbind = true);
	virtual ~EBO();

	// Загрузить EBO
	void bind();

	// Выгрузить EBO
	static void unbind();

	// Удалить EBO
	void finalize();

	// Поменять данные, не меняя объект
	void reload(const GLuint* const indices, const GLsizeiptr size, const GLenum usage, bool unbind = true);

	GLuint get_UUID() const { return this->m_UUID; }
};