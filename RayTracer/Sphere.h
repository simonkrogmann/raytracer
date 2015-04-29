#pragma once

#include "SceneObject.h"
#include "Vector.h"

class Sphere : public SceneObject {
public:
	Sphere();
	Sphere(vec3<float> center, float radius, Material material) 
		: SceneObject{ material }, center{ center }, radius{ radius } {}
	~Sphere();
	
	vec3<float> normalAt(vec3<float> p) { return (p - center).normalize(); }

	vec3<float> center;
	float radius;
	bool hasIntersectionWith(Line l);
	vec3<float> intersect(Line l);
private:
};

