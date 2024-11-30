#pragma once

#include <glad/glad.h>
#include <utils/Log.h>

enum class ShaderDataType
{
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
};

enum class ShaderType
{
    Vertex,
    Fragment,
};

enum class TextureType
{
    DIFFUSE,
    SPECULAR
};

enum class TextureFormat
{
    RGB,
    RGBA,
    RED,
    GREEN,
    BLUE,
    ALPHA,
};

enum class PixelType
{
    Byte,
    Unsigned_byte,
    Short,
    Unsigned_short,
    Int,
    Unsigned_int,
    Float,
};

enum class Usage
{
    Static,
    Dynamic,
    Stream,
};

constexpr size_t shader_data_type_to_components_count(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Int:
        return 1;

    case ShaderDataType::Float2:
    case ShaderDataType::Int2:
        return 2;

    case ShaderDataType::Float3:
    case ShaderDataType::Int3:
        return 3;

    case ShaderDataType::Float4:
    case ShaderDataType::Int4:
        return 4;
    }

    LERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
    return 0;
}

constexpr size_t shader_data_type_size(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
        return sizeof(GLfloat) * shader_data_type_to_components_count(type);

    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
        return sizeof(GLint) * shader_data_type_to_components_count(type);
    }

    LERROR("shader_data_type_size: unknown ShaderDataType!");
    return 0;
}

constexpr GLenum shader_data_type_to_component_type(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
        return GL_FLOAT;

    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
        return GL_INT;
    }

    LERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
    return GL_FLOAT;
}

constexpr GLenum shader_type_to_component_type(const ShaderType type)
{
    switch (type)
    {
    case ShaderType::Vertex:   return GL_VERTEX_SHADER;
    case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
    }

    LERROR("shader_type_to_component_type: unknown ShaderType!");
    return GL_VERTEX_SHADER;
}

constexpr GLenum usage_to_GLenum(const Usage usage)
{
    switch (usage)
    {
    case Usage::Static:  return GL_STATIC_DRAW;
    case Usage::Dynamic: return GL_DYNAMIC_DRAW;
    case Usage::Stream:  return GL_STREAM_DRAW;
    }

    LERROR("Unknown VertexBuffer usage");
    return GL_STREAM_DRAW;
}

constexpr std::string texture_type_to_string(const TextureType type)
{
    switch (type)
    {
    case TextureType::DIFFUSE:  return "diffuse";
    case TextureType::SPECULAR: return "specular";
    }

    LERROR("Unknown texture type");
    return "diffuse";
}

constexpr GLenum slot_to_component_type(const unsigned int slot)
{
    if (slot > 31 || slot < 0) {
        LERROR("Texture slots range is: 31 >= slot >= 0");
        return GL_TEXTURE0;
    }

    return(GL_TEXTURE0 + slot);
}

constexpr GLenum format_to_component_type(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::RGB:   return GL_RGB;
    case TextureFormat::RGBA:  return GL_RGBA;
    case TextureFormat::RED:   return GL_RED;
    case TextureFormat::GREEN: return GL_GREEN;
    case TextureFormat::BLUE:  return GL_BLUE;
    case TextureFormat::ALPHA: return GL_ALPHA;
    }

    LERROR("Unknown format");
    return GL_RGBA;
}

constexpr GLenum pixel_type_to_component_type(const PixelType type)
{
    switch (type)
    {
    case PixelType::Byte:           return GL_BYTE;
    case PixelType::Unsigned_byte:  return GL_UNSIGNED_BYTE;
    case PixelType::Short:          return GL_SHORT;
    case PixelType::Unsigned_short: return GL_UNSIGNED_SHORT;
    case PixelType::Int:            return GL_INT;
    case PixelType::Unsigned_int:   return GL_UNSIGNED_INT;
    case PixelType::Float:          return GL_FLOAT;
    }

    LERROR("Unknown type");
    return GL_UNSIGNED_BYTE;
}