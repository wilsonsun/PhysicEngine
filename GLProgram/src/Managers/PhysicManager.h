#pragma once
#include <glm.hpp>

class OctTree;
class GameObject;

class PhysicManager
{
public:
    PhysicManager();
    ~PhysicManager();

    static bool RayToCubeIntersectionTest(
        glm::vec3 rayOrigin,        // Ray origin, in world space
        glm::vec3 rayDir,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
        glm::vec3 cubeMin,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
        glm::vec3 cubeMax,          // Maximum X,Y,Z coords. Often cubeMin*-1 if your mesh is centered, but it's not always the case.
        glm::mat4 modelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
        float& intersectionDistance // Output : distance between rayOrigin and the intersection with the OBB
    );

    static bool RayCastAll(
        glm::vec3 rayOrigin,        // Ray origin, in world space
        glm::vec3 rayDir     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
    );

    static void MousePickUpdate(bool isPressed);
    static GameObject* GetMousePickObject() { return mousePick_SelectedObject;  }
    // Update Force Direction
    static void MousePickHandle(const glm::vec3 &worldPosition);
    
    static void DrawOctTree(OctTree* octTree);

    static void SetPotentialContatctListSize(int size) { potentialContactListSize = size; }
    static int GetPotentialContatctListSize() { return potentialContactListSize; }

private:
    static GameObject* mousePick_SelectedObject;
    static glm::vec3 mousePick_ForceDir;
    static float mousePick_RayDistance;
    static int potentialContactListSize;
};
