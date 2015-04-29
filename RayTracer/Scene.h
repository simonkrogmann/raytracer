#pragma once

#include "Vector.h"
#include <vector>
#include "SceneObject.h"
#include "Line.h"

#define BACKGROUND vec3<float>{52.0f}
#define MAX_BOUNCES 7

struct Intersection {
	vec3<float> intersection;
	SceneObject * object;
	bool isValid() { return intersection.isValid(); }
};

class Scene {
public:
	Scene(vec3<float> light) : light{ light } {}
	~Scene() { for (auto o : objects) delete o; };

	void add(SceneObject * s) { objects.push_back(s); }
	Intersection intersect(const Line& l, const float& closerThanSquared = INFINITY);
	vec3<float> findColorFor(Line ray, int bounces = 0, vec3<float> intensity = vec3 < float > { 1.0f});

private:
	vec3<float> light;
	std::vector < SceneObject * > objects;

};

