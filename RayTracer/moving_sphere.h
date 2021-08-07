#pragma once
#include "hitable.h"

class moving_sphere : public hitable
{
public:
	moving_sphere() {}
	moving_sphere(vec3 cen0, vec3 cen1, double t0, double t1, double r, material *m)
		: center0(cen0), center1(cen1), time0(t1), time1(t1), radius(r), mat_ptr(m)
	{}
	virtual bool hit(const ray &r, double tmin, double tmax, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;

	vec3 center(double time) const;
	vec3 center0, center1;
	double time0, time1;
	double radius;
	material *mat_ptr;
};

vec3 moving_sphere::center(double time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray &r, double tmin, double tmax, hit_record &rec) const
{
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp < tmax && temp > tmin)
		{
			rec.t = temp;
			rec.p = r.at(temp);
			vec3 outward_normal = (rec.p - center(r.time())) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < tmax && temp > tmin)
		{
			rec.t = temp;
			rec.p = r.at(temp);
			vec3 outward_normal = (rec.p - center(r.time())) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb &box) const
{
	aabb box0 = aabb(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
	aabb box1 = aabb(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
	box = surrounding_box(box0, box1);
	return true;
}