#include "Plane.h"

Plane::~Plane()
{
}

bool Plane::hasIntersectionWith(const Line& l) const
{
    return intersect(l).isValid();
}

vec3<float> Plane::intersect(const Line& l) const
{
    const float denominator = l.direction.dot(normal);
    if (denominator == 0)
    {
        return vec3<float>::invalidVector();
    }
    const float numerator = (origin - l.origin).dot(normal);

    if (numerator / denominator < BIAS)
    {
        return vec3<float>::invalidVector();
    }
    return l.at(numerator / denominator);
}
