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
	scene->count_obj = (cl_int)5;
	scene->count_light = (cl_int)3;
	
	scene->light[0].type_num = (cl_int)DIRECT;
	scene->light[0].intensity = (cl_double)1;
	scene->light[0].v = (cl_double3){{1, 4, 2}};

	scene->light[1].type_num = (cl_int)AMBIENT;
	scene->light[1].intensity = (cl_double)0.4;
	scene->light[1].v = (cl_double3){{0, 0, 0}};

	scene->light[2].type_num = (cl_int)POINT;
	scene->light[2].intensity = (cl_double)1;
	scene->light[2].v = (cl_double3){{2, 1, 0}};

	// scene->obj[0].fig_type = (cl_int)SPHERE;
	// scene->obj[0].shape.sphere.cent = (cl_double3){{0, 0, 4}};
	// scene->obj[0].color = (cl_double3){{0, 0, 0}};
	// scene->obj[0].shape.sphere.radius = (cl_double)1;
	// scene->obj[0].specular = (cl_int)-1;

	// scene->obj[0].reflective = (cl_double)0;
	// scene->obj[0].trans = (cl_double)0.5;
	// scene->obj[0].rotation = (cl_double3){{80.0 * M_PI / 180.0, 10.0 * M_PI / 180.0, 50.0 * M_PI / 180.0}};

	scene->obj[0].fig_type = (cl_int)SPHERE;
	scene->obj[0].shape.sphere.cent = (cl_double3){{-2, 0, 3}};
	scene->obj[0].color = (cl_double3){{0, 0, 255}};
	scene->obj[0].shape.sphere.radius = (cl_double)1;
	scene->obj[0].specular = (cl_int)500;
	scene->obj[0].reflective = (cl_double)0.6;
	scene->obj[0].trans = (cl_double)0.8;
	scene->obj[0].ior = (cl_double)1.4;

	scene->obj[1].fig_type = (cl_int)SPHERE;
	scene->obj[1].shape.sphere.cent = (cl_double3){{0, 0, 4}};
	scene->obj[1].color = (cl_double3){{255, 0, 0}};
	scene->obj[1].shape.sphere.radius = (cl_double)1;
	scene->obj[1].specular = (cl_int)500;
	scene->obj[1].reflective = (cl_double)0.6;
	scene->obj[1].trans = (cl_double)0.8;
	scene->obj[1].ior = (cl_double)1.2;

	scene->obj[2].fig_type = (cl_int)SPHERE;
	scene->obj[2].shape.sphere.cent = (cl_double3){{2, 0, 3}};
	scene->obj[2].shape.sphere.radius = (cl_double)1;
	scene->obj[2].specular = (cl_int)10;
	scene->obj[2].color = (cl_double3){{0, 255, 0}};
	scene->obj[2].reflective = (cl_double)0.2;
	scene->obj[2].trans = (cl_double)0.6;
	scene->obj[2].ior = (cl_double)1.4;

	scene->obj[3].fig_type = (cl_int)SPHERE;
	scene->obj[3].shape.sphere.cent = (cl_double3){{0, -5001, 0}};
	scene->obj[3].shape.sphere.radius = (cl_double)5000;
	scene->obj[3].specular = (cl_int)1000;
	scene->obj[3].color = (cl_double3){{255, 255, 0}};
	scene->obj[3].reflective = (cl_double)0.6;
	scene->obj[3].trans = (cl_double)0;
	scene->obj[3].ior = (cl_double)1.4;

	scene->obj[4].fig_type = (cl_int)SPHERE;
	scene->obj[4].shape.sphere.cent = (cl_double3){{0, 0, 8}};
	scene->obj[4].shape.sphere.radius = (cl_double)2;
	scene->obj[4].specular = (cl_int)10;
	scene->obj[4].color = (cl_double3){{255, 0, 255}};
	scene->obj[4].reflective = (cl_double)0;
	scene->obj[4].trans = (cl_double)0;
	scene->obj[4].ior = (cl_double)1.4;
}

void	init_pov(t_pov *pov)
{
	pov->d = D;
	pov->vw = VW;
	pov->vh = VH;

	pov->cx = cos(0);
	pov->sx = sin(0);
	pov->cy = cos(0);
	pov->sy = sin(0);

	pov->coord = (cl_double3){{0, 0, 0}};
}

int		main(int argc, char **argv)
{
	t_rt	rt;

	if (init_sdl(&rt.sdl))
		return (1);

	rt.envi.txt_count = 1;
	rt.envi.txt = read_texture("envi/8k_earth_daymap.jpg", &rt.envi.txt_par);

	if (rt.envi.txt == 0)
		return (error_message(RED"texture failure"COLOR_OFF));
	if (init_cl(&rt.cl))
		return (1);
	if (create_program_and_kernels(&rt.cl))
		return (1);
	init_pov(&rt.pov);
	make_little_default_scene(&rt.scene);

	if (set_up_memory(&rt, &rt.cl))
		return (1);
	render_scene(&rt);
	there_will_be_loop(&rt);

	close_sdl(&rt.sdl);
	freed_up_memory(&rt.cl);
	return (0);
}
