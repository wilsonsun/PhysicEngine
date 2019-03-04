#include "PhysicManager.h"

#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <list>

#include "debug_draw.hpp"

#include "Global.h"

#include "OctTree.h"
#include "BoundingBox.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"

GameObject* PhysicManager::mousePick_SelectedObject;
glm::vec3 PhysicManager::mousePick_ForceDir;
float PhysicManager::mousePick_RayDistance;
int PhysicManager::potentialContactListSize;

PhysicManager::PhysicManager()
{
}

PhysicManager::~PhysicManager()
{
}

bool PhysicManager::RayToCubeIntersectionTest(
    glm::vec3 rayOrigin,        // Ray origin, in world space
    glm::vec3 rayDir,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
    glm::vec3 cubeMin,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
    glm::vec3 cubeMax,          // Maximum X,Y,Z coords. Often cubeMin*-1 if your mesh is centered, but it's not always the case.
    glm::mat4 modelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
    float& intersectionDistance // Output : distance between rayOrigin and the intersection with the OBB
)
{
    // Intersection method from Real-Time Rendering and Essential Mathematics for Games

    float tMin = 0.0f;
    float tMax = 100000.0f;

    glm::vec3 OBBposition_worldspace(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

    glm::vec3 delta = OBBposition_worldspace - rayOrigin;

    // Test intersection with the 2 planes perpendicular to the OBB's X axis
    {
        glm::vec3 xaxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
        float e = glm::dot(xaxis, delta);
        float f = glm::dot(rayDir, xaxis);

        if (fabs(f) > 0.001f) { // Standard case

            float t1 = (e + cubeMin.x) / f; // Intersection with the "left" plane
            float t2 = (e + cubeMax.x) / f; // Intersection with the "right" plane
            // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

            // We want t1 to represent the nearest intersection, 
            // so if it's not the case, invert t1 and t2
            if (t1 > t2) {
                float w = t1; t1 = t2; t2 = w; // swap t1 and t2
            }

            // tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
            if (t2 < tMax)
                tMax = t2;
            // tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
            if (t1 > tMin)
                tMin = t1;

            // And here's the trick :
            // If "far" is closer than "near", then there is NO intersection.
            // See the images in the tutorials for the visual explanation.
            if (tMax < tMin)
                return false;

        }
        else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
            if (-e + cubeMin.x > 0.0f || -e + cubeMax.x < 0.0f)
                return false;
        }
    }


    // Test intersection with the 2 planes perpendicular to the OBB's Y axis
    // Exactly the same thing than above.
    {
        glm::vec3 yaxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
        float e = glm::dot(yaxis, delta);
        float f = glm::dot(rayDir, yaxis);

        if (fabs(f) > 0.001f) {

            float t1 = (e + cubeMin.y) / f;
            float t2 = (e + cubeMax.y) / f;

            if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin)
                tMin = t1;
            if (tMin > tMax)
                return false;

        }
        else {
            if (-e + cubeMin.y > 0.0f || -e + cubeMax.y < 0.0f)
                return false;
        }
    }


    // Test intersection with the 2 planes perpendicular to the OBB's Z axis
    // Exactly the same thing than above.
    {
        glm::vec3 zaxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
        float e = glm::dot(zaxis, delta);
        float f = glm::dot(rayDir, zaxis);

        if (fabs(f) > 0.001f) {

            float t1 = (e + cubeMin.z) / f;
            float t2 = (e + cubeMax.z) / f;

            if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin)
                tMin = t1;
            if (tMin > tMax)
                return false;

        }
        else {
            if (-e + cubeMin.z > 0.0f || -e + cubeMax.z < 0.0f)
                return false;
        }
    }

    intersectionDistance = tMin;
    return true;
}

bool PhysicManager::RayCastAll(glm::vec3 rayOrigin, glm::vec3 rayDir)
{
    float min_t = std::numeric_limits<float>::max();

    std::list<GameObject*> GOJList = GameObjectManager::GetGOJList();
    for (auto it = GOJList.begin(); it != GOJList.end(); ++it)
    {
        GameObject* gameObject = (*it);
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model *= gameObject->orientation;

        float distance;
        if (PhysicManager::RayToCubeIntersectionTest(rayOrigin,
            rayDir,
            gameObject->rigidbodyComponent->minXYZ,
            gameObject->rigidbodyComponent->maxXYZ,
            model,
            distance))
        {
            if (distance < min_t)
            {
                mousePick_SelectedObject = (*it);
                min_t = distance;
            }
        }        
    }

    if (min_t == std::numeric_limits<float>::max())
    {
        mousePick_SelectedObject = NULL;
        return false;
    }

    mousePick_RayDistance = min_t;
    glm::vec3 hitPoint = rayOrigin + rayDir * min_t;
    float _hitPoint[3];
    _hitPoint[0] = hitPoint.x;
    _hitPoint[1] = hitPoint.y;
    _hitPoint[2] = hitPoint.z;

    // Debug Draw
    dd::sphere(_hitPoint, dd::colors::Red, 0.01f, 500);
    return true;
}

void PhysicManager::MousePickUpdate(bool isPressed)
{
    if (!isPressed || mousePick_SelectedObject == NULL)
        return;

    RigidBodyComponent* rigidbodyComponent = mousePick_SelectedObject->rigidbodyComponent;
    rigidbodyComponent->ApplyForce(mousePick_ForceDir*MOUSEPICK_FORCE, glm::vec3(0.0, 0.0, 0.0));
}

void PhysicManager::MousePickHandle(const glm::vec3 &worldPosition)
{
    //std::cout << "Mouse Pick Handle" << std::endl;
    glm::vec3 rayDir = glm::normalize(worldPosition - CAMERA.Position);
    glm::vec3 targetPosition = CAMERA.Position + rayDir * mousePick_RayDistance;

#define GLM_TO_FLOAT(v, glmV) \
            v[0] = glmV.x; \
            v[1] = glmV.y; \
            v[2] = glmV.z

    float From[3];
    float To[3];
    //float Target[3];
    GLM_TO_FLOAT(From, mousePick_SelectedObject->position);
    GLM_TO_FLOAT(To, targetPosition);

    dd::line(From, To, dd::colors::Yellow, 1);
    //dd::sphere(To, dd::colors::Yellow, 1.0f, 1000);
    if (glm::distance(targetPosition, mousePick_SelectedObject->position) < 0.5f)
    {
        mousePick_ForceDir = glm::vec3(0.0, 0.0, 0.0);
    }
    else
    {
        mousePick_ForceDir = glm::normalize(targetPosition - mousePick_SelectedObject->position);
    }

}

void PhysicManager::DrawOctTree(OctTree* octTree)
{
    if (octTree == NULL)
        return;

    octTree->boundingBox->DebugDraw();

    for (unsigned i = 0; i < 8; ++i)
    {
        DrawOctTree(octTree->subTrees[i]);
    }
}
