#pragma once

class VertexBuffer
{
private:
    unsigned int m_RendererID;

public:
    VertexBuffer();
    ~VertexBuffer();

    void LoadData(const void* data, unsigned int size) const;
    void Bind() const;
    void Unbind() const;
};
