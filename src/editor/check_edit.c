/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_edit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozhyhadl <ozhyhadl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 18:05:09 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/24 20:54:56 by ozhyhadl         ###   ########.fr       */
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
	printf("d = %f\n", t);
	return (t);
}

int		ft_edit(t_fig *fig)
{
	const Uint8		*k_s = SDL_GetKeyboardState(0);
	
	if (k_s[SDL_SCANCODE_T] && k_s[87])
		fig->trans = ft_clamp(fig->trans += 0.04, 0, 1);
	else if (k_s[SDL_SCANCODE_T] && k_s[86])
		fig->trans = ft_clamp(fig->trans -= 0.04, 0, 1);
	// else if (fig->fig_type == SPHERE)
	// 	fig->shape.sphere.radius++;
	else
		return (0);
	return (1);
}