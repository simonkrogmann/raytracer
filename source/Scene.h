#pragma once

#include <memory>
#include <vector>

#include "Line.h"
#include "SceneObject.h"
#include "Vector.h"

struct Intersection
{
    vec3<float> intersection;
    SceneObject *object;
    bool isValid() const { return intersection.isValid(); }
};

class Scene
{
public:
    Scene(const vec3<float> &light, const vec3<float> &background,
          const unsigned int &maxBounces)
        : light{light}, m_background{background}, m_maxBounces{maxBounces}
    {
    }
    ~Scene() {}
    void add(std::unique_ptr<SceneObject> &&s)
    {
        objects.push_back(std::move(s));
    }
    Intersection intersect(const Line &l,
                           const float &closerThanSquared =
                               std::numeric_limits<float>::infinity()) const;
    vec3<float> findColorFor(const Line &ray, const unsigned int &bounces = 0,
                             const vec3<float> &intensity = {1.0f}) const;

private:
    vec3<float> light;
    std::vector<std::unique_ptr<SceneObject>> objects;
    vec3<float> m_background;
    unsigned int m_maxBounces;
};
