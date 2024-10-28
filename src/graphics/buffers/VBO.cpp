#include "VBO.h"

VBO::VBO(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind) {
	glGenBuffers(1, &ID);
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);

	if (unbind) this->unbind();
}

void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::finalize() {
	glDeleteBuffers(1, &ID);
}

void VBO::reload(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind) {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);

	if (unbind) this->unbind();
}

VBO::~VBO() { 
	this->unbind();
	this->finalize();
}