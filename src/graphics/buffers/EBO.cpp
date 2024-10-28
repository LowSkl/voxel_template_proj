#include "EBO.h"

EBO::EBO(const GLuint* indices, const GLsizeiptr size, const GLenum usage, bool unbind) {
	glGenBuffers(1, &ID);
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);

	if (unbind) this->unbind();
}

void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::finalize() {
	glDeleteBuffers(1, &ID);
}

void EBO::reload(const GLuint* indices, const GLsizeiptr size, const GLenum usage, bool unbind) {
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);

	if (unbind) this->unbind();
}

EBO::~EBO() {
	this->unbind();
	this->finalize();
}