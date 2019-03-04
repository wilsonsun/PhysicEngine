#include "GeometryCube.h"
#include <iostream>

GeometryCube::GeometryCube()
{
}

GeometryCube::~GeometryCube()
{
}

void GeometryCube::Init()
{
    float temp[] = {
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 0.0f,
        0.5f*width, -0.5f*height, -0.5f*depth,  1.0f, 0.0f,
        0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
        0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
       -0.5f*width,  0.5f*height, -0.5f*depth,  0.0f, 1.0f,
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 0.0f,

       -0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,
        0.5f*width, -0.5f*height,  0.5f*depth,  1.0f, 0.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 1.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 1.0f,
       -0.5f*width,  0.5f*height,  0.5f*depth,  0.0f, 1.0f,
       -0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,

       -0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,
       -0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
       -0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,
       -0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,

        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,
        0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
        0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
        0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
        0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,

       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,
        0.5f*width, -0.5f*height, -0.5f*depth,  1.0f, 1.0f,
        0.5f*width, -0.5f*height,  0.5f*depth,  1.0f, 0.0f,
        0.5f*width, -0.5f*height,  0.5f*depth,  1.0f, 0.0f,
       -0.5f*width, -0.5f*height,  0.5f*depth,  0.0f, 0.0f,
       -0.5f*width, -0.5f*height, -0.5f*depth,  0.0f, 1.0f,

       -0.5f*width,  0.5f*height, -0.5f*depth,  0.0f, 1.0f,
        0.5f*width,  0.5f*height, -0.5f*depth,  1.0f, 1.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,
        0.5f*width,  0.5f*height,  0.5f*depth,  1.0f, 0.0f,
       -0.5f*width,  0.5f*height,  0.5f*depth,  0.0f, 0.0f,
       -0.5f*width,  0.5f*height, -0.5f*depth,  0.0f, 1.0f
    };

    //std::cout << "size:" << sizeof(temp) << std::endl;
    
    for (unsigned int i = 0; i < sizeof(temp); ++i)
    {
        verticesGraphic[i] = temp[i];
    }
}
