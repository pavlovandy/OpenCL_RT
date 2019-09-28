/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 11:22:24 by apavlov           #+#    #+#             */
/*   Updated: 2019/09/28 11:22:54 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

cl_double3		canvas_to_viewport(int x, int y, t_pov pov)
{
	return ((cl_double3){{(double)x * pov.vw / pov.w, \
								-(double)y * pov.vh / pov.h, (double)pov.d}});
}

int				changes_norm(t_rt *rt, int *rotations, int *move)
{
	int	changes;

	changes = 0;
	changes += translate(rt);
	if (*rotations)
		changes += rotation(rt);
	else if (*move)
		changes += move_fig(rt);
	return (changes);
}
