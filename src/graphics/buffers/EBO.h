#pragma once

#include<glad/glad.h>

class EBO {
	GLuint ID;

public:
	EBO(const GLuint* indices, const GLsizeiptr size, const GLenum usage, bool unbind = true);
	virtual ~EBO();

	void bind();
	void unbind();
	void finalize();

	void reload(const GLuint* indices, const GLsizeiptr size, const GLenum usage, bool unbind = true);

	GLuint get_id() const { return this->ID; }
};