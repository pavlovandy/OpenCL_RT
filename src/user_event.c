/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_event.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myuliia <myuliia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 14:15:40 by apavlov           #+#    #+#             */
/*   Updated: 2019/09/27 16:08:54 by myuliia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

static int	translate(t_rt *rt)
{
	const Uint8		*keyboard_state = SDL_GetKeyboardState(0);
	cl_double3		translate_vector;

	translate_vector = (cl_double3){{0, 0, 0}};
	if (keyboard_state[SDL_SCANCODE_D])
		translate_vector.s[0] += TRANSLATE_SPEED;
	if (keyboard_state[SDL_SCANCODE_A])
		translate_vector.s[0] -= TRANSLATE_SPEED;
	if (keyboard_state[SDL_SCANCODE_W])
		translate_vector.s[2] += TRANSLATE_SPEED;
	if (keyboard_state[SDL_SCANCODE_S])
		translate_vector.s[2] -= TRANSLATE_SPEED;
	if (keyboard_state[SDL_SCANCODE_SPACE])
		translate_vector.s[1] += TRANSLATE_SPEED;
	if (keyboard_state[SDL_SCANCODE_LCTRL])
		translate_vector.s[1] -= TRANSLATE_SPEED;
	translate_vector = new_basis(translate_vector, rt->pov.pov_rm);
	rt->pov.coord = add_double3(rt->pov.coord, translate_vector);
	if (vector_len(translate_vector) != 0)
		return (1);
	return (0);
}

int			rotation(t_rt *rt) //sometimes its must rotate z coordinate also
{
	int				x;
	int				y;
	cl_double3		rotate_vector;
	

	SDL_GetRelativeMouseState(&x, &y);
	if (x == 0 && y == 0)
		return (0);
	rotate_vector.s[0] = (y) * ROTATION_SPEED;
	rotate_vector.s[1] = (x) * ROTATION_SPEED;
	rotate_vector.s[2] = 0;

	rt->pov.pov_rm = rotate_matrix_of_rotation(rt->pov.pov_rm, rotate_vector);
	rt->pov.dir = find_angles_from_rotation_matrix(rt->pov.pov_rm);
	
	return (1);
}

cl_double3	canvas_to_viewport(int x, int y, t_pov pov)
{
	return ((cl_double3){{(double)x * pov.vw / pov.w, \
								-(double)y * pov.vh / pov.h, (double)pov.d}});
}

static void	cool_little_function(t_rt *rt, cl_double3 *data, cl_double3 dir)
{
	cl_double3		oc;
	
	oc = minus_double3(rt->pov.coord, *data);
	dir = increase_double3(dir, vector_len(oc));
	*data = add_double3(*data, dir);
}

int		move_fig(t_rt *rt)
{
	int		x;
	int		y;
	t_fig	*obj;
	cl_double3	*dot;
	cl_int	ret;
	int		g_x;
	int		g_y;
	double		dir_len;

	
	if (rt->edi.chosen_obj != -1)
	{
		SDL_GetRelativeMouseState(&x, &y);
		if (x == 0 && y == 0)
			return(0);
		SDL_GetMouseState(&g_x, &g_y);
		obj = rt->scene.obj + rt->edi.chosen_obj;
		dir_len = vector_len(canvas_to_viewport(g_x, g_y, rt->pov));	
		cl_double3	dir = (cl_double3){{x * rt->pov.vh / (rt->pov.w * dir_len), (-y) * rt->pov.vw / (rt->pov.h * dir_len), 0}};
		dir = new_basis(dir, rt->pov.pov_rm);

		dot = get_obj_dot(obj);
		cool_little_function(rt, dot, dir);

		ret = clEnqueueWriteBuffer(rt->cl.command_queue, rt->cl.scene_mem, CL_TRUE, 0, sizeof(t_scene), &rt->scene, 0, 0, 0);
		if (ret != CL_SUCCESS)
			exit(error_message(RED"Something went bad\n"COLOR_OFF));
		return (1);
	}
	return (0);
}
int			mouse_events(t_rt *rt, Uint8 button, int x, int y)
{
	if (button == SDL_BUTTON_LEFT)
	{
		rt->edi.chosen_obj = get_object_intersection(rt, x, y);
		return (1);
	}
	else
		return (0);
}

int			user_events(t_rt *rt)
{
	SDL_Event	ev;
	int			changes;
	static int	rotations = 0;
	static int	move = 0;
	int		ret;
	changes = 0;
	changes += translate(rt);
	if (rotations)
		changes += rotation(rt);
	else if (move)
		changes += move_fig(rt);
	while (SDL_PollEvent(&ev))
	{
		if (ev.type == SDL_KEYDOWN)
		{
			if (ev.key.keysym.sym == SDLK_1 || ev.key.keysym.sym == SDLK_2 ||
			ev.key.keysym.sym == SDLK_3 || ev.key.keysym.sym == SDLK_4 ||
			ev.key.keysym.sym == SDLK_5 || ev.key.keysym.sym == SDLK_6)
			{
				change_filters(rt, ev);
				changes++;
			}
			
			if (ev.key.keysym.sym == SDLK_ESCAPE)
			{
				// system("leaks -q RT");
					exit(error_message(GREEN"Bye bye"COLOR_OFF) - 1);
			}
			else if (ev.key.keysym.sym == SDLK_0)
			{
				rotations = !rotations;
				SDL_SetRelativeMouseMode(rotations);
				SDL_GetRelativeMouseState(NULL, NULL);
			}
			else if (ev.key.keysym.sym == SDLK_m)
			{
				rt->filters.obj_movement[0].move = 1;
				rt->filters.obj_movement[0].dir = (cl_double3){{0, 0, 0.05}};
				rt->filters.motion = !rt->filters.motion;
				changes = 1;
			}
		}
		else if (ev.type == SDL_QUIT)
			exit(error_message(GREEN"Bye bye"COLOR_OFF) - 1);
		else if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
					if (ev.button.button == SDL_BUTTON_LEFT)
						changes += pres_buttn(rt, ev.button.x, ev.button.y);
				move = mouse_events(rt, ev.button.button, ev.button.x, ev.button.y);
				SDL_GetRelativeMouseState(NULL, NULL);
			}
		
		else if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT)
			move = 0;
		else if (rt->edi.chosen_obj != -1)
		{
				if ((changes += ft_edit(rt->scene.obj + rt->edi.chosen_obj, rt)))
				{
					ret = clEnqueueWriteBuffer(rt->cl.command_queue, rt->cl.scene_mem, CL_TRUE, 0, sizeof(t_scene), &rt->scene, 0, 0, 0);
					if (ret != CL_SUCCESS)
					exit(error_message(RED"Something went bad\n"COLOR_OFF));
				}
		}
	}
	return (changes);
}

int			there_will_be_loop(t_rt *rt)
{
	int	ret;

	render_scene(rt);
	while (1)
	{
		
		if (user_events(rt))
		{
			/*There should be relinkage to OpenCL*/
			ret = clSetKernelArg(rt->cl.rt_kernel, 2, sizeof(rt->pov), &rt->pov);
			if (ret != CL_SUCCESS)
				return (error_message(RED"clSetKernelArg(4) exception"COLOR_OFF));
			/*__________________________________________________*/


			render_scene(rt);
		}
	}
	return (0);
}
