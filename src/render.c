/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myuliia <myuliia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 13:52:27 by apavlov           #+#    #+#             */
/*   Updated: 2019/09/27 16:08:18 by myuliia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

void		move_obj_a_little(t_rt *rt)
{
	int			i;
	cl_double3	*dot;
	t_obj_movement	move;
	int			ret;

	i = -1;
	while (++i < rt->scene.count_obj)
	{
		move = rt->filters.obj_movement[i];
		if (move.move)
		{
			dot = get_obj_dot(rt->scene.obj + i);
			*dot = add_double3(*dot, move.dir);
		}
	}
	ret = clEnqueueWriteBuffer(rt->cl.command_queue, rt->cl.scene_mem, CL_TRUE, 0, sizeof(t_scene), &rt->scene, 0, 0, 0);
	if (ret != CL_SUCCESS)
		exit(error_message(RED"Something went bad\n"COLOR_OFF));
}

void		set_obj_back(t_rt *rt)
{
	int			i;
	cl_double3	*dot;
	t_obj_movement	move;
	int			ret;

	i = -1;
	while (++i < rt->scene.count_obj)
	{
		move = rt->filters.obj_movement[i];
		if (move.move)
		{
			dot = get_obj_dot(rt->scene.obj + i);
			move.dir = increase_double3(move.dir, RENDER_ITARATION);
			*dot = minus_double3(*dot, move.dir);
		}
	}
	ret = clEnqueueWriteBuffer(rt->cl.command_queue, rt->cl.scene_mem, CL_TRUE, 0, sizeof(t_scene), &rt->scene, 0, 0, 0);
	if (ret != CL_SUCCESS)
		exit(error_message(RED"Something went bad\n"COLOR_OFF));
}

void		add_to_buff_to_array(cl_double3 *arr, Uint32 *buff, t_rt *rt)
{
	int		x;
	int		y;
	int		indx;

	y = -1;
	while (++y < rt->pov.h)
	{
		x = -1;
		while (++x < rt->pov.w)
		{
			indx = y * rt->pov.w + x;
			arr[indx].s[0] += (buff[indx] >> 16 & 0xff);
			arr[indx].s[1] += (buff[indx] >> 8 & 0xff);
			arr[indx].s[2] += (buff[indx] & 0xff);
		}	
	}
}

void		array_to_screen_pixels(Uint32 *pix, cl_double3 *arr, int div, t_rt *rt)
{
	int		x;
	int		y;
	int		indx;

	y = -1;
	while (++y < rt->pov.h)
	{
		x = -1;
		while (++x < rt->pov.w)
		{
			indx = y * rt->pov.w + x;
			pix[indx] = ((Uint32)(arr[indx].s[0] / div) << 16) + \
			((Uint32)(arr[indx].s[1] / div) << 8) + \
			((Uint32)(arr[indx].s[2] / div));
		}
	}
	
}

int			render_scene(t_rt *rt)
{
	int		ret;
	cl_uint	*pixels;
	int		i;

	pixels = (cl_uint*)rt->sdl.win_sur->pixels;

	if (rt->filters.motion)
	{
		ft_bzero(rt->filters.colors, sizeof(cl_double3) * rt->pov.w * rt->pov.h);
		i = -1;
		while (++i < RENDER_ITARATION)
		{
			move_obj_a_little(rt);
			ret = clEnqueueNDRangeKernel(rt->cl.command_queue, rt->cl.rt_kernel, 1, NULL, \
						&rt->cl.global_size, &rt->cl.local_size, 0, NULL, NULL);
			if (ret != CL_SUCCESS)
				return (error_message(RED"Oops"COLOR_OFF));
			ret = clEnqueueReadBuffer(rt->cl.command_queue, rt->cl.pixel_ptr, CL_FALSE, 0,
					sizeof(cl_uint) * rt->pov.w * rt->pov.h, rt->filters.buff, 0, 0, 0);

			clFinish(rt->cl.command_queue);
			add_to_buff_to_array(rt->filters.colors, rt->filters.buff, rt);
		}

		set_obj_back(rt);
		array_to_screen_pixels(pixels, rt->filters.colors, RENDER_ITARATION, rt);
	}
	else
	{
		ret = clEnqueueNDRangeKernel(rt->cl.command_queue, rt->cl.rt_kernel, 1, NULL, \
						&rt->cl.global_size, &rt->cl.local_size, 0, NULL, NULL);
		if (ret != CL_SUCCESS)
			return (error_message(RED"Oops"COLOR_OFF));
		ret = clEnqueueReadBuffer(rt->cl.command_queue, rt->cl.pixel_ptr, CL_FALSE, 0,
			sizeof(cl_uint) * rt->pov.w * rt->pov.h, pixels, 0, 0, 0);
		if (ret != CL_SUCCESS)
			return (error_message(RED"Oops"COLOR_OFF));		
		ret = clEnqueueReadBuffer(rt->cl.command_queue, rt->cl.zbuff, CL_FALSE, 0,
			sizeof(cl_uint) * rt->pov.w * rt->pov.h, rt->filters.zbuff, 0, 0, 0);
		if (ret != CL_SUCCESS)
			return (error_message(RED"Oops"COLOR_OFF));		
		clFinish(rt->cl.command_queue);		
	}
	
	i = -1;
	// while (++i < rt->pov.w * rt->pov.h)
	// 	pixels[i] = ((int)((CLAMP(rt->filters.zbuff[i], 0.0, 50.0) / 50.0) * 255)) << 16;
	//add_filter(rt);

	

	// while (++i < rt->pov.w * rt->pov.h)
	// {
	// 	double dist = CLAMP(rt->filters.zbuff[i], 0.0, 50.0) / 50.0;
	// 	cl_double3	color = (cl_double3){{(pixels[i] >> 16) & 0xff, (pixels[i] >> 8) & 0xff, (pixels[i]) & 0xff}};
	// 	color.s[0] = color.s[0] * exp(-10 * dist);
	// 	color.s[1] = color.s[1] * exp(-10 * dist);
	// 	color.s[2] = color.s[2] * exp(-10 * dist);
	// 	pixels[i] = ((int)color.s[0] << 16) + ((int)color.s[1] << 8) + (int)color.s[2];
	// }



	 if  (rt->filters.index_filter != 0)
	 	add_filter(rt);
	apply_surface(rt->sdl.win_sur, rt);
	SDL_UpdateWindowSurface(rt->sdl.win);
	return (0);
}