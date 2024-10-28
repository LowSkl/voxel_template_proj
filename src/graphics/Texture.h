#pragma once

#include <string>

#include <glad/glad.h>
#include "Shader.h"

class Texture {
	GLuint m_UUID, m_weight, m_height, m_colorCodes;
	GLenum m_textureType, m_format, m_pixelType;

public:
	Texture(unsigned char* const bytes, const GLenum texType, const GLenum slot, const GLenum format, const GLenum pixelType, const int weight, const int height, const int colorCodes, bool unbind = true);
	virtual ~Texture();

	// Загрузить текстуру
	void bind();

	// Выгрузить текстуру
	void unbind();

	// Удалить текстуру
	void finalize();

	// Поменять данные, не меняя объект
	void reload(const unsigned char* const bytes, bool unbind = true);

	// Загрузить в сэмплеры шейдера текстуру
	void textureUnit(Shader* const shader, const char* const uniform, const GLuint unit);

	// Загрузить текстуру из файла
	static Texture* load_texture(const std::string file, const GLenum texType, const GLenum slot, const GLenum format, const GLenum pixelType, bool unbind = true);

	GLuint get_UUID()       const { return this->m_UUID;       }
	GLuint get_weight()     const { return this->m_weight;     }
	GLuint get_height()     const { return this->m_height;     }
	GLuint get_colorCodes() const { return this->m_colorCodes; }
};

