/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 13:39:54 by apavlov           #+#    #+#             */
/*   Updated: 2019/08/19 13:39:55 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include <fcntl.h>
# include "../libft/libft.h"
# ifdef __APPLE__
#  include "../frameworks/SDL2.framework/Headers/SDL.h"
#  include <OpenCL/opencl.h>
# else
#  include <SDL2/SDL.h>
#  include <CL/cl.h>
# endif
# include <stdio.h>
# include "terminal_colors.h"

# define WIN_WIDTH	800
# define WIN_HEIGHT	600
# define MAX_OBJ_COUNT 20
# define MAX_LIGHTING_COUNT 10
# define RGB(v) (((int)v[0] << 16) + ((int)v[1] << 8) + (int)v[2])
# define MIN(a,b)				(((a) < (b)) ? (a) : (b))
# define MAX(a,b)				(((a) > (b)) ? (a) : (b))
# define CLAMP(a, mi,ma)		MIN(MAX(a,mi),ma)

# define D	0.1
# define VW	(1.155 * D)
# define VH	(VW * WIN_HEIGHT / WIN_WIDTH)

# define BIG_VALUE 9e9

typedef	struct s_fig	t_fig;
typedef	struct s_sdl	t_sdl;
typedef	struct s_scene	t_scene;
typedef	struct s_rt		t_rt;
typedef	struct s_pov	t_pov;

typedef	double	t_vector __attribute__((vector_size(sizeof(double)*4)));

enum	e_fig
{
	SPHERE = 0, PLANE, CONE, CYLIN
};
enum	e_light
{
	AMBIENT = 0, DIRECT, POINT
};

typedef struct	s_sphere_data
{
	t_vector	cent;
	double		radius;
	t_vector	color;
	int			specular;
}				t_sphere_data;

typedef struct	s_plane_data
{
	t_vector	normal;
	t_vector	dot;
	t_vector	color;
	int			specular;
}				t_plane_data;

typedef struct	s_cone_data
{
	t_vector	vertex;
	t_vector	dir;
	double		tangent;
	t_vector	color;
	int			specular;
}				t_cone_data;

typedef struct	s_cylin_data
{
	t_vector	dir;
	t_vector	dot;
	t_vector	color;
	double		radius;
	int			specular;
}				t_cylin_data;

struct	s_fig
{
	int		fig_type;
	void	*data;
};

struct	s_sdl
{
	SDL_Window	*win;
	SDL_Surface	*win_sur;
};

struct	s_pov
{
	t_vector	coord;
	t_vector	dir;
	double		cx;
	double		cy;
	double		sx;
	double		sy;
};

typedef struct	s_light
{
	char		*type;
	int			type_num;
	double		intensity;
	t_vector	v;
}				t_light;

struct	s_scene
{
	int			count_obj;
	t_fig		obj[MAX_OBJ_COUNT];
	int			count_light;
	t_light		light[MAX_LIGHTING_COUNT];
};

struct	s_rt
{
	t_sdl	sdl;
	t_scene	scene;
	t_pov	pov;
};

#endif
