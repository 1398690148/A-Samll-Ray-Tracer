#pragma once
#include "material.h"
#include "texture.h"
class lambertian : public material
{
public:
	lambertian(texture *a) : albedo(a) {}
	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
	{
		vec3 target = rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target, r_in.time());
		attenuation = albedo->value(0, 0, rec.p);
		return true;
	}
	texture *albedo;
};