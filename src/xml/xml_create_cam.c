/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xml_create_cam.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myuliia <myuliia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 12:22:48 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/27 21:42:47 by myuliia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/rt.h"

void	ft_add_w_h(t_pov *pov, mxml_node_t *rt)
{
	double	w;
	double	h;

	w = -1;
	h = -1;
	pov->w = WIN_WIDTH;
	pov->h = WIN_HEIGHT;
	if (mxmlElementGetAttr(rt, "width") != NULL)
		if (ft_get_3param(1, mxmlElementGetAttr(rt, "width"), NULL, &w))
		{
			if (w > 0 && w < 3000)
				pov->w = w;
			else
				error_message(RED"0 < width < 3000:now default"COLOR_OFF);
		}
	if (mxmlElementGetAttr(rt, "heigth") != NULL)
		if (ft_get_3param(1, mxmlElementGetAttr(rt, "heigth"), NULL, &h))
		{	
			if (h > 0 && h < 3000)
				pov->h = h;
			else
				error_message(RED"0 < height < 3000:now default"COLOR_OFF);
		}
}

void	ft_create_cam(t_pov *pov)
{
	pov->d = D;
	pov->vw = 1.155 * D;
	pov->vh = pov->vw * pov->h / pov->w;
	pov->coord = (cl_double3){{0, 0, 0}};
	pov->dir = (cl_double3){{0, 0, 0}};
}

int		ft_add_cam_dot(const char *str, t_pov *pov)
{
	cl_double3 dot;

	if (ft_get_3param(3, str, &dot, NULL))
		pov->coord = (cl_double3)dot;
	else
		return (1);
	return (0);
}

int		ft_add_cam_dir(const char *str, t_pov *pov)
{
	cl_double3 dot;

	if (ft_get_3param(3, str, &dot, NULL))
		pov->pov_rm = \
		build_rotation_matrix_form_angles((pov->dir = (cl_double3)dot));
	else
		return (1);
	return (0);
}
