#pragma once

#include <cstdint>

class VertexBuffer
{
    std::uint32_t _id;

public:
    explicit VertexBuffer(const void *data, std::uint32_t size, bool dynamic = false);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};
