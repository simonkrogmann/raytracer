#pragma once
#include "SceneObject.h"
class Plane :
	public SceneObject {
public:
	Plane();
	Plane(vec3<float> origin, vec3<float> normal, Material material)
		: SceneObject{ material }, origin{ origin }, normal{ normal.normalize() } {}
	~Plane();

	vec3<float> normalAt(vec3<float> p) { return normal; }

	bool hasIntersectionWith(Line l);
	vec3<float> intersect(Line l);
private:
	vec3<float> origin;
	vec3<float> normal;
};

