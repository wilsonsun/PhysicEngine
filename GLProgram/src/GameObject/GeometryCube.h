#pragma once
#include "GeometryComponent.h"

class GeometryCube : public GeometryComponent
{
public:
    GeometryCube();
    ~GeometryCube();

    float width;
    float height;
    float depth;

    float verticesGraphic[720];
    void Init();
};
