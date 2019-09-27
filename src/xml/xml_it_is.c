/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xml_it_is.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yruda <yruda@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 16:48:30 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/27 19:04:55 by yruda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/rt.h"

int	ft_check_child(mxml_node_t *node)
{
	if ((node = mxmlGetFirstChild(node)) != NULL)
		if (mxmlGetOpaque(mxmlGetParent(node)) != NULL)
			node = mxmlGetNextSibling(node);
	if (node != NULL)
	{
		ft_putstr(RED"XML: Mistake palace tag "COLOR_OFF);
		ft_putstr(mxmlGetElement(node));
		ft_putstr(RED" after "COLOR_OFF);
		ft_putendl(mxmlGetElement(mxmlGetParent(node)));
		return (1);
	}
	return (0);
}

int	ft_is_cam(mxml_node_t *node, t_pov *pov, int what_is)
{
	if (what_is != 3)
		return (1);
	if (ft_strequ(mxmlGetElement(node), "position"))
		return (ft_add_cam_dot(mxmlGetOpaque(node), pov));
	if (ft_strequ(mxmlGetElement(node), "dir"))
		return(ft_add_cam_dir(mxmlGetOpaque(node), pov));
	else
		return (error_message(RED"XML: invalid tag for cam"COLOR_OFF));
}

int	ft_is_light(mxml_node_t *node, t_scene *scene, int l, int what_is)
{
	const char	*name;
	cl_double3	dot;

	if (what_is != 2)
		return (1);
	name = mxmlGetElement(node);
	if (ft_strequ(name, "type"))
		return (ft_add_type_light(scene, l, (char *)mxmlGetOpaque(node)));
	else if (ft_strequ(name, "intensity") && \
			ft_get_3param(3, mxmlGetOpaque(node), &dot, NULL))
		  if (dot.s[0] >= 0 && dot.s[1] >= 0 && dot.s[2] >= 0 )
			scene->light[l].intensity = (cl_double3)dot;
		 else
			 return (error_message(RED"XML : intensity >= 0 "COLOR_OFF));
	else if (ft_strequ(name, "position") && \
		ft_get_3param(3, mxmlGetOpaque(node), &dot, NULL))
		scene->light[l].v = (cl_double3)dot;
	else
		return (error_message(RED"XML: invalid tag for light"COLOR_OFF));
	return (0);
}

int	ft_is_param2(mxml_node_t *node, t_rt *rt, int i, const char *name)
{
	t_scene	*scene;

	scene = &rt->scene;
		if (ft_strequ(name, "mmin") || ft_strequ(name, "mmax"))
			return (ft_add_mmin_mmax(mxmlGetOpaque(node), scene, i, name));
		else if (ft_strequ(name, "transparency") || ft_strequ(name, "specular") || \
		ft_strequ(name, "cut_normal") || ft_strequ(name, "texture") || \
		ft_strequ(name, "bump") || ft_strequ(name, "cutting") || \
		ft_strequ(name, "cut_dot") || ft_strequ(name, "reflective") || \
		ft_strequ(name, "rotation") || ft_strequ(name, "ior") || \
		ft_strequ(name, "transp_map_no") || ft_strequ(name, "txt_offset") || \
		ft_strequ(name, "txt_scale"))
		{
			if (scene->obj[i].fig_type == RECTANGLE && scene->obj[i].complex_fig != -1)
			{
				add_for_all_obj(mxmlGetOpaque(node), rt, i, name);
				ft_fill_rectangle(scene, scene->obj[i].complex_fig, &scene->obj[i], &rt->filters);
			}
				return (add_for_all_obj(mxmlGetOpaque(node), rt, i, name));
		}
		else if (ft_strequ(name, "move_dir"))
			return (ft_add_move_dir(mxmlGetOpaque(node), rt, i));
		else if (ft_strequ(name, "v0") || ft_strequ(name, "v1") || \
			ft_strequ(name, "v2") || ft_strequ(name, "v3"))
			return (ft_add_v(mxmlGetOpaque(node), scene, i, name));
		else if (ft_strequ(name, "distance"))
			return (ft_add_distance(mxmlGetOpaque(node), scene, i, &rt->filters));
		else if (ft_strequ(name, "cub_rotation"))
			return (ft_add_rotation(mxmlGetOpaque(node), scene, i, &rt->filters));
		return (0);
}

int	ft_is_param(mxml_node_t *node, t_rt *rt, int i, int what_is)
{
	const char	*name;
	t_scene		*scene;

	scene = &rt->scene;
	if (what_is != 1)
		return (1);
	name = mxmlGetElement(node);
	if (ft_strequ(name, "centre") || ft_strequ(name, "dot") || \
		ft_strequ(name, "vertex"))
		return (add_position(mxmlGetOpaque(node), scene, i, name));
	else if (ft_strequ(name, "RGB"))
		return (ft_add_rgb(mxmlGetOpaque(node), scene, i));
	else if (ft_strequ(name, "radius"))
		return (ft_add_radius(mxmlGetOpaque(node), scene, i));
	else if (ft_strequ(name, "normal") || ft_strequ(name, "dir"))
		return (ft_add_normal_dir(mxmlGetOpaque(node), scene, i, name));
	else if (ft_strequ(name, "angle"))
		return (ft_add_tanget(mxmlGetOpaque(node), scene, i));
	else if (ft_is_param2(node, rt, i, name))
		return(0);
	ft_putstr(RED"XML : invalid param "COLOR_OFF);
	ft_putendl(name);
	return (1);
}

int	ft_is_obj2(const char *str, t_scene *scene, int *il, t_rt *rt)
{
	if (ft_strequ(str, "spher"))
		ft_create_spher(scene, il[0], &rt->filters);
	else if (ft_strequ(str, "plane"))
		ft_create_pale(scene, il[0], &rt->filters);
	else if (ft_strequ(str, "cone"))
		ft_create_cone(scene, il[0], &rt->filters);
	else if (ft_strequ(str, "cylin"))
		ft_create_cylin(scene, il[0], &rt->filters);
	if (ft_strequ(str, "disk"))
		ft_create_disk(scene, il[0], &rt->filters);
	else if (ft_strequ(str, "triangle"))
		ft_create_triangle(scene, il[0], &rt->filters);
	else if (ft_strequ(str, "rectangle"))
		ft_create_rectangle(scene, il[0], &rt->filters);
	else if (ft_strequ(str, "ellipse"))
		ft_create_ellipse(scene, il[0], &rt->filters);
	else if (ft_strequ(str, "cube"))
		ft_create_cube(scene, &rt->filters, il);
	else
		return (0);
	return (1);
}

int	ft_is_obj(const char *str, t_scene *scene, int *il, t_rt *rt)
{
	il[0] += 1;

	if (ft_is_obj2(str, scene, il, rt))
		return (1);
	else if (ft_strequ(str, "light"))
	{
		il[0] -= 1;
		il[1] += 1;
		ft_creat_light(scene, il[1]);
		return (2);
	}
	else if (ft_strequ(str, "cam"))
	{
		il[0] -= 1;
		ft_create_cam(&rt->pov);
		return (3);
	}
	else if (ft_strequ(str, "negative_spher"))
	{
		il[0] -= 1;
		return (ft_create_negative_spher(scene, il));
	}
	else
		return (0);
	return (1);
}
