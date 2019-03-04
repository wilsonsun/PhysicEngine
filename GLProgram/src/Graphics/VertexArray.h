#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_RendererID;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& Layout);
    void AddBuffer(VertexBuffer *vb, VertexBufferLayout *Layout);
    void Bind() const;
    void Unbind() const;
};
