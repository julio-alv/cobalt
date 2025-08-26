#include <glad/glad.h>

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "vertexlayout.h"

VertexArray::VertexArray() { glGenVertexArrays(1, &_id); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &_id); }

void VertexArray::AddBuffer(const VertexBuffer &vbo, const VertexLayout &layout)
{
    Bind();
    vbo.Bind();
    const auto &elements = layout.GetElements();

    std::uint32_t offset = 0;

    for (std::uint32_t i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i, element.count, element.type,
            element.normalized, layout.GetStride(),
            reinterpret_cast<const void *>(static_cast<std::uintptr_t>(offset)));

        offset += element.count * LayoutElement::GetSize(element.type);
    }
}

void VertexArray::Bind() const { glBindVertexArray(_id); }
void VertexArray::Unbind() const { glBindVertexArray(0); }
