#pragma once

class Renderer;
class Shader;
class Texture;
class VertexArray;
class VertexBuffer;
class VertexBufferLayout;

// Debug Drawing
class DebugRenderInterface;

class GraphicSystem
{
public:
    GraphicSystem();
    ~GraphicSystem();
    void Init();
    void Update();

private:
    Renderer *renderer; 
};
