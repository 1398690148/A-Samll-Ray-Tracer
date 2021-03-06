#pragma once

#include "hitable.h"

class sphere : public hitable
{
public:
	sphere() {}
	sphere(vec3 cen, float r, material *mat) : center(cen), radius(r), mat_ptr(mat) {}
	virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;
	vec3 center;
	float radius;
	material *mat_ptr;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.at(temp);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.at(temp);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

bool sphere::bounding_box(float t0, float t1, aabb &box) const
{
	box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}