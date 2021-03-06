#pragma once
#include "hitable.h"

extern vec3 random_in_unit_sphere();

vec3 reflect(const vec3 &v, const vec3 &n)
{
	return v - 2 * dot(v, n) * n;
}

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
};

class mental : public material
{
public:
	mental(const vec3 &a, float f) : albedo(a) 
	{
		if (f > 1) fuzz = f;
		else fuzz = 1;
	}
	
	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	vec3 albedo;
	float fuzz;
};