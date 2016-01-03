#include "Sphere.h"

#include <iostream>

Sphere::~Sphere()
{
}

bool Sphere::hasIntersectionWith(const Line& l) const
{
    return intersect(l).isValid();
}

vec3<float> Sphere::intersect(const Line& l) const
{
    const vec3<float> to_origin = l.origin - center;
    const double determinant = std::pow(l.direction.dot(to_origin), 2) -
                               std::pow(to_origin.length(), 2) +
                               std::pow(radius, 2);
    if (determinant < 0)
    {
        return vec3<float>::invalidVector();
    }
    const double root = std::sqrt(determinant);
    const double mid = -l.direction.dot(to_origin);
    double solution;
    if (mid - root > BIAS)
    {
        solution = mid - root;
    }
    else if (mid + root > BIAS)
    {
        solution = mid + root;
    }
    else
    {
        return vec3<float>::invalidVector();
    }
    return l.at(solution);
}
