#include <glm.hpp>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <vector>

#include "PhysicSystem.h"
#include "Global.h"
#include "PhysicManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"

#include "RigidBodyComponent.h"
#include "GeometryCube.h"

#include "OctTree.h"
#include "BoundingBox.h"

PhysicSystem::PhysicSystem():octTree(NULL)
{
}

PhysicSystem::~PhysicSystem()
{
    if (octTree != NULL)
        delete octTree;
}

void PhysicSystem::Init()
{
    /* RigidBody Vertices Init */
    std::list<GameObject*> GOJList = GameObjectManager::GetGOJList();
    for (auto it = GOJList.begin(); it != GOJList.end(); ++it)
    {
        RigidBodyComponent* rigidbodyComponent = (*it)->rigidbodyComponent;
        rigidbodyComponent->UpdateVertices();
    }

    /* BoardPhase Init */
    BoundingBox* rootBoundingBox = new BoundingBox(glm::vec3(0.0, 10.0, 0.0), OCTTREE_MAXSIZE, OCTTREE_MAXSIZE, OCTTREE_MAXSIZE);
    octTree = new OctTree(rootBoundingBox, GameObjectManager::GetGOJList(), NULL);
    octTree->BuildTree();
}


void PhysicSystem::Update()
{
    // BroadPhase Update
    octTree->Update();
    std::list<Contact*> potentialContacts = octTree->GeneratePotentialContactList();

    // For UI
    PhysicManager::SetPotentialContatctListSize(potentialContacts.size());

    PhysicManager::DrawOctTree(octTree);

    std::list<GameObject*> GOJList = GameObjectManager::GetGOJList();
    for (auto it = GOJList.begin(); it != GOJList.end(); ++it)
    {
        //(*it)->orientation = glm::rotate((*it)->orientation, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
        RigidBodyComponent* rigidbodyComponent = (*it)->rigidbodyComponent;
        rigidbodyComponent->linearVelocity += rigidbodyComponent->inverseMass * (rigidbodyComponent->forceAccumulator * DELTATIME);

        //printf("forceAccumlator : %f, %f, %f\n", rigidbodyComponent->forceAccumulator.x, rigidbodyComponent->forceAccumulator.y, rigidbodyComponent->forceAccumulator.z);
        //printf("Linear velocity : %f, %f, %f\n", rigidbodyComponent->linearVelocity.x, rigidbodyComponent->linearVelocity.y, rigidbodyComponent->linearVelocity.z);
        // integrate angular velocity
        /*
        body.m_angularVelocity +=
            body.m_globalInverseInertiaTensor
            * (body.m_torqueAccumulator * dt);
            */
        // zero out accumulated force and torque
        rigidbodyComponent->forceAccumulator = glm::vec3(0.0, 0.0, 0.0);
        rigidbodyComponent->torqueAccumulator = glm::vec3(0.0, 0.0, 0.0);

        // integrate position
        //m_globalCentroid += m_linearVelocity * dt;
        rigidbodyComponent->owner->position += rigidbodyComponent->linearVelocity * DELTATIME;

        rigidbodyComponent->linearVelocity *= 0.9f;
        // integrate orientation
        /*
        const Vec3 axis = m_angularVelocity.Normalized();
        const float angle = m_angularVelocity.Length() * dt;
        m_orientation = RotationMatrix(axis, angle) * m_orientation;
        */

        // update physical properties
        //body.UpdateOrientation();
        //body.UpdatePositionFromGlobalCentroid();
        rigidbodyComponent->UpdateVertices();
    }
}


