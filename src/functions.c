/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myuliia <myuliia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 23:17:16 by anri              #+#    #+#             */
/*   Updated: 2019/09/26 16:48:53 by myuliia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

cl_double3		*get_obj_dot(t_fig *fig)
{
	if (fig->fig_type == SPHERE)
		return (&fig->shape.sphere.cent);
	else if (fig->fig_type == PLANE)
		return (&fig->shape.plane.dot);
	else if (fig->fig_type == CONE)
		return (&fig->shape.cone.vertex);
	else if (fig->fig_type == CYLIN)
		return (&fig->shape.cylin.dot);
	else if (fig->fig_type == TORUS)
		return (&fig->shape.torus.cent);
	else if (fig->fig_type == ELLIPSE)
		return (&fig->shape.ellipse.cent);
	else if (fig->fig_type == TRIANGLE)
		return (&fig->shape.triangle.v0);
	else if (fig->fig_type == RECTANGLE)
		return (&fig->shape.rectangle.v0);
	else if (fig->fig_type == DISK)
		return (&fig->shape.disk.cent);
	return (0);
}
