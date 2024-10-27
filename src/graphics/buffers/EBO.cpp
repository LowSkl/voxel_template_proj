#include "EBO.h"

EBO::EBO(const GLuint* indices, const GLsizeiptr size, const GLenum usage) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
}

EBO::EBO(const GLuint id) : ID(id) {}

void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::finalize() {
	glDeleteBuffers(1, &ID);
}

EBO::~EBO() {
	this->unbind();
	this->finalize();
}