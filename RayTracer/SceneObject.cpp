#include "SceneObject.h"


SceneObject::~SceneObject() {}

Line SceneObject::reflect(Line ray, vec3<float> at) {
	return Line{ at, ray.direction.reflect(normalAt(at)) };
}