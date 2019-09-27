/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xml_cub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozhyhadl <ozhyhadl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 19:41:54 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/27 18:48:59 by ozhyhadl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/rt.h"

void	ft_create_rec_cube(t_scene *scene, int *il, t_filters *filter)
{
	int i;

	i = -1;
	while (++i < 6)
	{
		il[0]++;
		if (ft_check_count(il[0], il[1], il[2], il[3]))
			exit(1);
		ft_create_rectangle(scene, il[0], filter);
		scene->obj[il[0]].complex_fig = il[3];
		scene->obj[il[0]].color = (cl_double3){{255 / 6 * i, 255 - 255 / 6 * i, 0 }};
	}
	scene->count_obj = il[0] + 1;
}

void	ft_fill_vertex(cl_double3 *vertex, t_cube cube)
{
	double	a;
	int		i;

	a = cube.dist;
	vertex[0] = (cl_double3){{a, a, -a}};
	vertex[1] = (cl_double3){{a, -a, -a}};
	vertex[2] = (cl_double3){{a, -a, a}};
	vertex[3] = (cl_double3){{a, a, a}};
	vertex[4] = (cl_double3){{-a, a, -a}};
	vertex[5] = (cl_double3){{-a, -a, -a}};
	vertex[6] = (cl_double3){{-a, -a, a}};
	vertex[7] = (cl_double3){{-a, a, a}};
	i = -1;
	while (++i < 8)
		vertex[i] = add_double3(new_basis(vertex[i], cube.rotation_matrix), cube.cent);
}

t_rectangle_data	set_rectangles(cl_double3 *v, int r)  //dobavity v functions
{
	if (r == 0)
		return ((t_rectangle_data){v[0], v[1], v[2], v[3]});
	else if (r == 1)
		return ((t_rectangle_data){v[0], v[3], v[7], v[4]});
	else if (r == 2)
		return ((t_rectangle_data){v[3], v[7], v[6], v[2]});
	else if (r == 3)
		return ((t_rectangle_data){v[6], v[2], v[1], v[5]});
	else if (r == 4)
		return ((t_rectangle_data){v[7], v[6], v[5], v[4]});
	else
		return ((t_rectangle_data){v[0], v[4], v[5], v[1]});
}

void rewrite_rectangle_data(t_fig *src, t_fig *dst)
{
	src->color = dst->color;
	src->specular = dst->specular;
	src->reflective = dst->reflective;
	src->trans = dst->trans;
	src->ior = dst->ior;
	src->noise = dst->noise;
	src->text_no = dst->text_no;
	src->normal_map_no = dst->normal_map_no;
	src->transparancy_map_no = dst->transparancy_map_no;
	src->txt_offset = dst->txt_offset;
	src->txt_scale = dst->txt_scale;
	src->color = dst->color;
	src->cutting = 0;
}

void	ft_fill_rectangle(t_scene *scene, int cub, t_fig *fig, t_filters *filter) //dobavity v functions
{
	int i;
	int r;
	cl_double3	v[8];

	i = -1;
	r = 0;
	ft_fill_vertex(v, scene->cubs[cub]);
	while (++i < scene->count_obj)
	{
		if (scene->obj[i].complex_fig == cub)
		{
			scene->obj[i].shape.rectangle = set_rectangles(v, r);
			scene->obj[i].rotation_martix = build_rotation_matrix_for_rectangles(get_obj_dir(scene->obj[i]),\
							 scene->obj[i].shape.rectangle);
			if (fig != NULL)
				rewrite_rectangle_data(scene->obj + i, fig);
			if (filter != NULL)
			{
				filter->obj_movement[i].move = 0;
				filter->obj_movement[i].dir = (cl_double3){{0, 0, 0}};
			}
			r++;
		}
	}
}

void	ft_create_cube(t_scene *scene, t_filters *filter, int *il)
{
	il[3]++;
	il[0]--;
	scene->cubs[il[3]].cent = (cl_double3){{0, 0, 0}};
	scene->cubs[il[3]].dist = (cl_double)1;
	scene->cubs[il[3]].no = il[3];
	scene->cubs[il[3]].rotation = (cl_double3){{0, 0, 0}};
	scene->cubs[il[3]].rotation_matrix = \
	build_rotation_matrix_form_angles(scene->cubs[il[3]].rotation);
	ft_create_rec_cube(scene, il, filter);
	ft_fill_rectangle(scene, il[3], NULL, filter);
}
