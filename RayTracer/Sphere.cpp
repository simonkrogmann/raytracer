#include "Sphere.h"

#include <iostream>


Sphere::Sphere() {}


Sphere::~Sphere() {}

bool Sphere::hasIntersectionWith(Line l) {
	return intersect(l).isValid();
}

vec3<float> Sphere::intersect(Line l) {
	vec3<float> to_origin = l.origin - center;
	double determinant = std::pow(l.direction.dot(to_origin), 2)
		- std::pow(to_origin.length(), 2)
		+ std::pow(radius, 2);
	if (determinant < 0) {
		return vec3<float>::invalidVector();
	}
	double root = std::sqrt(determinant);
	double mid = -l.direction.dot(to_origin);
	double solution;
	if (mid - root > BIAS) {
		solution = mid - root;
	} else if (mid + root > BIAS) {
		solution = mid + root;
	} else {
		return vec3<float>::invalidVector();
	}
	return l.at(solution);
}
