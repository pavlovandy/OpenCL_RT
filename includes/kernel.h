/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:41:32 by apavlov           #+#    #+#             */
/*   Updated: 2019/08/20 15:41:33 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
# define KERNEL_H

# define MAX_OBJ_COUNT 20
# define MAX_LIGHTING_COUNT 10

__constant double EPSILON = 0.00001;
__constant double BIG_VALUE = 9e9;
__constant double3 BACKGROUND_COLOR =  ((double3)(0.f, 0.f, 0.f));
__constant double	MINIMUM_INTENSITY = 0.01;
__constant int tree_nodes =	31;
__constant double	PI = 3.14159265359;

enum	e_fig
{
	SPHERE = 1, PLANE, CONE, CYLIN
};
enum	e_light
{
	AMBIENT = 1, DIRECT, POINT
};

typedef struct	s_sphere_data
{
	double3	cent;
	double	radius;
}				t_sphere_data;

typedef struct	s_plane_data
{
	double3	normal;
	double3	dot;
}				t_plane_data;

typedef struct	s_cone_data
{
	double3	vertex;
	double3	dir;
	double	tangent;
}				t_cone_data;

typedef struct	s_cylin_data
{
	double3	dir;
	double3	dot;
	double	radius;
}				t_cylin_data;

typedef union	u_shape
{
	t_sphere_data	sphere;
	t_cone_data		cone;
	t_plane_data	plane;
	t_cylin_data	cylin;
}				t_shape;

typedef struct	s_fig
{
	int			fig_type;
	t_shape		shape;

	double3		color;
	int			specular;
	double		reflective;
	double		trans;
	double3		rotation;
	double		ior;
	int			text_no;
}				t_fig;

typedef struct	s_pov
{
	double3		coord;
	double3		dir;
	double		cx;
	double		cy;
	double		sx;
	double		sy;
	double		d;
	double		vh;
	double		vw;
}				t_pov;

typedef struct	s_raytrace_tree
{
	double		part_of_primary_ray;
	double3		start;
	double3		dir;
	double		min_range;
	double		max_range;
}				t_raytrace_tree;

typedef struct	s_obj_and_dist
{
	int		obj;
	double	dist;
}				t_obj_and_dist;

typedef struct	s_light
{
	int			type_num;
	double		intensity;
	double3		v;
}				t_light;

typedef struct	s_scene
{
	int			count_obj;
	t_fig		obj[MAX_OBJ_COUNT];
	int			count_light;
	t_light		light[MAX_LIGHTING_COUNT];
}				t_scene;

double2	intersect_sphere(double3 eye, double3 dir, t_sphere_data sphere);
double3		ray_trace(double3 eye, double3 dir, __global t_scene *scene, double min_range, double max_range, __global uint *texture, __global uint *bump);
double3	trim_color(double3 color);
uint	color_to_canvas(double3 color);
double3	canvas_to_viewport(int x, int y, int w, int h, t_pov pov);
t_obj_and_dist		check_closest_inter(double3 eye, double3 dir, \
										__global t_scene *scene, \
										double mini, double max);
double	calculate_light(__global t_scene *scene, double3 eye, \
						double3 dir, double3 normal, double3 intersect_point, \
						int	closest_obj, __global uint *bump_map);
double3	reflected_ray(double3 normal, double3 prim_ray);
double	get_intersity_after_shadow_rays(double3 intersect_point, double3 light_dir, \
										__global t_scene *scene, double min_range, \
										double max_range, __global t_light *light);
double3	refract_ray(double3 prim_ray, double3 normal, double ior2_new);
double3	rotate_camera(double3 direction, t_pov pov);

uint		get_texture_pixel_sphere(double3 intersect_point, t_fig data, __global uint *texture);
void		swap(double* a, double*b);
double		fresnel(double3 prim_ray, double3 normal, double n1, double reflective);
double3		uint_to_double3(uint a);
double3		rotate_x(double3 v, double angle);
double3		rotate_y(double3 v, double angle);
double3		rotate_z(double3 v, double angle);
double2		cartesian_to_sperical_coords(double3 intersect_point, t_fig data);

#endif