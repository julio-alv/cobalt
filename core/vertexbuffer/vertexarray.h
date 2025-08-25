#pragma once

#include "vertexbuffer.h"
#include "vertexlayout.h"

class VertexArray
{
    unsigned int _id;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer &vbo, const VertexLayout &layout);
    void Bind() const;
    void Unbind() const;
};
