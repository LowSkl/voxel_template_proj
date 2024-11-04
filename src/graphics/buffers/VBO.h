#pragma once

#include "../SneakyThings.h"

#include <vector>
#include <cstdint>

struct BufferElement
{
    ShaderDataType type;
    uint32_t component_type;
    size_t components_count;
    size_t size;
    size_t offset;

    BufferElement(const ShaderDataType _type)
        : type(_type)
        , component_type(shader_data_type_to_component_type(_type))
        , components_count(shader_data_type_to_components_count(_type))
        , size(shader_data_type_size(_type))
        , offset(0) {}
};

class BufferLayout
{
    std::vector<BufferElement> m_elements;
    size_t m_stride = 0;

public:
    BufferLayout(std::initializer_list<BufferElement> elements)
        : m_elements(elements)
    {
        size_t offset = 0;
        m_stride = 0;
        for (auto& element : m_elements)
        {
            element.offset = offset;
            offset += element.size;
            m_stride += element.size;
        }
    }

    const std::vector<BufferElement>& get_elements() const { return m_elements; }
    size_t get_stride() const { return m_stride; }
};

class VBO
{
	unsigned int m_UUID = 0;
    BufferLayout m_bufferLayout;

    void finalize();

public:
    VBO(const void* data, const size_t size, BufferLayout bufferLayout, const Usage usage = Usage::Static);
    ~VBO();

    VBO(const VBO&     ) = delete;
    VBO(      VBO&& vbo) noexcept;

    VBO& operator=(const VBO&     ) = delete;
    VBO& operator=(      VBO&& vbo) noexcept;

    void bind() const;
    static void unbind();

    void reload(const void* data, const size_t size, BufferLayout bufferLayout, const Usage usage = Usage::Static);

    unsigned int get_UUID()          const { return m_UUID;          }
    const BufferLayout& get_layout() const { return m_bufferLayout; }
};

