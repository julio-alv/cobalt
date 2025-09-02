#pragma once

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;

    void SetWireframe() const;
    void SetFill() const;
};
