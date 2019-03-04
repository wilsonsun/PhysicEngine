#include <iostream>

#include "RigidBodyComponent.h"
#include "GameObject.h"
#include "GeometryCube.h"


RigidBodyComponent::RigidBodyComponent()
{
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::ApplyForce(const glm::vec3 & force, const glm::vec3 & torque)
{
    //std::cout << "Apply force : " << tcount << std::endl;
    forceAccumulator += force;
    //m_torqueAccumulator += (at - m_globalCentroid).Cross(f);   
}

void RigidBodyComponent::UpdateVertices()
{
    GameObject* gameObject = owner;

    vertices.clear();

    if (gameObject->shape == SHAPE_CUBE)
    {
        GeometryCube* geometryCube = (GeometryCube*)gameObject->geometryComponent;
        glm::vec3 center = gameObject->position;
        float halfWidth = geometryCube->width / 2.0f;
        float halfHeight = geometryCube->height / 2.0f;
        float halfDepth = geometryCube->depth / 2.0f;

        glm::vec4 vertexA = glm::vec4(center.x + halfWidth, center.y + halfHeight, center.z + halfDepth, 1.0f) * gameObject->orientation;
        glm::vec4 vertexB = glm::vec4(center.x - halfWidth, center.y + halfHeight, center.z + halfDepth, 1.0f) * gameObject->orientation;
        glm::vec4 vertexC = glm::vec4(center.x + halfWidth, center.y + halfHeight, center.z - halfDepth, 1.0f) * gameObject->orientation;
        glm::vec4 vertexD = glm::vec4(center.x - halfWidth, center.y + halfHeight, center.z - halfDepth, 1.0f) * gameObject->orientation;
        glm::vec4 vertexE = glm::vec4(center.x + halfWidth, center.y - halfHeight, center.z + halfDepth, 1.0f) * gameObject->orientation;
        glm::vec4 vertexF = glm::vec4(center.x - halfWidth, center.y - halfHeight, center.z + halfDepth, 1.0f) * gameObject->orientation;
        glm::vec4 vertexG = glm::vec4(center.x + halfWidth, center.y - halfHeight, center.z - halfDepth, 1.0f) * gameObject->orientation;
        glm::vec4 vertexH = glm::vec4(center.x - halfWidth, center.y - halfHeight, center.z - halfDepth, 1.0f) * gameObject->orientation;

        vertices.push_back(vertexA);
        vertices.push_back(vertexB);
        vertices.push_back(vertexC);
        vertices.push_back(vertexD);
        vertices.push_back(vertexE);
        vertices.push_back(vertexF);
        vertices.push_back(vertexG);
        vertices.push_back(vertexH);

        float minX = findMin(vertices, 'x');
        float minY = findMin(vertices, 'y');
        float minZ = findMin(vertices, 'z');
        float maxX = findMax(vertices, 'x');
        float maxY = findMax(vertices, 'y');
        float maxZ = findMax(vertices, 'z');

        minXYZ = glm::vec3(minX, minY, minZ);
        maxXYZ = glm::vec3(maxX, maxY, maxZ);
    }
    
}


float RigidBodyComponent::findMin(const std::vector<glm::vec4>& vertices, const char flag)
{
    float min = std::numeric_limits<float>::max();
    for (unsigned i = 0; i < vertices.size(); ++i)
    {
        if (flag == 'x')
        {
            if (vertices[i].x < min)
            {
                min = vertices[i].x;
            }
        }
        else if (flag == 'y')
        {
            if (vertices[i].y < min)
            {
                min = vertices[i].y;
            }
        }
        else if (flag == 'z')
        {
            if (vertices[i].z < min)
            {
                min = vertices[i].z;
            }
        }
    }

    return min;
}

float RigidBodyComponent::findMax(const std::vector<glm::vec4>& vertices, const char flag)
{
    float max = -std::numeric_limits<float>::max();
    for (unsigned i = 0; i < vertices.size(); ++i)
    {
        if (flag == 'x')
        {
            if (vertices[i].x > max)
            {
                max = vertices[i].x;
            }
        }
        else if (flag == 'y')
        {
            if (vertices[i].y > max)
            {
                max = vertices[i].y;
            }
        }
        else if (flag == 'z')
        {
            if (vertices[i].z > max)
            {
                max = vertices[i].z;
            }
        }
    }

    return max;
}