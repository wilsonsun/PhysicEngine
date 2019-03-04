#include "GameObject.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "GraphicComponent.h"
#include "RigidBodyComponent.h"
#include "GeometryComponent.h"

GameObject::GameObject() : octTree(NULL), 
                           geometryComponent(NULL), 
                           graphicComponent(NULL),
                           rigidbodyComponent(NULL), 
                           orientation(glm::mat4(1.0f))
{
    //orientation = glm::mat4(1.0f);
   // orientation = glm::rotate(orientation, 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    //geometryComponent = NULL;
    //graphicComponent = NULL;
    //rigidbodyComponent = NULL;
}

GameObject::~GameObject()
{
    if (rigidbodyComponent)
        delete rigidbodyComponent;

    if (graphicComponent)
        delete graphicComponent;

    if (geometryComponent)
        delete geometryComponent;
}
