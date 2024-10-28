#include "EBO.h"

// Создать объект с уникальным ID и вставить данные
EBO::EBO(const GLuint* const indices, const GLsizeiptr size, const GLenum usage, bool unbind) : m_UUID(-1) {
	// Генерируем буфер, загружаем его, запихиваем данные
	glGenBuffers(1, &this->m_UUID);
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);

	// Добавил для сокращения кода, чтобы не писать каждый раз ebo->bind()
	if (unbind) this->unbind();
}

// Загрузить EBO
void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_UUID);
}

// Выгрузить EBO
void EBO::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Удаление EBO
void EBO::finalize() {
	glDeleteBuffers(1, &this->m_UUID);
}

// Поменять данные, не меняя объект
void EBO::reload(const GLuint* const indices, const GLsizeiptr size, const GLenum usage, bool unbind) {
	// Загружаем буфер, запихиваем данные
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);

	// Добавил для сокращения кода, чтобы не писать каждый раз ebo->bind()
	if (unbind) this->unbind();
}

// Выгружаем и удаляем буфер
EBO::~EBO() {
	this->unbind();
	this->finalize();
}