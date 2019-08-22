#include "kernel.h"

float2	intersect_sphere(float3 eye, float3 dir, t_sphere_data sphere);
float3	ray_trace(float3 eye, float3 dir, __global t_scene *scene, float mini, float maxi);
float3	trim_color(float3 color);
uint	color_to_canvas(float3 color);
float3	canvas_to_viewport(int x, int y, int w, int h, t_pov pov);
t_obj_and_dist		check_closest_inter(float3 eye, float3 dir, \
										__global t_scene *scene, \
										float mini, float max);
float	calculate_light(__global t_scene *scene, float3 eye, \
						float3 dir, float3 normal, float3 intersect_point, \
						int	closest_obj);


float2	intersect_sphere(float3 eye, float3 dir, t_sphere_data sphere)
{
	float3	oc = eye - sphere.cent;
	float	a;
	float	b;
	float	c;
	float	d;
	float2	roots;
	
	a = dot(dir, dir);
	b = dot(oc, dir);
	c = dot(oc, oc) - sphere.radius * sphere.radius;
	
	d = b * b - a * c;
	if (d < 0)
		return (BIG_VALUE);
	d = sqrt(d);
	roots = (float2)((-b + d) / a, (-b - d) / a);
	return (roots);
}

float	calculate_light(__global t_scene *scene, float3 eye, \
						float3 dir, float3 normal, float3 intersect_point, \
						int	closest_obj)
{
	int		i;
	__global t_light	*light;
	float	t_max;
	float	intensity = 0;
	float3	light_dir;
	t_obj_and_dist	obj_and_dist;
	float	scalar;
	float3	reflected_ray;

	i = -1;
	while (++i < scene->count_light)
	{
		light = scene->light + i;
		if (light->type_num == AMBIENT)
		{
			intensity += light->intensity;
		}
		else
		{
			if (light->type_num == POINT)
				light_dir = light->v - intersect_point;
			else
				light_dir = light->v;
			t_max = (light->type_num == POINT) ? 1 : BIG_VALUE;
			/*shadow ray*/
			obj_and_dist = check_closest_inter(intersect_point, light_dir, scene, EPSILON, t_max);
			if (obj_and_dist.obj != -1)
				continue ;
			
			/*brightness*/
			scalar = dot(normal, light_dir);
			if (scalar > 0)
				intensity += (light->intensity * scalar / (length(light_dir) * length(normal)));
			/*blicks*/
			if (scene->obj[closest_obj].specular != -1)
			{
				reflected_ray = normal * 2 * dot(normal, light_dir) - light_dir;
				scalar = dot(reflected_ray, -dir);
				if (scalar > 0)
					intensity += light->intensity * \
pow(scalar / (length(-dir) * length(reflected_ray)), scene->obj[closest_obj].specular);
			}
		}
	}
	return (intensity);
}

t_obj_and_dist		check_closest_inter(float3 eye, float3 dir, \
										__global t_scene *scene, \
										float mini, float max)
{
	float2	res;
	float	closest_dist = BIG_VALUE;
	int		closest_obj = -1;
	int		i;

	i = -1;
	while (++i < scene->count_obj)
	{
		res = intersect_sphere(eye, dir, scene->obj[i].shape.sphere);
		if (res[0] > mini && res[0] < max && res[0] < closest_dist)
		{
			closest_dist = res[0];
			closest_obj = i;
		}
		if (res[1] > mini && res[1] < max && res[1] < closest_dist)
		{
			closest_dist = res[1];
			closest_obj = i;
		}
	}
	return ((t_obj_and_dist){closest_obj, closest_dist});
}

float3	ray_trace(float3 eye, float3 dir, __global t_scene *scene, float mini, float maxi)
{
	float3	normal;
	float3	intersect_point;
	t_obj_and_dist obj_and_dist;

	obj_and_dist = check_closest_inter(eye, dir, scene, mini, maxi);
	if (obj_and_dist.obj != -1)
	{
		intersect_point = eye + dir * obj_and_dist.dist;
		normal = intersect_point - scene->obj[obj_and_dist.obj].shape.sphere.cent;
		normal = normalize(normal);
		return (scene->obj[obj_and_dist.obj].color * calculate_light(scene, eye, dir, normal, intersect_point, obj_and_dist.obj));
	}
	return ((float3)(0.f, 0.f, 0.f));
}


float3	trim_color(float3 color)
{
	if (color[0] > (float)255)
		color[0] = (float)255;
	if (color[1] > (float)255)
		color[1] = (float)255;
	if (color[2] > (float)255)
		color[2] = (float)255;

	if (color[0] < (float)0)
		color[0] = (float)0;
	if (color[1] < (float)0)
		color[1] = (float)0;
	if (color[2] < (float)0)
		color[2] = (float)0;
	return (color);
}

uint	color_to_canvas(float3 color)
{
	return (((uint)color[0] << 16) + ((uint)color[1] << 8) + (uint)color[2]);
}

float3	canvas_to_viewport(int x, int y, int w, int h, t_pov pov)
{
	return ((float3){(float)x * pov.vw / w, \
								-(float)y * pov.vh / h, (float)pov.d});
}

__kernel void	test_kernel(__global t_scene *scene,
							__global uint *canvas,
							int w,
							int h,
							t_pov pov)
{
	int	id = get_global_id(0);
	int	x = id % w;
	int	y = id / w;
	float3	direction;
	float3	color;

	direction = canvas_to_viewport(x - w /2 , y - h / 2, w, h, pov);
	color = ray_trace(pov.coord, direction, scene, 1, BIG_VALUE);
	color = trim_color(color);
	canvas[y * w + x] = color_to_canvas(color);
}
