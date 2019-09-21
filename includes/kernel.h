/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:41:32 by apavlov           #+#    #+#             */
/*   Updated: 2019/09/19 18:16:01 by apavlov          ###   ########.fr       */
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
	SPHERE = 1, PLANE, CONE, CYLIN, RECTANGLE, TRIANGLE, DISK, TORUS
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

typedef struct	s_rectangle_data
{
	double3	v1;
	double3	v2;
	double3	v3;
	double3	v4;
}				t_rectangle_data;

typedef struct	s_disk_data
{
	double3	cent;
	double	radius;
}				t_disk_data;

typedef struct	s_torus_data
{
}				t_torus_data;

typedef struct	s_triangle_data
{
	double3	v1;
	double3	v2;
	double3	v3;
}				t_triangle_data;

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
	t_rectangle_data	rectangle;
	t_triangle_data	triangle;
	t_torus_data	torus;
	t_disk_data		disk;
}				t_shape;

typedef struct	s_rotation_matrix
{
	double3		e1;
	double3		e2;
	double3		e3;
}				t_rotation_matrix;

typedef struct	s_fig
{
	int			fig_type;
	t_shape		shape;

	double3		color;
	int			specular;
	double		reflective;
	double		trans;
	double3		rotation;
	t_rotation_matrix	rotation_martix;
	double		ior;
	int			text_no;
	int			normal_map_no;
	double2		txt_offset;
	double2		txt_scale;

	int		cut;
	t_plane_data	plane;
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
	int			w;
	int			h;
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
	double3		intensity;
	double3		v;
}				t_light;

typedef struct	s_scene
{
	int			count_obj;
	t_fig		obj[MAX_OBJ_COUNT];
	int			count_light;
	t_light		light[MAX_LIGHTING_COUNT];
}				t_scene;

typedef struct	s_txt_params
{
	int		w;
	int		h;
	int		start_pos;
}				t_txt_params;

//main stuff
double3		ray_trace(double3 eye, double3 dir, __global t_scene *scene, double min_range, double max_range, __global uint *texture, __global t_txt_params *txt_params);
double3		canvas_to_viewport(int x, int y, int w, int h, t_pov pov);
double3		calculate_light(__global t_scene *scene, double3 eye, \
						double3 dir, double3 normal, double3 intersect_point, \
						t_fig fig, __global uint *texture, __global t_txt_params *txt_params);




double3		get_intersity_after_shadow_rays(double3 intersect_point, double3 light_dir, \
										__global t_scene *scene, double min_range, \
										double max_range, __global t_light *light);
double3		refract_ray(double3 prim_ray, double3 normal, double ior2_new);
double3		reflected_ray(double3 normal, double3 prim_ray);
double		fresnel(double3 prim_ray, double3 normal, double n1, double reflective);
double3		beers_law(double distance, double3 obj_absorb);


//rotation
double3		rotate_camera(double3 direction, t_pov pov);
double3		rotate_x(double3 v, double angle);
double3		rotate_y(double3 v, double angle);
double3		rotate_z(double3 v, double angle);

//intersections
double2		intersect_sphere(double3 eye, double3 dir, t_sphere_data sphere);
double2		intersect_plane(double3 eye, double3 dir, t_plane_data plane);
double2		intersect_cylin(double3 eye, double3 dir, t_cylin_data cylin);
double2		intersect_cone(double3 eye, double3 dir, t_cone_data cone);
double3		calculate_normal(t_fig fig, double3 intersect_point, t_raytrace_tree curr_node);
t_obj_and_dist		check_closest_inter(double3 eye, double3 dir, \
										__global t_scene *scene, \
										double mini, double max);

//color managment
double3		uint_to_double3(uint a);
double3		trim_color(double3 color);
uint		color_to_canvas(double3 color);


//texture staff
double2		get_sperical_coords(double3 intersect_point, t_fig data);
double2		get_plane_coords(double3 intersect_point, t_fig data);
double2		get_cylin_coords(double3 intersect_point, t_fig data);
double2		get_cone_coords(double3 intersect_point, t_fig data);
double3		new_basis(double3 point, t_rotation_matrix m);
double2		get_texture_space_coords(double3 intersect_point, t_fig data);
uint		get_texture_pixel(double2 coord, __global uint *texture, t_txt_params params, int no);

//other functions
void		swap(double* a, double*b);
double		line_point(double start, double end, double p);

#endif