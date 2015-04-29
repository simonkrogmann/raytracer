#pragma once

#include <vector>
#include <algorithm>


template <typename number>
inline number clip(number n, number min, number max) {
	return std::min(std::max(n, min), max);
}

template <typename number>
class vec3 {
public:
	vec3() : x{ 0 }, y{ 0 }, z{ 0 } {};
	vec3(bool valid) : x{ 0 }, y{ 0 }, z{ 0 }, valid{ valid } {};
	vec3(number fill) : x{ fill }, y{ fill }, z{ fill } {};
	vec3(number x, number y, number z) : x{ x }, y{ y }, z{ z } {};
	~vec3() {};

	vec3<number> operator+(const vec3<number>& v) const { return vec3{ x + v.x, y + v.y, z + v.z }; }
	vec3<number> operator-(const vec3<number>& v) const { return vec3{ x - v.x, y - v.y, z - v.z }; }
	vec3<number> operator*(const vec3<number>& v) const { return vec3{ x * v.x, y * v.y, z * v.z }; }
	vec3<number> operator/(const vec3<number>& v) const { return vec3{ x / v.x, y / v.y, z / v.z }; }
	vec3<number> operator+(const number& fill) const { return operator+(vec3{ fill }); }
	vec3<number> operator-(const number& fill) const { return operator-(vec3{ fill }); }
	vec3<number> operator*(const number& fill) const { return operator*(vec3{ fill }); }
	vec3<number> operator/(const number& fill) const { return operator/(vec3{ fill }); }

	number dot(vec3<number> v) { return x * v.x + y * v.y + z * v.z; }
	vec3<number> cross(vec3<number> v) { return vec3{ y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x }; }

	double length() { return sqrt(x * x + y * y + z * z); }
	float lengthSquared() { return x * x + y * y + z * z; }

	vec3<number> clamp(number min, number max) {
		return vec3 < number > { clip(x, min, max), clip(y, min, max), clip(z, min, max)};
	}

	//I - 2.0 * dot(N, I) * N
	vec3<number> reflect(vec3<number> normal) { return operator-(normal * (number) (2.0 * dot(normal))); }

	vec3<number> normalize() { return operator/(length()); }

	number operator[](int i) { switch (i) { case 0: return x; case 1: return y; case 2: return z; } }

	static vec3<number> invalidVector() { return vec3{ false }; }
	bool isValid() { return valid; }

	number max() { return std::max(x, std::max(y, z)); }

	number x;
	number y;
	number z;
private:
	bool valid = true;
};

template <typename number>
std::ostream& operator<<(std::ostream &os, vec3<number> const &v) {
	return os << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
}