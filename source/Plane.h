#pragma once
#include "SceneObject.h"
class Plane : public SceneObject
{
public:
    Plane(vec3<float> origin, vec3<float> normal, Material material)
        : SceneObject{material}, origin{origin}, normal{normal.normalize()}
    {
    }
    ~Plane();

    vec3<float> normalAt(const vec3<float>&) const override { return normal; }
    bool hasIntersectionWith(const Line& l) const override;
    vec3<float> intersect(const Line& l) const override;

private:
    vec3<float> origin;
    vec3<float> normal;
};
