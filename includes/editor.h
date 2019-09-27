/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yruda <yruda@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 16:59:59 by apavlov           #+#    #+#             */
/*   Updated: 2019/09/26 18:32:48 by yruda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H
# include "../includes/rt.h"

int			get_object_intersection(t_rt *rt, int x, int y);
void		change_texture(t_fig *fig);
void		change_colours(t_fig *fig);

cl_double3	normal_direction_rotation(const Uint8 *k_s, cl_double3 n, double angle, t_fig *fig);
cl_double3	vector_rotation(const Uint8 *k_s, cl_double3 n, double angle);

#endif