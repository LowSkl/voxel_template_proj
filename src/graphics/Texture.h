#pragma once

#include "SneakyThings.h"

struct Shader;

class Texture {
	unsigned int m_UUID, m_width, m_height, m_colorCodes, m_slot;

	TextureTypeGL m_textureTypeGL;
	TextureType   m_textureType;
	TextureFormat m_format;
	PixelType     m_pixelType;

	void finalize();

public:
	static Texture load_texture(const std::string file, const TextureTypeGL texTypeGL, const TextureType texType, const unsigned int slot, const TextureFormat format, const PixelType pixelType);

	Texture(unsigned char* const bytes,
		   const TextureTypeGL texTypeGL, 
		   const TextureType texType,
		   const unsigned int slot, 
		   const TextureFormat format, 
		   const PixelType pixelType, 
		   const unsigned int width,
		   const unsigned int height, 
		   const unsigned int colorCodes);

	~Texture();

	void bind();
	void unbind();
	void reload(const unsigned char* const bytes);

	unsigned int get_UUID()       const { return this->m_UUID;       }
	unsigned int get_width()      const { return this->m_width;      }
	unsigned int get_height()     const { return this->m_height;     }
	unsigned int get_colorCodes() const { return this->m_colorCodes; }

	TextureType get_textureType() const { return this->m_textureType; }
};