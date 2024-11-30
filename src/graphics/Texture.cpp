#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture Texture::load_texture(const std::string file, const TextureTypeGL texTypeGL, const TextureType texType, const unsigned int slot, const TextureFormat format, const PixelType pixelType)
{
	int weight, height, colorCodes;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(file.c_str(), &weight, &height, &colorCodes, 0);

	return Texture(bytes, texTypeGL, texType, slot, format, pixelType, weight, height, colorCodes);
}

Texture::Texture(
	unsigned char* const bytes,
	const TextureTypeGL texTypeGL,
	const TextureType texType,
	const unsigned int slot,
	const TextureFormat format,
	const PixelType pixelType,
	const unsigned int width,
	const unsigned int height,
	const unsigned int colorCodes)
	:
	m_textureType(texType),
	m_slot(slot),
	m_UUID(0), 
	m_textureTypeGL(texTypeGL),
	m_format(format), 
	m_pixelType(pixelType), 
	m_width(width),
	m_height(height), 
	m_colorCodes(colorCodes)
{
	glGenTextures(1, &this->m_UUID);
	this->bind();

	glActiveTexture(slot_to_component_type(slot));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLenum texture_type = texture_GL_type_to_component_type(texTypeGL);

	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(texture_type, 0, texture_type_to_color_set(texType), width, height, 0, format_to_component_type(format), pixel_type_to_component_type(pixelType), (GLvoid*)bytes);
	glGenerateMipmap(texture_type);

	stbi_image_free(bytes);
	this->unbind();
}

void Texture::  bind() { glActiveTexture(slot_to_component_type(this->m_slot)); glBindTexture(texture_GL_type_to_component_type(this->m_textureTypeGL), this->m_UUID); }
void Texture::unbind() {                                                        glBindTexture(texture_GL_type_to_component_type(this->m_textureTypeGL),            0); }

void Texture::reload(const unsigned char* const bytes)
{
	this->bind();
	glTexImage2D(texture_GL_type_to_component_type(this->m_textureTypeGL), 0, texture_type_to_color_set(this->m_textureType), this->m_width, this->m_height, 0,
		         format_to_component_type(this->m_format), 
		         pixel_type_to_component_type(this->m_pixelType), (GLvoid*)bytes);
}

void Texture::finalize() { glDeleteTextures(1, &this->m_UUID); }
     Texture::~Texture() { this->finalize();                   }
