#pragma once

#include "SneakyThings.h"

class Shader;

class Texture {
	unsigned int m_UUID, m_width, m_height, m_colorCodes, m_slot;

	TextureType   m_textureType;
	TextureFormat m_format;
	PixelType     m_pixelType;

	void finalize();

public:
	static Texture* load_texture(const std::string file, const TextureType texType, const unsigned int slot, const TextureFormat format, const PixelType pixelType);

	Texture(unsigned char* const bytes,
		   const TextureType texType, 
		   const unsigned int slot, 
		   const TextureFormat format, 
		   const PixelType pixelType, 
		   const unsigned int width,
		   const unsigned int height, 
		   const unsigned int colorCodes);

	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&& texture) noexcept;
	Texture(Texture&& texture) noexcept;

	void bind();
	void unbind();
	void reload(const unsigned char* const bytes);

	unsigned int get_UUID()       const { return this->m_UUID;       }
	unsigned int get_width()      const { return this->m_width;      }
	unsigned int get_height()     const { return this->m_height;     }
	unsigned int get_colorCodes() const { return this->m_colorCodes; }
};