#pragma once

#include "Line.h"
#include <iostream>
#include "Material.h"

#define BIAS 0.01

class SceneObject {
public:
	SceneObject() {}
	SceneObject(Material material) : material{ material } {}
	~SceneObject();

	virtual bool hasIntersectionWith(Line l) = 0;
	virtual vec3<float> intersect(Line l) = 0;

	virtual vec3<float> normalAt(vec3<float> point) = 0;

	Line reflect(Line ray, vec3<float> at);

	Material material;
};