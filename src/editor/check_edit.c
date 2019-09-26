/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_edit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yruda <yruda@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 18:05:09 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/26 13:20:49 by yruda            ###   ########.fr       */
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

/*
**	put in the number of textures that are properly textures
*/

void	change_texture(t_fig *fig)
{
	if (fig->text_no >= 3)
		fig->text_no = 0;
	else
		fig->text_no++;
	printf("texture: %d\n", fig->text_no);
}

cl_double3	normal_direction_rotation(const Uint8 *k_s, cl_double3 n, double angle)
{
	if (k_s[SDL_SCANCODE_KP_8])
		return (rotate_x(n, angle));
	else if (k_s[SDL_SCANCODE_KP_2])
		return (rotate_x(n, -angle));
	else if (k_s[SDL_SCANCODE_KP_4])
		return (rotate_y(n, -angle));
	else if (k_s[SDL_SCANCODE_KP_6])
		return (rotate_y(n, angle));
	else if (k_s[SDL_SCANCODE_KP_7] || k_s[SDL_SCANCODE_KP_1])
		return (rotate_z(n, angle));
	else if (k_s[SDL_SCANCODE_KP_9] || k_s[SDL_SCANCODE_KP_3])
		return (rotate_z(n, -angle));
	return (n);
}

/*
**	T [+/-] - changes transparency
**	Y - changes textures
**	I [+/-] - changes ior
**	U & [1...9] - rotating texture in sphere
**	N [1...9] rotationg normal/direction
*/

int		ft_edit(t_fig *fig)
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
	else if (k_s[SDL_SCANCODE_Y])
		change_texture(fig);

	else if (fig->fig_type == SPHERE)
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
				fig->rotation.x -= 0.05;
			else if (k_s[SDL_SCANCODE_KP_8])
				fig->rotation.x += 0.05;
			else if (k_s[SDL_SCANCODE_KP_4])
				fig->rotation.y += 0.05;
			else if (k_s[SDL_SCANCODE_KP_6])
				fig->rotation.y -= 0.05;
			else if (k_s[SDL_SCANCODE_KP_7] || k_s[SDL_SCANCODE_KP_1])
				fig->rotation.z += 0.1;
			else if (k_s[SDL_SCANCODE_KP_9] || k_s[SDL_SCANCODE_KP_3])
				fig->rotation.z -= 0.1;
			fig->rotation_martix = build_rotation_matrix_form_angles(fig->rotation);
		}
	}
	else if (fig->fig_type == CONE)
	{
		if (k_s[SDL_SCANCODE_R] && k_s[87])
			fig->shape.cone.tangent =
				ft_clamp(fig->shape.cone.tangent + 0.05, 0.01, 3.14);///which are the limits?
		else if (k_s[SDL_SCANCODE_R] && k_s[86])
			fig->shape.cone.tangent =
				ft_clamp(fig->shape.cone.tangent - 0.05, 0.01, 3.14);
		else if (k_s[SDL_SCANCODE_N])
			fig->shape.cone.dir =
				normal_direction_rotation(k_s, fig->shape.cone.dir, 0.2);
	}
	else if (fig->fig_type == PLANE)
	{
		if (k_s[SDL_SCANCODE_N])
			fig->shape.plane.normal =
				normal_direction_rotation(k_s, fig->shape.plane.normal, 0.2);
	}
	else if (fig->fig_type == DISK)
	{
		if (k_s[SDL_SCANCODE_R] && k_s[87])
			fig->shape.disk.radius =
				ft_clamp(fig->shape.disk.radius + 1, 1, BIG_VALUE);
		else if (k_s[SDL_SCANCODE_R] && k_s[86])
			fig->shape.disk.radius =
				ft_clamp(fig->shape.disk.radius - 1, 1, BIG_VALUE);
	}
	
	// else if (k_s[SDL_SCANCODE_R] && k_s[86])//////
	// 	fig->trans = ft_clamp(fig->trans -= 0.04, 0, 1);
	
	else
		return (0);
	return (1);
}