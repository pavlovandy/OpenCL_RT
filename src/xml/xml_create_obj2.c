/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xml_create_obj2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozhyhadl <ozhyhadl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 19:39:38 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/24 21:40:42 by ozhyhadl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/rt.h"

void	ft_create_disk(t_scene *scene, int i, t_filters *filter)
{
	scene->obj[i].fig_type = (cl_int)DISK;
	scene->obj[i].shape.disk.cent = (cl_double3){{0, 0, 0}};
	scene->obj[i].shape.disk.normal = (cl_double3){{0, 0, 1}};
	scene->obj[i].shape.disk.radius = (cl_double)1;
	ft_create_all(scene, i, filter);
}