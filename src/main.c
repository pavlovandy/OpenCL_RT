/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anri <anri@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 13:40:05 by apavlov           #+#    #+#             */
/*   Updated: 2019/09/28 01:10:59 by anri             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"
	#include <time.h>
#include <stdlib.h>


int		main(int argc, char **argv)
{
	t_rt	rt;

	if (init_start_params(&rt))
		return (error_message(RED"Couldnt init params"COLOR_OFF));
	if (argc == 2)
	{
		if (ft_parse_xml(argv[1], &rt.scene, &rt.pov, &rt))
			return (1);
	}
	else
		return (print_usage());
	rt.scene.obj[0].noise = 0;
	if (init_sdl(&rt.sdl, rt.pov.w, rt.pov.h))
		return (1);
	if (init_but(&rt))
		return (1);
	if (init_cl(&rt.cl))
		return (1);
	if (create_program_and_kernels(&rt.cl))
		return (1);
	if (set_up_memory(&rt, &rt.cl))
		return (1);
	there_will_be_loop(&rt);
	close_sdl(&rt.sdl);
	freed_up_memory(&rt.cl, &rt);
	return (0);
}
