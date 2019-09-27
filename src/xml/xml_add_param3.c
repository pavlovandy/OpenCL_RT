/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xml_add_param3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozhyhadl <ozhyhadl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 13:55:18 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/26 18:23:00 by ozhyhadl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/rt.h"

int		ft_add_v(const char *str, t_scene *scene, int i, const char *tag)
{
	cl_double3	dot;

	if (!ft_get_3param(3, str, &dot, NULL))
		return (0);
	if (ft_strequ(tag, "v0") && scene->obj[i].fig_type == TRIANGLE)
		scene->obj[i].shape.triangle.v0 = (cl_double3)dot;
	else if (ft_strequ(tag, "v1") && scene->obj[i].fig_type == TRIANGLE)
		scene->obj[i].shape.triangle.v1 = (cl_double3)dot;
	else if (ft_strequ(tag, "v2") && scene->obj[i].fig_type == TRIANGLE)
		scene->obj[i].shape.triangle.v2 = (cl_double3)dot;
	else if (ft_strequ(tag, "v0") && scene->obj[i].fig_type == RECTANGLE)
		scene->obj[i].shape.rectangle.v0 = (cl_double3)dot;
	else if (ft_strequ(tag, "v1") && scene->obj[i].fig_type == RECTANGLE)
		scene->obj[i].shape.rectangle.v1 = (cl_double3)dot;
	else if (ft_strequ(tag, "v2") && scene->obj[i].fig_type == RECTANGLE)
		scene->obj[i].shape.rectangle.v2 = (cl_double3)dot;
	else if (ft_strequ(tag, "v3") && scene->obj[i].fig_type == RECTANGLE)
		scene->obj[i].shape.rectangle.v3 = (cl_double3)dot;
	else
		return (0);
	return (1);
}

int		ft_add_distance(const char *str, t_scene *scene, int i)
{
	cl_double	one_dot;

	if (!ft_get_3param(1, str, NULL, &one_dot))
		return (0);
	if (scene->obj[i].fig_type == ELLIPSE)
		if (one_dot > 0)
			scene->obj[i].shape.ellipse.dist_btwn_cent = (cl_double)one_dot;
		else
			return(!error_message(RED"XML : distance must be >= 0"COLOR_OFF));
	else
		return (0);
	return (1);
}