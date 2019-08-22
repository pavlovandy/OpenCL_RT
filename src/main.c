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

	scene->obj[0].fig_type = SPHERE;
	scene->obj[0].shape.sphere.cent = (cl_float3){{0, 1, 5}};
	scene->obj[0].color = (cl_float3){{0, 255, 0}};
	scene->obj[0].shape.sphere.radius = (cl_float)1;
	scene->obj[0].specular = (cl_int)5;
	
	/*
	**для того щоб звернутися окремо до якогось елемента вектора юзай
	** cl_float3	a;
	** a.v4[0] = 1; для x
	** a.v4[1] = 1; для у
	** a.v4[2] = 1; для z
	*/

	scene->obj[1].fig_type = SPHERE;
	scene->obj[1].shape.sphere.cent = (cl_float3){{0, 0, 5}};
	scene->obj[1].color = (cl_float3){{0, 0, 255}};
	scene->obj[1].shape.sphere.radius = (cl_float)1.5;
	scene->obj[1].specular = (cl_int)5;
}

void	init_pov(t_pov *pov)
{
	pov->d = D;
	pov->vw = VW;
	pov->vh = VH;
}

int		main(int argc, char **argv)
{
	t_rt	rt;

	if (init_sdl(&rt.sdl))
		return (1);

	if (init_cl(&rt.cl))
		return (1);
	if (create_program_and_kernels(&rt.cl))
		return (1);

	make_little_default_scene(&rt.scene);

	if (set_up_memory(rt, &rt.cl))
		return (1);

	render_scene(&rt);
	there_will_be_loop(&rt);

	close_sdl(&rt.sdl);
	freed_up_memory(&rt.cl);
	return (0);
}
