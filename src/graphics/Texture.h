#pragma once

#include <string>

#include <glad/glad.h>
#include "Shader.h"

class Texture {
	GLuint ID;
	GLenum itextureType, iformat, ipixelType;

	int iweight, iheight, icolorCodes;

public:
	Texture(unsigned char* bytes, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, int weight, int height, int colorCodes, bool unbind);
	virtual ~Texture();

	void bind();
	void unbind();
	void finalize();

	void reload(unsigned char* data);
	void textureUnit(Shader* shader, const char* uniform, GLuint unit);

	static Texture* load_texture(std::string file, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, bool unbind = true);

	GLuint get_id() const { return this->ID; }

	int get_weight()     const { return this->iweight; }
	int get_height()     const { return this->iheight; }
	int get_colorCodes() const { return this->icolorCodes; }
};

