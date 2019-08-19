/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 13:40:05 by apavlov           #+#    #+#             */
/*   Updated: 2019/08/19 13:40:05 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

void	make_little_default_scene(t_scene *scene)
{
	scene->count_obj = 2;
	scene->count_light = 2;
	
	scene->light[0].type_num = DIRECT;
	scene->light[0].type = ft_strdup("direct");
	scene->light[0].intensity = 0.5;
	scene->light[0].v = (cl_float3){{0, 0, 0}};
	scene->light[1].type_num = AMBIENT;
	scene->light[1].type = ft_strdup("ambient");
	scene->light[1].intensity = 0.3;

	t_sphere_data	*data;

	data = malloc(sizeof(t_sphere_data));
	scene->obj[0].fig_type = SPHERE;
	data->cent = (cl_float3){{0, 0, 0, 0}};
	data->color = (cl_float3){{0, 0, 0, 0}};
	data->radius = (cl_float)5;
	data->specular = (cl_int)5;
	scene->obj[0].data = data;

	data = malloc(sizeof(t_sphere_data));
	scene->obj[1].fig_type = SPHERE;
	data->cent = (cl_float3){{0, 0, 0}};
	data->color = (cl_float3){{0, 0, 0}};
	data->radius = (cl_float)5;
	data->specular = (cl_int)5;
	scene->obj[1].data = data;
}

int		main(int argc, char **argv)
{
	t_rt	rt;

	if (init_cl(&rt.cl))
		return (1);
	if (create_program_and_kernels(&rt.cl))
		return (1);
	make_little_default_scene(&rt.scene);
	if (init_sdl(&rt.sdl))
		return (1);
	return (0);
}
