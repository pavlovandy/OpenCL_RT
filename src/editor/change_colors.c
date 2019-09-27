/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_colors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yruda <yruda@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 18:02:40 by yruda             #+#    #+#             */
/*   Updated: 2019/09/26 18:44:41 by yruda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/rt.h"

/*
**	put in the number of textures that are properly textures
*/

void	change_texture(t_fig *fig)
{
	if (fig->text_no >= 3)
		fig->text_no = -1;
	else
		fig->text_no++;
}

void	change_colours(t_fig *fig)
{
	cl_double3  colours[5];

	fig->color_index = (fig->color_index <= 4 && fig->color_index >= -1) ?
		fig->color_index + 1 : 0;
	colours[0] = (cl_double3){{255, 0, 0}};
	colours[1] = (cl_double3){{0, 255, 0}};
	colours[2] = (cl_double3){{0, 255, 0}};
	colours[3] = (cl_double3){{0, 0, 255}};
	colours[4] = (cl_double3){{255, 0, 0}};
	fig->color = colours[fig->color_index + 1];
}
