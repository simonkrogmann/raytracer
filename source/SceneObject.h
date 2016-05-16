#pragma once

#include <iostream>

#include "Line.h"
#include "Material.h"

#define BIAS 0.01

class SceneObject
{
public:
    SceneObject() {}
    SceneObject(Material material) : material{material} {}
    virtual ~SceneObject();

    virtual bool hasIntersectionWith(const Line& l) const = 0;
    virtual vec3<float> intersect(const Line& l) const = 0;

    virtual vec3<float> normalAt(const vec3<float>& point) const = 0;

    Line reflect(const Line& ray, const vec3<float>& at) const;

    Material material;
};
