#pragma once

class IndexBuffer
{
    unsigned int _id;
    unsigned int _count;

public:
    explicit IndexBuffer(const unsigned int *data, unsigned int count, bool dynamic = false);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount();
};