#include "kernel.h"

double3	reflected_ray(double3 normal, double3 prim_ray) //not sure
{
	return (2 * dot(normal, prim_ray) * normal - prim_ray);
}

void	swap(double* a, double*b)
{
	double	c;

	c = *a;
	*a = *b;
	*b = c;
}

/*
	Seems like i dont know how to insert this feature here.
	If someones wants to add he will need to calculate the \
	distance of how much it is in medium and multiply "somewheres" \
	color by this value.
	Some usefull links :
	https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
	https://graphicscompendium.com/raytracing/11-fresnel-beer
*/

double3	beers_law(double distance, double3 obj_absorb)
{
	return (exp(obj_absorb * (-distance)));
}

double fresnel(double3 prim_ray, double3 normal, double n1, double reflective)
{
	double n2 = 1.00029;
	double r0 = (n1 - n2) / (n1 + n2);
	r0 *= r0;
	double cosX = -dot(normal, prim_ray);
	if (n1 > n2)
	{
		double n = n1 / n2;
		double sinT2 = n * n * (1.0 - cosX * cosX);
		// Total internal reflection
		if (sinT2 > 1.0)
			return 1.0;
		cosX = sqrt(1.0 - sinT2);
	}
	double x = 1.0 - cosX;
	double ret = r0 + (1.0 - r0) * x * x * x * x * x;
 
	ret = (reflective + (1.0 - reflective) * ret);
	return ret;
}

double3	refract_ray(double3 prim_ray, double3 normal, double ior_new)
{
	double		cosi = dot(normal, prim_ray);
	double		ior_prev = 1.00029;
	double		k;

	if (cosi < 0)
		cosi = -cosi;
	else
	{
		normal = -normal;
		swap(&ior_prev, &ior_new);
	}
	double		eta = ior_prev / ior_new;

	k = 1 - eta * eta * (1 - cosi * cosi);
	return (k < 0 ? 0 : eta * prim_ray + (eta * cosi - sqrt(k)) * normal);
}

double3	get_intersity_after_shadow_rays(double3 intersect_point, double3 light_dir, \
										__global t_scene *scene, double min_range, \
										double max_range, __global t_light *light)
{
	double3			local_intensity;
	t_obj_and_dist	obj_and_dist;
	double			intensity_level;

	local_intensity = light->intensity;
	intensity_level = length(local_intensity);
	while (intensity_level > MINIMUM_INTENSITY)
	{
		obj_and_dist = check_closest_inter(intersect_point, light_dir, scene, min_range, max_range);
	
		if (obj_and_dist.obj != -1)
		{
			local_intensity *= scene->obj[obj_and_dist.obj].trans;
			intensity_level *= scene->obj[obj_and_dist.obj].trans;
			intersect_point = intersect_point + light_dir * obj_and_dist.dist;
		}
		else
			break ;
	}
	return (local_intensity);
}

double2	cartesian_to_sperical_coords(double3 intersect_point, t_fig data)
{
	double		s;
	double		t;

	double3		point = intersect_point - data.shape.sphere.cent;
	if (length(data.rotation) > 0)
		point = rotate_z(rotate_y(rotate_x(point, data.rotation[0]), data.rotation[1]), data.rotation[2]);
	point = normalize(point);
	s = acos(point[2]) / PI;
	if (fabs(point[0]) < fabs(sin(s * PI))) //this is for arcos(x) where x < -1 or x > 1. \
												Could be optimized with 1 time caculus of sin
		t = acos(point[0] / sin(s * PI)) / (2 * PI);
	else
		t = point[0] < 0 ? 0.5 : 0;
	if (point[1] < 0)
		t = 1 - t;
	return ((double2)(s, t));
}


uint	get_texture_pixel_sphere(double3 intersect_point, t_fig data, __global uint *texture)
{
	int			is;
	int			it;
	double2	texture_space_coords = cartesian_to_sperical_coords(intersect_point, data);
	is = texture_space_coords[0] * 4095;
	it = texture_space_coords[1] * 8191;
	return (texture[is * 8192 + it]);
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

double	bump_function_u_deriv(double u, double v)
{
	return (0.5 * cos(u * 2 * M_PI * 10) * sin(v * 2 * M_PI * 10));
}

double	bump_function_v_deriv(double u, double v)
{
	return (0.5 * cos(v * 2 * M_PI * 10) * sin(u * 2 * M_PI * 10));
}

double3	calculate_light(__global t_scene *scene, double3 eye, \
						double3 dir, double3 normal, double3 intersect_point, \
						int	closest_obj, __global uint *bump_map)
{
	int		i;
	__global t_light	*light;
	double	t_max;
	double3	intensity = 0;
	double3	light_dir;
	double	scalar;
	double3	reflect_ray;
	double3	local_intensity;
	double2	texture_space_coords;

	texture_space_coords = cartesian_to_sperical_coords(intersect_point, scene->obj[closest_obj]);

	double3	n = normalize(intersect_point - scene->obj[closest_obj].shape.sphere.cent);
	double3	A = (double3)(1, 0, 0);
	double3	t = normalize(cross(A, n));
	double3 b = normalize(cross(n, t));
	double	du = bump_function_u_deriv(texture_space_coords[0], texture_space_coords[1]);
	double	dv = bump_function_v_deriv(texture_space_coords[0], texture_space_coords[1]);
	int		is = texture_space_coords[0] * 1023;
	int		it = texture_space_coords[1] * 2047;
	double3	normal_from_map = uint_to_double3(bump_map[is * 2048 + it]);
	du = normal_from_map[0] / 255 * 2 - 1;
	dv = normal_from_map[1] / 255 * 2 - 1;

	double3	new_normal = normalize(n + du * t + dv * b);

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
			if (length(local_intensity) < MINIMUM_INTENSITY)
				continue ;
			
			//double3 light_dir_tangent_space = (double3)(dot(light_dir, t), dot(light_dir, b), dot(light_dir, n));
			//light_dir_tangent_space = normalize(light_dir_tangent_space);

			/*blicks*/
			if (scene->obj[closest_obj].specular > 0)
			{						
				reflect_ray = reflected_ray(normal, light_dir);
				scalar = dot(reflect_ray, -dir);
				if (scalar > 0)
					intensity += local_intensity * pow(scalar / (length(-dir) * length(reflect_ray)), scene->obj[closest_obj].specular);
			}
			/*brightness*/
			scalar = dot(new_normal, light_dir);
			if (scalar > 0)
				intensity += (local_intensity * scalar / (length(light_dir) * length(new_normal)));
			
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

double		line_point(double start, double end, double p)
{
	return ((start + (end - start) * p));
}

double3		rotate_x(double3 v, double angle)
{
	double y = v[1];
	double c = cos(angle);
	double s = sin(angle);

	v[1] = y * c - s * v[2];
	v[2] = y * s + c * v[2];
	return (v);
}

double3		rotate_y(double3 v, double angle)
{
	double x = v[0];
	double c = cos(angle);
	double s = sin(angle);

	v[0] = x * c + s * v[2];
	v[2] = -x * s + c * v[2];
	return (v);
}

double3		rotate_z(double3 v, double angle)
{
	double x = v[0];
	double c = cos(angle);
	double s = sin(angle);

	v[0] = x * c - s * v[1];
	v[1] = x * s + c * v[1];
	return (v);
}

double3		uint_to_double3(uint a)
{
	double3	d;

	d[0] = (a >> 16) & 0xff;
	d[1] = (a >> 8) & 0xff;
	d[2] = a & 0xff;
	return (d);
}

double3		ray_trace(double3 eye, double3 dir, __global t_scene *scene, double min_range, double max_range, __global uint *texture, __global uint *bump)
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
			//normal calculations

			intersect_point = curr_node.start + curr_node.dir * obj_and_dist.dist;
			fig = scene->obj[obj_and_dist.obj];
			normal = intersect_point - fig.shape.sphere.cent;
			normal = normalize(normal);
			//--------------------

			//get_texture_pixel_sphere(intersect_point, fig, texture)
			if (fig.text_no > -1)
				local_color = uint_to_double3(get_texture_pixel_sphere(intersect_point, fig, texture)) * calculate_light(scene, curr_node.start, curr_node.dir, normal, intersect_point, obj_and_dist.obj, bump);
			else
				local_color = fig.color * calculate_light(scene, curr_node.start, curr_node.dir, normal, intersect_point, obj_and_dist.obj, bump);
			local_color *= curr_node.part_of_primary_ray;

			double fren = fresnel(curr_node.dir, normal, fig.ior, fig.reflective); //prart of reflected ray

			if (fig.trans > 0 && count < tree_nodes && fren < 1)
			{
				tree[count].part_of_primary_ray = curr_node.part_of_primary_ray * (1 - fren);
				if (tree[count].part_of_primary_ray > MINIMUM_INTENSITY)
				{
					// local_color *= (1 - fig.trans);
					tree[count].start = intersect_point;
					tree[count].dir = normalize(refract_ray(curr_node.dir, normal, fig.ior));
					tree[count].min_range = EPSILON;
					tree[count].max_range = BIG_VALUE;
					count++;	
				}
			}
			if (fig.reflective > 0 && count < tree_nodes)
			{
				tree[count].part_of_primary_ray = curr_node.part_of_primary_ray * fren;
				if (tree[count].part_of_primary_ray > MINIMUM_INTENSITY)
				{
					//local_color *= (1 - fig.reflective);
					tree[count].start = intersect_point;
					tree[count].dir = normalize(reflected_ray(normal, -curr_node.dir));
					tree[count].min_range = EPSILON;
					tree[count].max_range = BIG_VALUE;
					count++;
				}
			}

			color += local_color;
			curr++;
		}
		else
		{
			color += BACKGROUND_COLOR * curr_node.part_of_primary_ray;
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

uint	color_to_canvas(double3 color)
{
	return (((uint)color[0] << 16) + ((uint)color[1] << 8) + (uint)color[2]);
}

double3	canvas_to_viewport(int x, int y, int w, int h, t_pov pov)
{
	return ((double3){(double)x * pov.vw / w, \
								-(double)y * pov.vh / h, (double)pov.d});
}

double3	rotate_camera(double3 direction, t_pov pov)
{
	double new_x;
	double new_y;
	double new_z;

	new_x = direction[0] * pov.cy + direction[2] * pov.sy;
	new_z = -direction[0] * pov.sy + direction[2] * pov.cy;
	direction[0] = new_x;
	direction[2] = new_z;
	new_y = direction[1] * pov.cx + direction[2] * pov.sx;
	new_z = -direction[1] * pov.sx + direction[2] * pov.cx;
	direction[1] = new_y;
	direction[2] = new_z;
	return (direction);
}

__kernel void	test_kernel(__global t_scene *scene,
							__global uint *canvas,
							int w,
							int h,
							t_pov pov,
							__global uint *texture,
							__global uint *bump_map)
{
	int	id = get_global_id(0);
	int	x = id % w;
	int	y = id / w;
	double3	direction;
	double3	color;

	direction = normalize(canvas_to_viewport(x - w /2 , y - h / 2, w, h, pov));
	direction = rotate_camera(direction, pov);
	color = ray_trace(pov.coord, direction, scene, 0.1, BIG_VALUE, texture, bump_map);
	color = trim_color(color);
	
	canvas[id] = color_to_canvas(color);
}
