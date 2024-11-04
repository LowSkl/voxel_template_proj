#include "VAO.h"

#include <glad/glad.h>

VAO::VAO() { glGenVertexArrays(1, &this->m_UUID); }

VAO::VAO(VAO&& vao) noexcept
    : m_UUID(vao.m_UUID)
    , m_elementsCount(vao.m_elementsCount)
{
    vao.m_UUID = 0;
    vao.m_elementsCount = 0;
}

VAO& VAO::operator=(VAO&& vao) noexcept
{
    this->m_UUID = vao.m_UUID;
    this->m_elementsCount = vao.m_elementsCount;

    vao.m_UUID = 0;
    vao.m_elementsCount = 0;

    return *this;
}

void VAO::  bind() const { glBindVertexArray(this->m_UUID); }
void VAO::unbind()       { glBindVertexArray(0           ); }

void VAO::add_vertexBuffer(const VBO& vbo)
{
    this->bind();

    for (const BufferElement& current_element : vbo.get_layout().get_elements())
    {
        glEnableVertexAttribArray(this->m_elementsCount);

        glBindVertexBuffer(this->m_elementsCount,
            vbo.get_UUID(),
            current_element.offset,
            static_cast<GLsizei>(vbo.get_layout().get_stride()));

        glVertexAttribFormat(this->m_elementsCount,
            static_cast<GLint>(current_element.components_count),
            current_element.component_type,
            GL_FALSE,
            0);

        glVertexAttribBinding(this->m_elementsCount, this->m_elementsCount);

        ++this->m_elementsCount;
    }
}

void VAO::set_indexBuffer(const EBO& ebo)
{
    this->bind();
    ebo.bind();
    this->m_indicesCount = ebo.get_count();
}

void VAO::finalize() { glDeleteVertexArrays(1, &this->m_UUID); }
     VAO::~VAO()     { this->finalize();                       }