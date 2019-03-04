#pragma once
#include <glm.hpp>
#include <vector>

class GameObject;

class BoundingBox
{
public:
    BoundingBox(glm::vec3 _position, float _width, float _height, float _depth);
    ~BoundingBox();
    void Init();

    glm::vec3 position;
    glm::vec3 mins;
    glm::vec3 maxs;

    float width;
    float height;
    float depth;

    bool isContain(GameObject* gameObject);

    void DebugDraw();

private:
};
