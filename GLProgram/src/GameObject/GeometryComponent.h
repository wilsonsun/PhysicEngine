#pragma once
class GameObject;

class GeometryComponent
{
public:
    GeometryComponent();
    ~GeometryComponent();

    GameObject *owner;
};
