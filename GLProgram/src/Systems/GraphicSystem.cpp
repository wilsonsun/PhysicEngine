#include <glm.hpp>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "GameObjectManager.h"
#include "GameObject.h"
#include "GeometryComponent.h"
#include "GeometryCube.h"
#include "GraphicComponent.h"

#include "Global.h"
#include "GraphicSystem.h"
#include "Shader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Camera.h"

GraphicSystem::GraphicSystem()
{
    renderer = new Renderer();
}

GraphicSystem::~GraphicSystem()
{
    delete renderer;
}

void GraphicSystem::Init()
{

    std::list<GameObject*> GOJList = GameObjectManager::GetGOJList();
    for (auto it = GOJList.begin(); it != GOJList.end(); ++it)
    {
        (*it)->graphicComponent->LoadVAO();
    }
}

void GraphicSystem::Update()
{
    renderer->Clear();

    std::list<GameObject*> GOJList = GameObjectManager::GetGOJList();
    for (auto it = GOJList.begin(); it != GOJList.end(); ++it)
    {
        // create transformations
        VertexArray *VAO = (*it)->graphicComponent->VAO;
        Shader *shader = (*it)->graphicComponent->shader;
        Texture *texture = (*it)->graphicComponent->texture;

        shader->Bind();
        texture->Bind();
        shader->SetUniform1i("u_Texture", 0);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(CAMERA.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderID(), "projection"), 1, GL_FALSE, &projection[0][0]);

        // CAMERA/view transformation
        glm::mat4 view = CAMERA.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderID(), "view"), 1, GL_FALSE, &view[0][0]);

        // Draw Grid
        renderer->SetDebugDrawMVP(projection * view);

        if ((*it)->shape == SHAPE_CUBE)
        {
            GeometryCube *geometryCube = (GeometryCube*)(*it)->geometryComponent;
            float width = geometryCube->width;
            float height = geometryCube->height;
            float depth = geometryCube->depth;

            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            glm::vec3 position = glm::vec3((*it)->position.x, (*it)->position.y, (*it)->position.z);
            model = glm::translate(model, position);

            //(*it)->orientation = glm::rotate((*it)->orientation, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
            model *= (*it)->orientation;

            //model = glm::scale(model, glm::vec3(width, height, depth));
            //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader->GetShaderID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

            if (DEBUG_DRAW_ENABLE)
            {
                renderer->DebugDrawCube((*it));
            }

            if (TEXTURE_DRAW_ENABLE)
            {
                renderer->DrawCube(*VAO, *shader);
                shader->Unbind();
                texture->Unbind();
            }
        }
    }

    renderer->DrawBackGround();

    renderer->DebugFlush();
    
}

