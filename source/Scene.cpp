#include "Scene.h"

#include <iostream>
#include <algorithm>

Intersection Scene::intersect(const Line &l,
                              const float &closerThanSquared) const
{
    auto record = closerThanSquared;
    auto best = vec3<float>::invalidVector();
    SceneObject *bestObject = nullptr;
    for (const auto &s : objects)
    {
        const auto intersection = s->intersect(l);
        if (intersection.isValid() &&
            (l.origin - intersection).lengthSquared() < record)
        {
            record = (l.origin - intersection).lengthSquared();
            best = intersection;
            bestObject = s.get();
        }
    }
    return {best, bestObject};
}

vec3<float> Scene::findColorFor(const Line &ray, const int &bounces,
                                const vec3<float> &intensity) const
{
    if (bounces > MAX_BOUNCES || intensity.max() < 0.01)
    {
        return BACKGROUND;
    }
    const auto i = intersect(ray);
    if (!i.isValid())
    {
        return BACKGROUND;
    }
    const auto reflection = i.object->reflect(ray, i.intersection);

    auto lightFactor = (light - i.intersection)
                           .normalize()
                           .dot(i.object->normalAt(i.intersection));
    lightFactor = std::max(lightFactor, 0.0f);

    const auto lightTest = Line::throughPoints(i.intersection, light);
    const auto lightIntersection =
        intersect(lightTest, (light - i.intersection).lengthSquared());
    if (lightIntersection.isValid())
    {
        lightFactor = 0;
    }

    const auto ambient = i.object->material.ambient;
    const auto diffuse = i.object->material.diffuse * lightFactor;
    const auto newIntensity = intensity * i.object->material.specular / 256;
    const auto specular =
        i.object->material.specular *
        (findColorFor(reflection, bounces + 1, newIntensity) / 256);
    return (ambient + diffuse + specular).clamp(0, 255);
}
