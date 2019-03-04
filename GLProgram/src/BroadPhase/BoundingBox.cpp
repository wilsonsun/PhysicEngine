#include "BoundingBox.h"

#include "debug_draw.hpp"

#include "GameObject.h"
#include "GeometryCube.h"
#include "RigidBodyComponent.h"

BoundingBox::BoundingBox(glm::vec3 _position, float _width, float _height, float _depth) 
    : position(_position), width(_width), height(_height), depth(_depth)
{
    Init();
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::Init()
{
    // Find mins and maxs
    mins.x = position.x - width / 2.0f;
    mins.y = position.y - height / 2.0f;
    mins.z = position.z - depth / 2.0f;

    maxs.x = position.x + width / 2.0f;
    maxs.y = position.y + height / 2.0f;
    maxs.z = position.z + depth / 2.0f;
}

bool BoundingBox::isContain(GameObject* gameObject)
{
    if (gameObject->shape == SHAPE_CUBE && gameObject->rigidbodyComponent != NULL)
    {
        RigidBodyComponent* rigidbodyComponent = gameObject->rigidbodyComponent;

        float minX = rigidbodyComponent->minXYZ.x;
        float minY = rigidbodyComponent->minXYZ.y;
        float minZ = rigidbodyComponent->minXYZ.z;
        float maxX = rigidbodyComponent->maxXYZ.x;
        float maxY = rigidbodyComponent->maxXYZ.y;
        float maxZ = rigidbodyComponent->maxXYZ.z;
            /*
        float minX = findMin(rigidbodyComponent->vertices, 'x');
        float minY = findMin(rigidbodyComponent->vertices, 'y');
        float minZ = findMin(rigidbodyComponent->vertices, 'z');
        float maxX = findMax(rigidbodyComponent->vertices, 'x');
        float maxY = findMax(rigidbodyComponent->vertices, 'y');
        float maxZ = findMax(rigidbodyComponent->vertices, 'z');
        */
        // Self bounding box
        float left = position.x - width / 2.0f;
        float right = position.x + width / 2.0f;
        float up = position.y + height / 2.0f;
        float down = position.y - height / 2.0f;
        float front = position.z - depth / 2.0f;
        float back = position.z + depth / 2.0f;

        //if (minX >= right || maxX <= left || minY >= up || maxY <= down || minZ <= front || maxZ >= back)
          //  return false;

        if (minX >= left && maxX <= right && minY >= down && maxY <= up && minZ >= front && maxZ <= back)
            return true;
            
        return false;
    }


    return false;
}

void BoundingBox::DebugDraw()
{
#define GLM_TO_FLOAT(v, glmV) \
            v[0] = glmV.x; \
            v[1] = glmV.y; \
            v[2] = glmV.z

    float f_mins[3];
    float f_maxs[3];

    GLM_TO_FLOAT(f_mins, mins);
    GLM_TO_FLOAT(f_maxs, maxs);

    dd::aabb(f_mins, f_maxs, dd::colors::White, 1);
}
