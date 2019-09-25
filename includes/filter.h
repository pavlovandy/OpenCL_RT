/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myuliia <myuliia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/23 13:04:56 by myuliia           #+#    #+#             */
/*   Updated: 2019/09/23 16:39:11 by myuliia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILTER_H
# define FILTER_H
# include "../includes/rt.h"

typedef	struct s_fil	t_fil;
typedef	struct s_color	t_color;

struct	s_color
{
	Uint32	r;
	Uint32	g;
	Uint32	b;
};

struct	s_fil
{
	t_color	rgb;
	t_color	array_filtr[10];
};

// int			get_object_intersection(t_rt *rt, int x, int y);

#endif