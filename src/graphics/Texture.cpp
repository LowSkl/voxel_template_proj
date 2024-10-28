#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture* Texture::load_texture(std::string file, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, bool unbind) {
    int weight, height, colorCodes;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(file.c_str(), &weight, &height, &colorCodes, 0);

    return new Texture(bytes, texType, slot, format, pixelType, weight, height, colorCodes, unbind);
}

Texture::Texture(unsigned char* bytes, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, int weight, int height, int colorCodes, bool unbind)
: itextureType(texType), iformat(format), ipixelType(pixelType), iweight(weight), iheight(height), icolorCodes(colorCodes) 
{
    glGenTextures(1, &this->ID);
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

void Texture::bind() {
    glBindTexture(this->itextureType, this->ID);
}

void Texture::unbind() {
    glBindTexture(this->itextureType, 0);
}

void Texture::finalize() {
    this->unbind();
    glDeleteTextures(1, &this->ID);
}

void Texture::reload(unsigned char* bytes) {
    this->bind();
    glTexImage2D(this->itextureType, 0, GL_RGBA, this->iweight, this->iheight, 0, this->iformat, this->ipixelType, (GLvoid*)bytes);
    this->unbind();
}

void Texture::textureUnit(Shader* shader, const char* uniform, GLuint unit) {
    GLuint texUni = glGetUniformLocation(shader->get_id(), uniform);
    shader->initialize();
    glUniform1i(texUni, unit);
}

Texture::~Texture() {
    this->unbind();
    this->finalize();
}