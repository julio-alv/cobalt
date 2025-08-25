#include <glad/glad.h>

#include "indexbuffer.h"

IndexBuffer::IndexBuffer(const std::uint32_t *data, std::uint32_t count, bool dynamic)
    : _count(count)
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint32_t), data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &_id);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

inline std::uint32_t IndexBuffer::GetCount()
{
    return _count;
}