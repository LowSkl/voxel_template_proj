#include "VAO.h"

VAO::VAO(bool bind) {
	glGenVertexArrays(1, &ID);
	if (bind) this->bind();
}

void VAO::linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.unbind();
}

void VAO::bind() {
	glBindVertexArray(ID);
}

void VAO::unbind() {
	glBindVertexArray(0);
}

void VAO::finalize() {
	glDeleteVertexArrays(1, &ID);
}

VAO::~VAO() {
	this->unbind();
	this->finalize();
}