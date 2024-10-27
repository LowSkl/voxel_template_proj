#include "VBO.h"

VBO::VBO(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

VBO::VBO(const GLuint id) : ID(id) {}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}

VBO::~VBO() { 
	this->Unbind();
	this->Delete(); 
}