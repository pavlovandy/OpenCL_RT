#include "kernel.h"

double2	intersect_sphere(double3 eye, double3 dir, t_sphere_data sphere);
double3		ray_trace(double3 eye, double3 dir, __global t_scene *scene, double min_range, double max_range, __global double3 *texture);
double3	trim_color(double3 color);
uint	color_to_canvas(double3 color);
double3	canvas_to_viewport(int x, int y, int w, int h, t_pov pov);
t_obj_and_dist		check_closest_inter(double3 eye, double3 dir, \
										__global t_scene *scene, \
										double mini, double max);
double	calculate_light(__global t_scene *scene, double3 eye, \
						double3 dir, double3 normal, double3 intersect_point, \
						int	closest_obj);
double3	reflected_ray(double3 normal, double3 prim_ray);
double	get_intersity_after_shadow_rays(double3 intersect_point, double3 light_dir, \
										__global t_scene *scene, double min_range, \
										double max_range, __global t_light *light);

double3		get_texture_pixel(double3 intersect_point, t_sphere_data data, __global double3 *texture);

__constant double EPSILON = 0.00001;
__constant double BIG_VALUE = 9e9;
__constant double3 BACKGROUND_COLOR =  ((double3)(0.f, 0.f, 0.f));
__constant double	MINIMUM_INTENSITY = 0.01;
__constant int tree_nodes =	31;
__constant double	PI = 3.14159265359;

double3	reflected_ray(double3 normal, double3 prim_ray)
{
	return (2 * dot(normal, prim_ray) * normal - prim_ray);
}

double	get_intersity_after_shadow_rays(double3 intersect_point, double3 light_dir, \
										__global t_scene *scene, double min_range, \
										double max_range, __global t_light *light)
{
	double			local_intensity;
	t_obj_and_dist	obj_and_dist;

	local_intensity = light->intensity;
	while (local_intensity > MINIMUM_INTENSITY)
	{
		obj_and_dist = check_closest_inter(intersect_point, light_dir, scene, min_range, max_range);
		if (obj_and_dist.obj != -1)
		{
			local_intensity *= scene->obj[obj_and_dist.obj].trans;
			intersect_point = intersect_point + light_dir * obj_and_dist.dist;
		}
		else
			break ;
	}
	return (local_intensity);
}


double2	intersect_sphere(double3 eye, double3 dir, t_sphere_data sphere)
{
	double3	oc = eye - sphere.cent;
	double	a;
	double	b;
	double	c;
	double	d;
	double2	roots;
	
	a = dot(dir, dir);
	b = dot(oc, dir);
	c = dot(oc, oc) - sphere.radius * sphere.radius;
	
	d = b * b - a * c;
	if (d < 0)
		return (BIG_VALUE);
	d = sqrt(d);
	roots = (double2)((-b + d) / a, (-b - d) / a);
	return (roots);
}

double	calculate_light(__global t_scene *scene, double3 eye, \
						double3 dir, double3 normal, double3 intersect_point, \
						int	closest_obj)
{
	int		i;
	__global t_light	*light;
	double	t_max;
	double	intensity = 0;
	double3	light_dir;
	t_obj_and_dist	obj_and_dist;
	double	scalar;
	double3	reflect_ray;
	double	local_intensity;

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
			local_intensity = get_intersity_after_shadow_rays(intersect_point, light_dir, scene, EPSILON, t_max, light);
			
			/*brightness*/
			scalar = dot(normal, light_dir);
			if (scalar > 0)
				intensity += (local_intensity * scalar / (length(light_dir) * length(normal)));
			/*blicks*/
			if (scene->obj[closest_obj].specular != -1)
			{						
				reflect_ray = reflected_ray(normal, light_dir);
				scalar = dot(reflect_ray, -dir);
				if (scalar > 0)
					intensity += local_intensity * pow(scalar / (length(-dir) * length(reflect_ray)), scene->obj[closest_obj].specular);
			}
		}
	}
	return (intensity);
}

t_obj_and_dist		check_closest_inter(double3 eye, double3 dir, \
										__global t_scene *scene, \
										double mini, double max)
{
	double2	res;
	double	closest_dist = BIG_VALUE;
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

/*
void	texture_sphere(t_obj *sphere, float3 hitpoint, float2 *coord)
{
	hitpoint /= sphere->primitive.sphere.r;
	float	theta = acos(hitpoint.y) / PI;
	float2	tmp = (float2)(hitpoint.x, hitpoint.z);
	tmp = normalize(tmp);
	float	phi = acos(tmp.x) / PI_2;
	phi = hitpoint.z > 0 ? 1.f - phi : phi;
	coord->x = phi;
	coord->y = theta;
*/

double		line_point(double start, double end, double p)
{
	return ((start + (end - start) * p));
}

double3		get_texture_pixel(double3 intersect_point, t_sphere_data data, __global double3 *texture)
{
	double		s;
	double		t;
	int			is;
	int			it;

	double3		point = intersect_point - data.cent;
	point = normalize(point);
	s = acos(point[2]) / PI;
	if (fabs(point[0]) < fabs(sin(s * PI))) //this is for arcos(x) where x < -1 or x > 1. \
												Could be optimized with 1 time caculus of sin
		t = acos(point[0] / sin(s * PI)) / (2 * PI);
	else
		t = point[0] < 0 ? 0.5 : 0;
	if (point[1] < 0)
		t = 1 - t;
	is = s * 4095;
	it = t * 8191;
	if (!(s >= 0 && s < 1) || !(t >= 0 && t < 1))
	{
		printf("%f\tteta %f\tphi %f\n", acos(point[0] / sin(s * PI)), point[0], sin(s * PI));
	}
	
	return (texture[is * 8192 + it]);
}

double3		ray_trace(double3 eye, double3 dir, __global t_scene *scene, double min_range, double max_range, __global double3 *texture)
{
	double3		normal;
	double3		local_color;

	int			count = 1;
	int			curr = 0;
	double3		color = 0;

	double3		intersect_point;
	t_obj_and_dist	obj_and_dist;

	t_fig				fig;
	t_raytrace_tree		curr_node;

	t_raytrace_tree		tree[tree_nodes];
	
	tree[0].part_of_primary_ray = 1;
	tree[0].start = eye;
	tree[0].dir = dir;
	tree[0].min_range = min_range;
	tree[0].max_range = max_range;

	while (curr < count)
	{
		curr_node = tree[curr];
		obj_and_dist = check_closest_inter(curr_node.start, curr_node.dir, scene, curr_node.min_range, curr_node.max_range);
		if (obj_and_dist.obj != -1)
		{
			intersect_point = curr_node.start + curr_node.dir * obj_and_dist.dist;
			fig = scene->obj[obj_and_dist.obj];
			normal = intersect_point - fig.shape.sphere.cent;
			normal = normalize(normal);

			local_color = get_texture_pixel(intersect_point, fig.shape.sphere, texture) * calculate_light(scene, curr_node.start, curr_node.dir, normal, intersect_point, obj_and_dist.obj);
			local_color *= tree[curr].part_of_primary_ray;

			color += local_color; 
			color += local_color * (1 - fig.trans - fig.reflective);

			if (fig.trans > 0 && count < tree_nodes)
			{
				tree[count].part_of_primary_ray = tree[curr].part_of_primary_ray * fig.trans;
				if (tree[count].part_of_primary_ray > MINIMUM_INTENSITY)
				{
					color -= local_color * fig.trans;
					tree[count].start = intersect_point;
					tree[count].dir = tree[curr].dir;
					tree[count].min_range = EPSILON;
					tree[count].max_range = BIG_VALUE;
					count++;
				}
			}

			if (fig.reflective > 0 && count < tree_nodes)
			{
				tree[count].part_of_primary_ray = tree[curr].part_of_primary_ray * fig.reflective;
				if (tree[count].part_of_primary_ray > MINIMUM_INTENSITY)
				{
					color -= local_color * fig.reflective;
					tree[count].start = intersect_point;
					tree[count].dir = reflected_ray(normal, -tree[curr].dir);
					tree[count].min_range = EPSILON;
					tree[count].max_range = BIG_VALUE;
					count++;
				}
			}
			curr++;
		}
		else
		{
			color += BACKGROUND_COLOR * tree[curr].part_of_primary_ray;
			curr++;
			continue ;
		}
	}
	return (color);
}

double3	trim_color(double3 color)
{
	if (color[0] > (double)255)
		color[0] = (double)255;
	if (color[1] > (double)255)
		color[1] = (double)255;
	if (color[2] > (double)255)
		color[2] = (double)255;

	if (color[0] < (double)0)
		color[0] = (double)0;
	if (color[1] < (double)0)
		color[1] = (double)0;
	if (color[2] < (double)0)
		color[2] = (double)0;
	return (color);
}

// double3	ft_rotate_camera(double3 direction, t_pov pov)
// {
// 	double new_x;
// 	double new_y;
// 	double new_z;

// 	new_x = direction[0] * pov.cy + direction[2] * pov.sy;
// 	new_z = -direction[0] * pov.sy + direction[2] * pov.cy;
// 	direction[0] = new_x;
// 	direction[2] = new_z;
// 	new_y = direction[1] * pov.cx + direction[2] * pov.sx;
// 	new_z = -direction[1] * pov.sx + direction[2] * pov.cx;
// 	direction[1] = new_y;
// 	direction[2] = new_z;
// 	return (direction);
// }

uint	color_to_canvas(double3 color)
{
	return (((uint)color[0] << 16) + ((uint)color[1] << 8) + (uint)color[2]);
}

double3	canvas_to_viewport(int x, int y, int w, int h, t_pov pov)
{
	return ((double3){(double)x * pov.vw / w, \
								-(double)y * pov.vh / h, (double)pov.d});
}

__kernel void	test_kernel(__global t_scene *scene,
							__global uint *canvas,
							int w,
							int h,
							t_pov pov,
							__global double3 *texture)
{
	int	id = get_global_id(0);
	int	x = id % w;
	int	y = id / w;
	double3	direction;
	double3	color;

	direction = canvas_to_viewport(x - w /2 , y - h / 2, w, h, pov);
	color = ray_trace(pov.coord, direction, scene, 1, BIG_VALUE, texture);
	color = trim_color(color);
	canvas[id] = color_to_canvas(color);
}
