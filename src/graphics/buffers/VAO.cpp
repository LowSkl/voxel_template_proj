#include "VAO.h"

// Создать объект с уникальным ID
VAO::VAO(bool bind) : m_UUID(-1) {
	glGenVertexArrays(1, &this->m_UUID);

	// Добавил для сокращения кода, чтобы не писать каждый раз vao->bind()
	if (bind) this->bind();
}

// Прилинковать вершины
void VAO::linkAttrib(VBO& const VBO, const GLuint layout, const GLuint numComponents, const GLenum type, const GLsizeiptr stride, const void* const offset, bool unbind) {
	VBO.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	
	// Добавил для сокращения кода, чтобы не писать каждый раз vao->bind()
	if (unbind) VBO.unbind();
}

// Загрузить VAO
void VAO::bind() {
	glBindVertexArray(this->m_UUID);
}

// Выгрузить VAO
void VAO::unbind() {
	glBindVertexArray(0);
}

// Удаление VAO
void VAO::finalize() {
	glDeleteVertexArrays(1, &this->m_UUID);
}

// Выгружаем и удаляем буфер
VAO::~VAO() {
	this->unbind();
	this->finalize();
}