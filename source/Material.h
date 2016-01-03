#pragma once

#include "Vector.h"

class Material
{
public:
    Material();
    Material(vec3<float> ambient, vec3<float> diffuse, vec3<float> specular)
        : ambient{ambient}, diffuse{diffuse}, specular{specular}
    {
    }
    ~Material();

    vec3<float> ambient;
    vec3<float> diffuse;
    vec3<float> specular;
};
