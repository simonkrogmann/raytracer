#pragma once

#include "SceneObject.h"
#include "Vector.h"

class Sphere : public SceneObject
{
public:
    Sphere(vec3<float> center, float radius, Material material)
        : SceneObject{material}, center{center}, radius{radius}
    {
    }
    ~Sphere();

    vec3<float> normalAt(const vec3<float>& p) const override
    {
        return (p - center).normalize();
    }
    bool hasIntersectionWith(const Line& l) const override;
    vec3<float> intersect(const Line& l) const override;

private:
    vec3<float> center;
    float radius;
};
