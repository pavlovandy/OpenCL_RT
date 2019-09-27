/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yruda <yruda@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 18:05:09 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/27 14:58:58 by yruda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/rt.h"

double	ft_clamp(double s, double min, double max)
{
	double t;

	t = s;
	if (s <= min)
		t = min;
	else if (s >= max)
		t = max;
	return (t);
}

cl_double3	vector_rotation(const Uint8 *k_s, cl_double3 n, double angle)
{
	if (k_s[SDL_SCANCODE_KP_8])
		n = (rotate_x(n, angle));
	else if (k_s[SDL_SCANCODE_KP_2])
		n = (rotate_x(n, -angle));
	else if (k_s[SDL_SCANCODE_KP_4])
		n = (rotate_y(n, -angle));
	else if (k_s[SDL_SCANCODE_KP_6])
		n = (rotate_y(n, angle));
	else if (k_s[SDL_SCANCODE_KP_7] || k_s[SDL_SCANCODE_KP_1])
		n = (rotate_z(n, angle));
	else if (k_s[SDL_SCANCODE_KP_9] || k_s[SDL_SCANCODE_KP_3])
		n = (rotate_z(n, -angle));
	return (n);
}

cl_double3	normal_direction_rotation(const Uint8 *k_s, cl_double3 n, double angle, t_fig *fig)
{
	n = vector_rotation(k_s, n, angle);
	fig->rotation_martix = build_rotation_matrix_for_dir(n);
	if (fig->cutting)
	{
		fig->cutting_plane.dot = vector_rotation(k_s, fig->cutting_plane.dot, angle);
		fig->cutting_plane.normal = vector_rotation(k_s, fig->cutting_plane.normal, angle);
	}
	return (n);
}

void	sphere_editor(t_fig *fig, const Uint8 *k_s)
{
	if (k_s[SDL_SCANCODE_R] && k_s[87])
			fig->shape.sphere.radius =
				ft_clamp(fig->shape.sphere.radius + 0.1, 0.01, BIG_VALUE);
	else if (k_s[SDL_SCANCODE_R] && k_s[86])
		fig->shape.sphere.radius =
		ft_clamp(fig->shape.sphere.radius - 0.1, 0.01, BIG_VALUE);
	else if (k_s[SDL_SCANCODE_U])//можна перемістити вище, для всіх фігур. але красиво і доцільно це лише для сфери
	{
		if (k_s[SDL_SCANCODE_KP_2])
			fig->rotation.s[0] -= 0.05;
		else if (k_s[SDL_SCANCODE_KP_8])
			fig->rotation.s[0] += 0.05;
		else if (k_s[SDL_SCANCODE_KP_4])
			fig->rotation.s[1] += 0.05;
		else if (k_s[SDL_SCANCODE_KP_6])
			fig->rotation.s[1] -= 0.05;
		else if (k_s[SDL_SCANCODE_KP_7] || k_s[SDL_SCANCODE_KP_1])
			fig->rotation.s[2] += 0.1;
		else if (k_s[SDL_SCANCODE_KP_9] || k_s[SDL_SCANCODE_KP_3])
			fig->rotation.s[2] -= 0.1;
		fig->rotation_martix = build_rotation_matrix_form_angles(fig->rotation);
	}
}

/*
**	T [+/-] - changes transparency
**	Y - changes textures
**	I [+/-] - changes ior
**	U & [1...9] - rotating texture in sphere
**	N & [1...9] rotating normal/direction and cutting plane if such exists
**	
*/

int		ft_edit(t_fig *fig, t_rt *rt)
{
	const Uint8	*k_s;
	
	k_s = SDL_GetKeyboardState(0);
	if (k_s[SDL_SCANCODE_T] && k_s[87])
		fig->trans = ft_clamp(fig->trans + 0.04, 0, 1);
	else if (k_s[SDL_SCANCODE_T] && k_s[86])
		fig->trans = ft_clamp(fig->trans - 0.04, 0, 1);
	else if (k_s[SDL_SCANCODE_I] && k_s[87])
		fig->ior = ft_clamp(fig->ior + 0.04, MIN_IOR, MAX_IOR);
	else if (k_s[SDL_SCANCODE_I] && k_s[86])
		fig->ior = ft_clamp(fig->ior - 0.04, MIN_IOR, MAX_IOR);
		//place for MYulia's function
	else if (k_s[SDL_SCANCODE_Y])
	{	change_texture(fig);
		printf("text: %d", fig->text_no);
	}
	else if (k_s[SDL_SCANCODE_N])
	{
		if (fig->fig_type == CONE)
			fig->shape.cone.dir =
				normal_direction_rotation(k_s, fig->shape.cone.dir, 0.2, fig);
		else if (fig->fig_type == PLANE)
			fig->shape.plane.normal =
				normal_direction_rotation(k_s, fig->shape.plane.normal, 0.2, fig);
		else if (fig->fig_type == DISK)
			fig->shape.disk.normal =
				normal_direction_rotation(k_s, fig->shape.disk.normal, 0.2, fig);
		else if (fig->fig_type == CYLIN)
			fig->shape.cylin.dir =
				normal_direction_rotation(k_s, fig->shape.cylin.dir, 0.2, fig);
	}
	else if (k_s[SDL_SCANCODE_KP_PLUS])
	{
		printf("HEY, jude\n");
		change_colours(fig);
	}
	else if (fig->fig_type == SPHERE)
		sphere_editor(fig, k_s);
	else if (fig->fig_type == CONE)
	{
		if (k_s[SDL_SCANCODE_R] && k_s[87])
			fig->shape.cone.tangent =
				ft_clamp(fig->shape.cone.tangent + 0.05, 0.01, 3.14);
		else if (k_s[SDL_SCANCODE_R] && k_s[86])
			fig->shape.cone.tangent =
				ft_clamp(fig->shape.cone.tangent - 0.05, 0.01, 3.14);
	}
	else if (fig->fig_type == DISK)
	{
		if (k_s[SDL_SCANCODE_R] && k_s[87])
			fig->shape.disk.radius =
				ft_clamp(fig->shape.disk.radius + 0.1, 0.01, BIG_VALUE);
		else if (k_s[SDL_SCANCODE_R] && k_s[86])
			fig->shape.disk.radius =
				ft_clamp(fig->shape.disk.radius - 0.1, 0.01, BIG_VALUE);
	}
	else if (fig->fig_type == CYLIN)
	{
		if (k_s[SDL_SCANCODE_R] && k_s[87])
			fig->shape.cylin.radius =
				ft_clamp(fig->shape.disk.radius + 0.1, 0.01, BIG_VALUE);
		else if (k_s[SDL_SCANCODE_R] && k_s[86])
			fig->shape.cylin.radius =
				ft_clamp(fig->shape.disk.radius - 0.1, 0.01, BIG_VALUE);
	}
	else
		return (0);
	return (1);
}