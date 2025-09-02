#include <glad/glad.h>

#include "renderer.h"

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::SetWireframe() const
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::SetFill() const
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
