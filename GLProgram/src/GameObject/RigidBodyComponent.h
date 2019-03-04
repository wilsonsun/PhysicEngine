#pragma once

#include <glm.hpp>
#include <vector>

class GameObject;

class RigidBodyComponent
{
public:
    RigidBodyComponent();
    ~RigidBodyComponent();

    GameObject *owner;

    std::vector<glm::vec4> vertices;
    glm::vec3 minXYZ;
    glm::vec3 maxXYZ;

    float mass;
    float inverseMass;
    glm::mat3 localInverseInertiaTensor;
    glm::mat3 globalInverseInertiaTensor;

    glm::vec3 globalCentroid;
    glm::vec3 localCentroid;

    glm::mat3 orientation;
    glm::vec3 linearVelocity;
    glm::vec3 angularVelocity;

    glm::vec3 forceAccumulator;
    glm::vec3 torqueAccumulator;

    void ApplyForce(const glm::vec3 &force, const glm::vec3 &torque);
    void UpdateVertices();

private:
    float findMin(const std::vector<glm::vec4>& vertices, const char flag);
    float findMax(const std::vector<glm::vec4>& vertices, const char flag);
};