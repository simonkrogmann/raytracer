#pragma once

#include "Vector.h"

class Line
{
public:
    Line(vec3<float> origin, vec3<float> direction)
        : origin{origin}, direction{direction.normalize()}
    {
    }
    ~Line();

    static Line throughPoints(const vec3<float>& point1,
                              const vec3<float>& point2)
    {
        return Line{point1, point2 - point1};
    }

    vec3<float> at(const double& parameter) const
    {
        return origin + direction * parameter;
    }
    // returns intersection closest to origin (only on positive side)

    const vec3<float> origin;
    const vec3<float> direction;
};
