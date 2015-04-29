#include "Scene.h"

#include <iostream>
#include <algorithm>


Intersection Scene::intersect(const Line& l, const float& closerThanSquared) {
	float record = closerThanSquared;
	vec3<float> best = vec3<float>::invalidVector();
	SceneObject * bestObject = nullptr;
	for (SceneObject * s : objects) {
		vec3<float> intersection = s->intersect(l);
		if (intersection.isValid() && (l.origin - intersection).lengthSquared() < record) {
			record = (l.origin - intersection).lengthSquared();
			best = intersection;
			bestObject = s;
		}
	}
	Intersection i = { best, bestObject };
	return i;
}

vec3<float> Scene::findColorFor(Line ray, int bounces, vec3<float> intensity) {
	if (bounces > MAX_BOUNCES || intensity.max() < 0.01) {
		return BACKGROUND;
	}
	Intersection i = intersect(ray);
	if (i.isValid()) {
		Line reflection = i.object->reflect(ray, i.intersection);

		float lightFactor = (light - i.intersection).normalize().dot(i.object->normalAt(i.intersection));
		lightFactor = std::max(lightFactor, 0.0f);

		Line lightTest = Line::throughPoints(i.intersection, light);
		Intersection lightIntersection = intersect(lightTest, (light - i.intersection).lengthSquared());
		if (lightIntersection.isValid()) {
			lightFactor = 0;
		}

		vec3<float> ambient = i.object->material.ambient;
		vec3<float> diffuse = i.object->material.diffuse * lightFactor;
		vec3<float> newIntensity = intensity * i.object->material.specular / 256;
		vec3<float> specular = i.object->material.specular * (findColorFor(reflection, bounces + 1, newIntensity) / 256);
		return (ambient + diffuse + specular).clamp(0, 255);
	} else {
		return BACKGROUND;
	}
}