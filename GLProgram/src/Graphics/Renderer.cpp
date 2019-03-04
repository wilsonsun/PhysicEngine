#include "Renderer.h"

#include <iostream>

#include "DebugRenderInterface.h"
#include "GeometryComponent.h"
#include "GeometryCube.h"

#define GLM_TO_FLOAT(v, glmV) \
    v[0] = glmV.x; \
    v[1] = glmV.y; \
    v[2] = glmV.z


Renderer::Renderer()
{
    debugRenderer = new DebugRenderInterface();
    dd::initialize(debugRenderer);
}

Renderer::~Renderer()
{
    delete debugRenderer;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::SetDebugDrawMVP(const glm::mat4 &mvp)
{
    debugRenderer->mvpMatrix = mvp;
}

void Renderer::DebugDraw()
{

}

void Renderer::DrawBackGround()
{
    //                                 y 
    dd::xzSquareGrid(-50.0f, 50.0f, 0.0f, 1.0f, dd::colors::Green); // Grid from -50 to +50 in both X & Z
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawCube(const VertexArray &va, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void Renderer::DebugDrawCube(GameObject* gameObject)
{
    // Debug Draw
    ddVec3 origin = { gameObject->position.x, gameObject->position.y, gameObject->position.z };
    GeometryCube *geometryCube = (GeometryCube*)gameObject->geometryComponent;

    glm::vec3 width = { geometryCube->width, 0, 0 };
    glm::vec3 height = { 0,  geometryCube->height, 0 };
    glm::vec3 depth = { 0, 0,  geometryCube->depth };

    //glm::vec3 VA = (glm::mat3)gameObject->orientation;
    glm::vec3 VA = gameObject->position + (glm::mat3)gameObject->orientation*(0.5f*height + 0.5f*width + 0.5f*depth);
    float VA_F[3];
    GLM_TO_FLOAT(VA_F, VA);
    
    glm::vec3 VB = gameObject->position + (glm::mat3)gameObject->orientation*(0.5f*height - 0.5f*width + 0.5f*depth);
    //VB *= gameObject->orientation;
    //VB = (glm::mat3)gameObject->orientation * VB;
    float VB_F[3];
    GLM_TO_FLOAT(VB_F, VB);
    
    glm::vec3 VC = gameObject->position + (glm::mat3)gameObject->orientation*(0.5f*height - 0.5f*width - 0.5f*depth);
    //VC *= gameObject->orientation;
    float VC_F[3];
    GLM_TO_FLOAT(VC_F, VC);

    glm::vec3 VD = gameObject->position + (glm::mat3)gameObject->orientation*(0.5f*height + 0.5f*width - 0.5f*depth);
   // VD *= gameObject->orientation;
    float VD_F[3];
    GLM_TO_FLOAT(VD_F, VD);

    glm::vec3 VE = gameObject->position + (glm::mat3)gameObject->orientation*(-0.5f*height + 0.5f*width + 0.5f*depth);
   // VE *= gameObject->orientation;
    float VE_F[3];
    GLM_TO_FLOAT(VE_F, VE);

    glm::vec3 VF = gameObject->position + (glm::mat3)gameObject->orientation*(-0.5f*height - 0.5f*width + 0.5f*depth);
  //  VF *= gameObject->orientation;
    float VF_F[3];
    GLM_TO_FLOAT(VF_F, VF);

    glm::vec3 VG = gameObject->position + (glm::mat3)gameObject->orientation*(-0.5f*height - 0.5f*width - 0.5f*depth);
  //  VG *= gameObject->orientation;
    float VG_F[3];
    GLM_TO_FLOAT(VG_F, VG);

    glm::vec3 VH = gameObject->position + (glm::mat3)gameObject->orientation*(-0.5f*height + 0.5f*width - 0.5f*depth);
  //  VH *= gameObject->orientation;
    float VH_F[3];
    GLM_TO_FLOAT(VH_F, VH);
    
    dd::line(VA_F, VB_F, dd::colors::Red);
    dd::line(VB_F, VC_F, dd::colors::Red);
    dd::line(VC_F, VD_F, dd::colors::Red);
    dd::line(VD_F, VA_F, dd::colors::Red);
    
    dd::line(VA_F, VE_F, dd::colors::Red);
    dd::line(VB_F, VF_F, dd::colors::Red);
    dd::line(VC_F, VG_F, dd::colors::Red);
    dd::line(VD_F, VH_F, dd::colors::Red);
    
    dd::line(VE_F, VF_F, dd::colors::Red);
    dd::line(VF_F, VG_F, dd::colors::Red);
    dd::line(VG_F, VH_F, dd::colors::Red);
    dd::line(VH_F, VE_F, dd::colors::Red);
    
    glm::vec3 forward = {1.0f, 0, 0};
    glm::vec3 up = { 0, 1.0f, 0 };
    glm::vec3 right = { 0, 0, 1.0f };

    glm::vec3 FV = gameObject->position + (glm::mat3)gameObject->orientation*forward;
    glm::vec3 UV = gameObject->position + (glm::mat3)gameObject->orientation*up;
    glm::vec3 RV = gameObject->position + (glm::mat3)gameObject->orientation*right;

    float Center_F[3];
    float FV_F[3];
    float UV_F[3];
    float RV_F[3];

    GLM_TO_FLOAT(Center_F, gameObject->position);
    GLM_TO_FLOAT(FV_F, FV);
    GLM_TO_FLOAT(UV_F, UV);
    GLM_TO_FLOAT(RV_F, RV);

    dd::line(Center_F, FV_F, dd::colors::Blue);
    dd::line(Center_F, UV_F, dd::colors::Yellow);
    dd::line(Center_F, RV_F, dd::colors::Green);
}

void Renderer::DebugDrawPoint(glm::vec3 & position, int duration)
{
    float Center[3];
    GLM_TO_FLOAT(Center, position);
    dd::sphere(Center, dd::colors::Yellow, 1.0f, duration);
}

void Renderer::DebugFlush()
{
    dd::flush(getTimeMilliseconds());
}

