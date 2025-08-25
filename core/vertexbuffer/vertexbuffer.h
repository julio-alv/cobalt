#pragma once

#include <cstdint>

class VertexBuffer
{
    unsigned int _id;

public:
    explicit VertexBuffer(const void *data, unsigned int size, bool dynamic = false);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};
