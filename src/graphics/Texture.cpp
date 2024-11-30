#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture Texture::load_texture(const std::string file, const TextureType texType, const unsigned int slot)
{
	int weight, height, colorCodes;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(file.c_str(), &weight, &height, &colorCodes, 0);

	return Texture(bytes, texType, slot, weight, height, colorCodes);
}

Texture::Texture(
	unsigned char* const bytes,
	const TextureType texType,
	const unsigned int slot,
	const unsigned int width,
	const unsigned int height,
	const unsigned int colorCodes)
	:
	m_textureType(texType),
	m_slot(slot),
	m_UUID(0), 
	m_width(width),
	m_height(height), 
	m_colorCodes(colorCodes)
{
	glGenTextures(1, &this->m_UUID);
	this->bind();

	glActiveTexture(slot_to_component_type(slot));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (colorCodes) {
	case 4: glTexImage2D
		  (
			  GL_TEXTURE_2D,
			  0,
			  GL_RGBA,
			  width,
			  height,
			  0,
			  GL_RGBA,
			  GL_UNSIGNED_BYTE,
			  (GLvoid*)bytes
		  ); break;
	case 3:
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			(GLvoid*)bytes
		); break;
	case 1:
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			(GLvoid*)bytes
		); break;
	default: throw std::invalid_argument("Automatic Texture type recognition failed"); break;
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	this->unbind();
}

void Texture::  bind() { glActiveTexture(slot_to_component_type(this->m_slot)); glBindTexture(GL_TEXTURE_2D, this->m_UUID); }
void Texture::unbind() {                                                        glBindTexture(GL_TEXTURE_2D,            0); }

void Texture::finalize() { glDeleteTextures(1, &this->m_UUID); }
     Texture::~Texture() { this->finalize();                   }
