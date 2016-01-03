#pragma once

#include <vector>
#include <memory>

#include "Vector.h"
#include "SceneObject.h"
#include "Line.h"

#define BACKGROUND \
    vec3<float> { 52.0f }
#define MAX_BOUNCES 7

struct Intersection
{
    vec3<float> intersection;
    SceneObject *object;
    bool isValid() const { return intersection.isValid(); }
};

class Scene
{
public:
    Scene(vec3<float> light) : light{light} {}
    ~Scene() {}
    void add(std::unique_ptr<SceneObject> &&s)
    {
        objects.push_back(std::move(s));
    }
    Intersection intersect(const Line &l,
                           const float &closerThanSquared = INFINITY) const;
    vec3<float> findColorFor(const Line &ray, const int &bounces = 0,
                             const vec3<float> &intensity = {1.0f}) const;

private:
    vec3<float> light;
    std::vector<std::unique_ptr<SceneObject>> objects;
};
