#pragma once

#include "EBO.h"
#include "VBO.h"

class VAO
{
    unsigned int m_UUID = 0;
    unsigned int m_elementsCount = 0;
    unsigned int m_indicesCount = 0;

	void finalize();

public:
	VAO();
	~VAO();

	VAO(const VAO&     ) = delete;
	VAO(      VAO&& vao) noexcept;

	VAO& operator=(const VAO&     ) = delete;
	VAO& operator=(      VAO&& vao) noexcept;

	void bind() const;
	static void unbind();

	void add_vertexBuffer(const VBO& vbo);
	void set_indexBuffer(const EBO& ebo);

	unsigned int get_UUID()          const { return this->m_UUID;          }
	unsigned int get_elementsCount() const { return this->m_elementsCount; }
	unsigned int get_indicesCount()  const { return this->m_indicesCount;  }
};

