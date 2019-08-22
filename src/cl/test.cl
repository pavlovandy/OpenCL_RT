#include "kernel.h"

float	intersect_sphere(float3 eye, float3 dir, float mini, float max, t_sphere_data sphere);
float3	ray_trace(float3 eye, float3 dir, __global t_scene *scene, float mini, float max);
float3	trim_color(float3 color);
uint	color_to_canvas(float3 color);
float3	canvas_to_viewport(int x, int y, int w, int h, t_pov pov);

float	intersect_sphere(float3 eye, float3 dir, float mini, float max, t_sphere_data sphere)
{
	float3	oc = eye - sphere.cent;
	float	a;
	float	b;
	float	c;
	float	d;
	float2	roots;
	float	res = (float)10e9;
	
	a = dot(dir, dir);
	b = dot(oc, dir);
	c = dot(oc, oc) - sphere.radius * sphere.radius;
	
	d = b * b - a * c;
	if (d < 0)
		return (res);
	
	d = sqrt(d);
	
	roots[0] = ((-b + d) / a);
	roots[1] = ((-b - d) / a);
	
	if (roots[0] > mini && roots[0] < max)
		res = roots[0];
	if (roots[1] > mini && roots[1] < max)
		res = min(res, roots[1]);
	return (res);
}


float3	ray_trace(float3 eye, float3 dir, __global t_scene *scene, float mini, float max)
{
	float	closest_dist = 9e9;
	int		closest_obj = -1;
	int		i;
	float	res;

	i = -1;
	while (++i < scene->count_obj)
	{
		res = intersect_sphere(eye, dir, mini, max, scene->obj[i].shape.sphere);
		if (closest_dist > res)
		{
			closest_dist = res;
			closest_obj = i;
		}
	}
	if (closest_obj != -1)
	{
		printf("hehe\n");
		return (scene->obj[closest_obj].color);
	}
	return ((float3){0.f, 0.f, 0.f});
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
	
	direction = canvas_to_viewport(x, y, w, h, pov);
	color = ray_trace(pov.coord, direction, scene, 1, 9e9);
	color = trim_color(color);
	canvas[y * w + x] = color_to_canvas(color);
}
