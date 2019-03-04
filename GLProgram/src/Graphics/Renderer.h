#pragma once
#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GameObject.h"

#include <glm.hpp>

class DebugRenderInterface;

class Renderer
{
public:
    Renderer();
    ~Renderer();
    void Clear() const;
    void SetDebugDrawMVP(const glm::mat4 &mvp);
    void DebugDraw();
    void DrawBackGround();
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawCube(const VertexArray &va, const Shader &shader) const;
    void DebugDrawCube(GameObject *gameObject);
    void DebugDrawPoint(glm::vec3 &position, int duration);
    void DebugFlush();

private:
    DebugRenderInterface *debugRenderer;

    
};
