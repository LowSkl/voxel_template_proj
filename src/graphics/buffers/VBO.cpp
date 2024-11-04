#include "VBO.h"

#include <utils/Log.h>
#include <glad/glad.h>

VBO::VBO(const void* data, const size_t size, BufferLayout bufferLayout, const Usage usage) : m_bufferLayout(std::move(bufferLayout))
{
    glGenBuffers(1, &this->m_UUID);
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));
}

VBO& VBO::operator=(VBO&& vbo) noexcept
{
    this->m_UUID = vbo.m_UUID;
    vbo.m_UUID = 0;
    return *this;
}

VBO::VBO(VBO&& vbo) noexcept
    : m_UUID(vbo.m_UUID)
    , m_bufferLayout(std::move(vbo.m_bufferLayout))
{
    vbo.m_UUID = 0;
}

void VBO::reload(const void* data, const size_t size, BufferLayout bufferLayout, const Usage usage)
{
    this->m_bufferLayout = std::move(bufferLayout);
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));
}

void VBO::  bind() const { glBindBuffer(GL_ARRAY_BUFFER, this->m_UUID); }
void VBO::unbind()       { glBindBuffer(GL_ARRAY_BUFFER, 0           ); }

void VBO::finalize() { glDeleteBuffers(1, &this->m_UUID); }
     VBO::~VBO()     { this->finalize();                  }
