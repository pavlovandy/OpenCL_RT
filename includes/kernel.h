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

#endif