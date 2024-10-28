#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Загрузить текстуру из файла
Texture* Texture::load_texture(const std::string file, const GLenum texType, const GLenum slot, const GLenum format, const GLenum pixelType, bool unbind) {
    int weight, height, colorCodes;

    // stbi и opengl читают по разному, ставим чтобы читали одинаково
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(file.c_str(), &weight, &height, &colorCodes, 0);

    return new Texture(bytes, texType, slot, format, pixelType, weight, height, colorCodes, unbind);
}

Texture::Texture(unsigned char* const bytes, const GLenum texType, const GLenum slot, const GLenum format, const GLenum pixelType, const int weight, const int height, const int colorCodes, bool unbind)
: m_UUID(-1), m_textureType(texType), m_format(format), m_pixelType(pixelType), m_weight(weight), m_height(height), m_colorCodes(colorCodes)
{
    glGenTextures(1, &this->m_UUID);
    this->bind();
    if (slot != -1) glActiveTexture(slot);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texType, 0, GL_RGBA, weight, height, 0, format, pixelType, (GLvoid*)bytes);
    glGenerateMipmap(texType);

    stbi_image_free(bytes);
    if (unbind) this->unbind();
}

// Загрузить текстуру
void Texture::bind() {
    glBindTexture(this->m_textureType, this->m_UUID);
}

// Выгрузить текстуру
void Texture::unbind() {
    glBindTexture(this->m_textureType, 0);
}

// Удалить текстуру
void Texture::finalize() {
    this->unbind();
    glDeleteTextures(1, &this->m_UUID);
}

// Поменять данные, не меняя объект
void Texture::reload(const unsigned char* const bytes, bool unbind) {
    // Запихиваем данные
    this->bind();
    glTexImage2D(this->m_textureType, 0, GL_RGBA, this->m_weight, this->m_height, 0, this->m_format, this->m_pixelType, (GLvoid*)bytes);

    // Добавил для сокращения кода, чтобы не писать каждый раз texture->bind()
    if (unbind) this->unbind();
}

// Загрузить в сэмплеры шейдера текстуру
void Texture::textureUnit(Shader* const shader, const char* const uniform, const GLuint unit) {
    GLuint texUni = glGetUniformLocation(shader->get_UUID(), uniform);
    shader->bind();
    glUniform1i(texUni, unit);
}

// Выгружаем и удаляем буфер
Texture::~Texture() {
    this->unbind();
    this->finalize();
}