#pragma once

#include "Shader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"

class GameObject;

class GraphicComponent
{
public:
    GraphicComponent();
    ~GraphicComponent();

    GameObject *owner;

    Shader *shader;
    Texture *texture;

    VertexArray *VAO;
    VertexBuffer *VBO;
    VertexBufferLayout *Layout;

    std::string TextureFilePath;

    void Init();
    void LoadVAO();
};
