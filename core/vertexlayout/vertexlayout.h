#pragma once

#include <glad/glad.h>
#include <vector>

struct LayoutElement
{
    std::uint32_t type;
    std::uint32_t count;
    unsigned char normalized;

    static std::uint32_t GetSize(std::uint32_t type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        default:
            return 0;
        }
    }
};

class VertexLayout
{
    std::vector<LayoutElement> _elements;
    std::uint32_t _stride;

public:
    VertexLayout()
        : _stride(0) {}

    template <typename T>
    void Push(std::uint32_t count) { static_assert(false); }

    template <>
    void Push<float>(std::uint32_t count)
    {
        _elements.push_back({GL_FLOAT, count, GL_FALSE});
        _stride += count * LayoutElement::GetSize(GL_FLOAT);
    }

    template <>
    void Push<std::uint32_t>(std::uint32_t count)
    {
        _elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        _stride += count * LayoutElement::GetSize(GL_UNSIGNED_INT);
    }

    template <>
    void Push<unsigned char>(std::uint32_t count)
    {
        _elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        _stride += count * LayoutElement::GetSize(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<LayoutElement> &GetElements() const { return _elements; }
    inline std::uint32_t GetStride() const { return _stride; }
};
