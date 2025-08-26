#pragma once

#include "vertexbuffer.h"
#include "vertexlayout.h"

class VertexArray
{
    std::uint32_t _id;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer &vbo, const VertexLayout &layout);
    void Bind() const;
    void Unbind() const;
};
