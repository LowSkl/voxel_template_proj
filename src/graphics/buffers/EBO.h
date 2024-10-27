#pragma once

#include<glad/glad.h>

class EBO {
	GLuint ID;

public:
	EBO(const GLuint* indices, const GLsizeiptr size, const GLenum usage);
	EBO(const GLuint id);

	virtual ~EBO();

	void bind();
	void unbind();
	void finalize();

	GLuint get_id() const { return this->ID; }
};