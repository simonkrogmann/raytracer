#include "Plane.h"


Plane::Plane() {}

Plane::~Plane() {}

bool Plane::hasIntersectionWith(Line l) {
	return intersect(l).isValid();
}

vec3<float> Plane::intersect(Line l) {
	float denominator = l.direction.dot(normal);
	//std::cout << denominator << std::endl;
	if (denominator == 0) {
		return vec3 < float > {false};
	}
	float numerator = (origin - l.origin).dot(normal);

	//std::cout << "num " << numerator << std::endl;
	//std::cout << "af " << numerator / denominator << std::endl;
	if (numerator / denominator < BIAS) {
		return vec3 < float > {false};
	}
	return l.at(numerator / denominator);
}