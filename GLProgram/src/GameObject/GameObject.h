#pragma once
#include <glm.hpp>

class GraphicComponent;
class RigidBodyComponent;
class GeometryComponent;
class OctTree;

enum SHAPE
{
    SHAPE_CUBE
};

class GameObject
{
public:
    GameObject();
    ~GameObject();

    SHAPE shape;

    // Position
    //float posX;
    //float posY;
    //float posZ;
    OctTree* octTree;

    glm::vec3 position;

    // Orientation
    glm::mat4 orientation;

    GraphicComponent *graphicComponent;
    RigidBodyComponent *rigidbodyComponent;
    GeometryComponent *geometryComponent;
};
