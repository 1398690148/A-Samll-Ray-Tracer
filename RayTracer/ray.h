#pragma once
#include "vec3.h"

class ray
{
public:
	ray() {}
	ray(const vec3 &a, const vec3 &b, double time = 0.0) : tm(time)
	{
		A = a;
		B = b;
	}
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	double time() const { return tm; }
	vec3 at(float t) const { return A + t * B; }

	vec3 A;
	vec3 B;
	double tm;
};