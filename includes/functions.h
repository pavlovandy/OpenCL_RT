/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 23:16:07 by anri              #+#    #+#             */
/*   Updated: 2019/09/27 20:21:02 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "rt.h"

cl_double3		*get_obj_dot(t_fig *fig);
cl_double3		get_obj_dir(t_fig fig);
void			ft_fill_rectangle(t_scene *scene, int cub, t_fig *fig, t_filters *filter);
cl_double3		canvas_to_viewport(int x, int y, t_pov pov);

#endif