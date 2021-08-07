#include <iostream>
#include <fstream>
#include <algorithm>
#include "sphere.h"
#include "moving_sphere.h"
#include "hitable_list.h"
#include <float.h>
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "dielectric.h"
#include "checker_texture.h"
#include "noise_texture.h"
using namespace std;

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

float hit_sphere(const vec3 &center, float radius, const ray &r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminat = b * b - 4 * a * c;
	if (discriminat < 0) return -1.0;
	else return (-b + sqrt(discriminat)) / (2.0 * a);
}

hitable_list *random_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), 
		new constant_texture(vec3(0.9, 0.9, 0.9)));
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = drand48(); //随机材质
			vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48()); //随机中心

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				//if (choose_mat < 0.8) {
					list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48()))));
				//}
				/*else if (choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2, 
						new mental(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}*/
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new mental(vec3(0.7, 0.6, 0.5), 0.0));
	return new hitable_list(list, i);
}

hitable *two_perlin_spheres()
{
	texture *pertext = new noise_texture(1.5);
	hitable **list = new hitable*[2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2.0, new lambertian(pertext));
	return new hitable_list(list, 2);

}

vec3 color(const ray &r, hitable *world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			//外面的颜色和自己的颜色的一个混合就是物体被看见的颜色
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

int main()
{
	std::ofstream outfile;
	outfile.open("./data.ppm");
	if (!outfile.is_open())
	{
		std::cout << "文件打开失败" << std::endl;
		return -1;
	}
	int nx = 200;
	int ny = 100;
	int ns = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
	//hitable *list[5];
	//list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	//list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100.0, new lambertian(vec3(0.8, 0.8, 0.0)));
	//list[2] = new sphere(vec3(1.0, 0.0, -1), 0.50, new mental(vec3(0.8, 0.6, 0.2), 0.3f));
	//list[3] = new sphere(vec3(-1.0, 0.0, -1), 0.5, new dielectric(1.5f));
	//list[4] = new sphere(vec3(-1.0, 0.0, -1), -0.45, new dielectric(1.5f));

	//hitable *world = new hitable_list(list, 5);
	hitable *world = two_perlin_spheres();//random_scene();

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			//抗锯齿在一个像素内采样更多的点
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + float(rand()) / RAND_MAX) / float(nx);
				float v = float(j + float(rand()) / RAND_MAX) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			//vec3 p = r.point_at_parameter(2.0);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outfile.close();
	std::cout << "文件保存成功" << std::endl;
	return 0;
}