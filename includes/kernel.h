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

// # ifdef __APPLE__
// #  include <OpenCL/opencl.h>
// # else
// #  include <CL/cl.h>
// # endif

# define MAX_OBJ_COUNT 20
# define MAX_LIGHTING_COUNT 10

enum	e_fig
{
	SPHERE = 0, PLANE, CONE, CYLIN
};
enum	e_light
{
	AMBIENT = 0, DIRECT, POINT
};

typedef struct	s_sphere_data
{
	float3	cent;
	double		radius;
}				t_sphere_data;

typedef struct	s_plane_data
{
	float3	normal;
	float3	dot;
}				t_plane_data;

typedef struct	s_cone_data
{
	float3	vertex;
	float3	dir;
	double		tangent;
}				t_cone_data;

typedef struct	s_cylin_data
{
	float3	dir;
	float3	dot;
	double		radius;
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

	float3	color;
	int			specular;
}				t_fig;

typedef struct	s_pov
{
	float3	coord;
	float3	dir;
	double		cx;
	double		cy;
	double		sx;
	double		sy;
}				t_pov;

typedef struct	s_light
{
	char		*type;
	int			type_num;
	double		intensity;
	float3	v;
}				t_light;

typedef struct	s_scene
{
	int			count_obj;
	t_fig		obj[MAX_OBJ_COUNT];
	int			count_light;
	t_light		light[MAX_LIGHTING_COUNT];
}				t_scene;

#endif