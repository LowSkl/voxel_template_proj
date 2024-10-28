#include "VBO.h"

// Создать объект с уникальным ID и вставить данные
VBO::VBO(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind) : m_UUID(-1) {
	// Генерируем буфер, загружаем его, запихиваем данные
	glGenBuffers(1, &this->m_UUID);
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);

	// Добавил для сокращения кода, чтобы не писать каждый раз vbo->bind()
	if (unbind) this->unbind();
}

// Загрузить VBO
void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, this->m_UUID);
}

// Выгрузить VBO
void VBO::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Удаление VBO
void VBO::finalize() {
	glDeleteBuffers(1, &this->m_UUID);
}

// Поменять данные, не меняя объект
void VBO::reload(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind) {
	// Загружаем буфер, запихиваем данные
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);

	// Добавил для сокращения кода, чтобы не писать каждый раз vbo->bind()
	if (unbind) this->unbind();
}

// Выгружаем и удаляем буфер
VBO::~VBO() {
	this->unbind();
	this->finalize();
}