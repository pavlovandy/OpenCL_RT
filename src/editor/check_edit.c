/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_edit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yruda <yruda@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 18:05:09 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/25 16:47:36 by yruda            ###   ########.fr       */
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

void	change_texture(t_fig *fig, t_rt *rt)
{
	if (fig->text_no >= 3)
		fig->text_no = 0;
	else
		fig->text_no++;
	printf("texture: %d\n", fig->text_no);
}

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
	else if (k_s[SDL_SCANCODE_Y])
		change_texture(fig, rt);
	// else if (fig->fig_type == SPHERE)
	// {
	// 	if (k_s[SDL_SCANCODE_R] && k_s[87])
	// 		fig->shape.sphere.radius =
	// 			ft_clamp(fig->shape.sphere.radius + 1, 1, BIG_VALUE);
	// 	if (k_s[SDL_SCANCODE_R] && k_s[86])
	// 		fig->shape.sphere.radius =
	// 		ft_clamp(fig->shape.sphere.radius - 1, 1, BIG_VALUE);
	// }
	// else if (fig->fig_type == CONE)
	// {
	// 	if (k_s[SDL_SCANCODE_R] && k_s[87])
	// 		fig->shape.cone.tangent =
	// 			ft_clamp(fig->shape.cone.tangent + 0.1, -1.73, BIG_VALUE);///which are the limits?
	// 	if (k_s[SDL_SCANCODE_R] && k_s[86])
	// 		fig->shape.cone.tangent =
	// 			ft_clamp(fig->shape.cone.tangent - 0.1, -1.73, BIG_VALUE);
	// }
	// else if (fig->fig_type == DISK)
	// {
	// 	if (k_s[SDL_SCANCODE_R] && k_s[87])
	// 		fig->shape.disk.radius =
	// 			ft_clamp(fig->shape.disk.radius + 1, 1, BIG_VALUE);
	// 	if (k_s[SDL_SCANCODE_R] && k_s[86])
	// 		fig->shape.disk.radius =
	// 			ft_clamp(fig->shape.disk.radius - 1, 1, BIG_VALUE);
	// }
	
	// else if (k_s[SDL_SCANCODE_R] && k_s[86])//////
	// 	fig->trans = ft_clamp(fig->trans -= 0.04, 0, 1);
	
	else
		return (0);
	printf("ior changing: %.4f", fig->ior);
	return (1);
}