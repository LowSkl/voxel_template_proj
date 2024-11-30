#pragma once

#include "SneakyThings.h"

struct Shader;

class Texture {
	unsigned int m_UUID, m_width, m_height, m_colorCodes, m_slot;

	TextureType m_textureType;

	void finalize();

public:
	static Texture load_texture(const std::string file, const TextureType texType, const unsigned int slot);

	Texture(unsigned char* const bytes,
		   const TextureType texType,
		   const unsigned int slot,
		   const unsigned int width,
		   const unsigned int height, 
		   const unsigned int colorCodes);

	~Texture();

	void bind();
	void unbind();

	unsigned int get_UUID()       const { return this->m_UUID;       }
	unsigned int get_width()      const { return this->m_width;      }
	unsigned int get_height()     const { return this->m_height;     }
	unsigned int get_colorCodes() const { return this->m_colorCodes; }

	TextureType get_textureType() const { return this->m_textureType; }
};