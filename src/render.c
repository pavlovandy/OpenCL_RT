/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 13:52:27 by apavlov           #+#    #+#             */
/*   Updated: 2019/08/21 13:52:27 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

int			render_scene(t_rt *rt)
{
	int		ret;
	cl_uint	*pixels;
	int		i;

	pixels = (cl_uint*)rt->sdl.win_sur->pixels;
	ret = clEnqueueNDRangeKernel(rt->cl.command_queue, rt->cl.kernel, 1, NULL, \
					&rt->cl.global_size, &rt->cl.local_size, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(rt->cl.command_queue, rt->cl.pixel_ptr, CL_FALSE, 0,
		sizeof(cl_uint) * WIN_WIDTH * WIN_HEIGHT, rt->cl.pixels_to_read_into, 0, 0, 0);
	
	if (ret != CL_SUCCESS)
		return (error_message(RED"Oops"COLOR_OFF));
	clFinish(rt->cl.command_queue);

	i = -1;
	while (++i < (WIN_HEIGHT * WIN_WIDTH))
		pixels[i] = rt->cl.pixels_to_read_into[i];

	SDL_UpdateWindowSurface(rt->sdl.win);
	return (0);
}