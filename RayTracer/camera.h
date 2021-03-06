#pragma once
#include "ray.h"
#include "drand48.h"

#define M_PI 3.1415926

vec3 random_in_unit_disk()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

class camera
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, double t0 = 0, double t1 = 0)
	{
		origin = lookfrom;
		lens_radius = aperture / 2;
		time0 = t0;
		time1 = t1;
		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * u * focus_dist 
			- half_height * v * focus_dist - w * focus_dist;
		horizontal = 2 * half_width * u * focus_dist;
		vertical = 2 * half_height * v * focus_dist; 
	}
	ray get_ray(float s, float t) 
	{
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, 
			lower_left_corner + s * horizontal + t * vertical - origin - offset, time0 + (time1 - time0) * drand48());
	}
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lens_radius;
	double time0, time1;
};