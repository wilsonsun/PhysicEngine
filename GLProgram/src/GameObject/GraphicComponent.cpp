#include "GraphicComponent.h"
#include "GameObject.h"
#include "GeometryComponent.h"
#include "GeometryCube.h"

GraphicComponent::GraphicComponent()
{
}

GraphicComponent::~GraphicComponent()
{
    delete Layout;
    delete VBO;
    delete VAO;
    delete texture;
    delete shader;
}

void GraphicComponent::Init()
{
    shader = new Shader("res/shaders/Basic.shader");
    texture = new Texture(TextureFilePath);
    VAO = new VertexArray();
    VBO = new VertexBuffer();
    Layout = new VertexBufferLayout();
}

void GraphicComponent::LoadVAO()
{
    if (owner->shape == SHAPE_CUBE)
    {
        GeometryCube *geometryCube = (GeometryCube*)owner->geometryComponent;

        float width = geometryCube->width;
        float height = geometryCube->height;
        float depth = geometryCube->depth;

        /*
        float CubeVertices[] = {
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 0.0f,
        0.5f*width, -0.5f*height, -0.5f*depth,  1.0f, 0.0f,
        0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
        0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
       -0.5f*width,  0.5f*height, -0.5f*depth,  0.0f, 1.0f,
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 0.0f,

       -0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,
        0.5f*width, -0.5f*height,  0.5f*depth,  1.0f, 0.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 1.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 1.0f,
       -0.5f*width,  0.5f*height,  0.5f*depth,  0.0f, 1.0f,
       -0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,

       -0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,
       -0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
       -0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,
       -0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,

        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,
        0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
        0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
        0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
        0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,

       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
        0.5f*width, -0.5f*height, -0.5f*depth,  1.0f, 1.0f,
        0.5f*width, -0.5f*height,  0.5f*depth,  1.0f, 0.0f,
        0.5f*width, -0.5f*height,  0.5f*depth,  1.0f, 0.0f,
       -0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,

       -0.5f*width,  0.5f*height, -0.5f*depth,  0.0f, 1.0f,
        0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,
       -0.5f*width,  0.5f*height,  0.5f*depth,  0.0f, 0.0f,
       -0.5f*width,  0.5f*height, -0.5f*depth,  0.0f, 1.0f
        };
        */
        VBO->LoadData(geometryCube->verticesGraphic, sizeof(geometryCube->verticesGraphic));

        Layout->AddFloat(3);
        Layout->AddFloat(2);
        VAO->AddBuffer(VBO, Layout);    
    }
}
