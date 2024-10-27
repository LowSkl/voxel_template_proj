#pragma once

#include<glad/glad.h>

class VBO {
	GLuint ID;

public:
	VBO(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage);
	VBO(const GLuint id);

	virtual ~VBO();

	void Bind();
	void Unbind();
	void Delete();

	GLuint get_id() const { return this->ID; }
};

