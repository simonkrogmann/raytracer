#include "SceneObject.h"

SceneObject::~SceneObject()
{
}

Line SceneObject::reflect(const Line& ray, const vec3<float>& at) const
{
    return Line{at, ray.direction.reflect(normalAt(at))};
}
