#pragma once

#include <cstdint>
#include <string>

class Texture
{
private:
    std::string _path;
    std::uint8_t *_buffer;
    std::uint32_t _id;
    std::int32_t _width, _height, _bpp;

public:
    Texture(const std::string &path);
    ~Texture();

    void Bind(std::uint32_t slot = 0) const;
    void Unbind() const;

    inline std::int32_t GetWidth() const { return _width; }
    inline std::int32_t GetHeight() const { return _height; }
};
