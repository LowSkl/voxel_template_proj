#pragma once

#include<glad/glad.h>

#include"VBO.h"

class VAO {
	GLuint ID;

public:
	VAO();
	VAO(const GLuint id);

	virtual ~VAO();

	void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	void bind();
	void unbind();
	void finalize();

	GLuint get_id() const { return this->ID; }
};