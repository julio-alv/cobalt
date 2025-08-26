#include <glad/glad.h>
#include <stb_image.h>

#include "texture.h"

Texture::Texture(const std::string &path)
    : _path(path), _buffer(nullptr), _id(0),
      _width(0), _height(0), _bpp(0)
{
    stbi_set_flip_vertically_on_load(true);
    _buffer = stbi_load(path.c_str(), &_width, &_height, &_bpp, 4);

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (_buffer)
        stbi_image_free(_buffer);
}

Texture::~Texture()
{
    glDeleteTextures(1, &_id);
}

void Texture::Bind(std::uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
