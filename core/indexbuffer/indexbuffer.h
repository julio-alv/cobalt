#pragma once

#include <cstdint>

class IndexBuffer
{
    std::uint32_t _id;
    std::uint32_t _count;

public:
    explicit IndexBuffer(const std::uint32_t *data, std::uint32_t count, bool dynamic = false);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline std::uint32_t GetCount();
};