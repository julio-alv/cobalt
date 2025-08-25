#pragma once

#include <glad/glad.h>
#include <vector>

struct LayoutElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSize(unsigned int type)
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
    unsigned int _stride;

public:
    VertexLayout()
        : _stride(0) {}

    template <typename T>
    void Push(unsigned int count) { static_assert(false); }

    template <>
    void Push<float>(unsigned int count)
    {
        _elements.push_back({GL_FLOAT, count, GL_FALSE});
        _stride += count * LayoutElement::GetSize(GL_FLOAT);
    }

    template <>
    void Push<unsigned int>(unsigned int count)
    {
        _elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        _stride += count * LayoutElement::GetSize(GL_UNSIGNED_INT);
    }

    template <>
    void Push<unsigned char>(unsigned int count)
    {
        _elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        _stride += count * LayoutElement::GetSize(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<LayoutElement> &GetElements() const { return _elements; }
    inline unsigned int GetStride() const { return _stride; }
};
