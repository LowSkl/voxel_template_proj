#include "EBO.h"

#include <glad/glad.h>

EBO::EBO(const void* data, const size_t size, const Usage usage) : m_count(size / sizeof(GLuint))
{
    glGenBuffers(1, &this->m_UUID);
    this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));
}

EBO::EBO(EBO&& ebo) noexcept
    : m_UUID(ebo.m_UUID)
    , m_count(ebo.m_count)
{
    ebo.m_UUID  = 0;
    ebo.m_count = 0;
}

EBO& EBO::operator=(EBO&& ebo) noexcept
{
    this->m_UUID  = ebo.m_UUID;
    this->m_count = ebo.m_count;

    ebo.m_UUID  = 0;
    ebo.m_count = 0;

    return *this;
}

void EBO::  bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_UUID); }
void EBO::unbind()       { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0           ); }

void EBO::reload(const void* data, const size_t size, const Usage usage)
{
    this->bind();
    this->m_count = size / sizeof(GLuint);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));
}

void EBO::finalize() { glDeleteBuffers(1, &this->m_UUID); }
     EBO::~EBO()     { this->finalize();                  }