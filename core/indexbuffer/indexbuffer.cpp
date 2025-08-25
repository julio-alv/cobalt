#include <glad/glad.h>

#include "indexbuffer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count, bool dynamic)
    : _count(count)
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
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

inline unsigned int IndexBuffer::GetCount()
{
    return _count;
}