#pragma once

#include<glad/glad.h>

class VBO {
	GLuint ID;

public:
	VBO(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind = true);
	virtual ~VBO();

	void bind();
	void unbind();
	void finalize();

	void reload(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind = true);

	GLuint get_id() const { return this->ID; }
};

