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

# ifndef CL_SILENCE_DEPRECATION
#  define CL_SILENCE_DEPRECATION
# endif

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

# define DEVICE_TYPE	CL_DEVICE_TYPE_GPU

# define WIN_WIDTH	800
# define WIN_HEIGHT	600
# define MAX_OBJ_COUNT 20
# define MAX_LIGHTING_COUNT 10
# define RGB(v) (((int)v[0] << 16) + ((int)v[1] << 8) + (int)v[2])
# define MIN(a,b)				(((a) < (b)) ? (a) : (b))
# define MAX(a,b)				(((a) > (b)) ? (a) : (b))
# define CLAMP(a, mi,ma)		MIN(MAX(a,mi),ma)
# define BUFFER_SIZE 200
# define D	0.1
# define VW	(1.155 * D)
# define VH	(VW * WIN_HEIGHT / WIN_WIDTH)

# define BIG_VALUE 9e9

typedef	struct s_fig	t_fig;
typedef	struct s_sdl	t_sdl;
typedef	struct s_scene	t_scene;
typedef	struct s_rt		t_rt;
typedef	struct s_pov	t_pov;

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
	cl_float3	cent;
	double		radius;
}				t_sphere_data;

typedef struct	s_plane_data
{
	cl_float3	normal;
	cl_float3	dot;
}				t_plane_data;

typedef struct	s_cone_data
{
	cl_float3	vertex;
	cl_float3	dir;
	double		tangent;
}				t_cone_data;

typedef struct	s_cylin_data
{
	cl_float3	dir;
	cl_float3	dot;
	cl_float3	color;
	double		radius;
}				t_cylin_data;

typedef union	u_shape
{
	t_sphere_data	sphere;
	t_cone_data		cone;
	t_plane_data	plane;
	t_cylin_data	cylin;
}				t_shape;

struct	s_fig
{
	int			fig_type;
	t_shape		shape;

	cl_float3	color;
	int			specular;
};

struct	s_sdl
{
	SDL_Window	*win;
	SDL_Surface	*win_sur;
};

struct	s_pov
{
	cl_float3	coord;
	cl_float3	dir;
	double		cx;
	double		cy;
	double		sx;
	double		sy;
	double		d;
	double		vh;
	double		vw;
};

typedef struct	s_light
{
	char		*type;
	int			type_num;
	double		intensity;
	cl_float3	v;
}				t_light;

struct	s_scene
{
	int			count_obj;
	t_fig		obj[MAX_OBJ_COUNT];
	int			count_light;
	t_light		light[MAX_LIGHTING_COUNT];
};

typedef struct	s_cl
{
	cl_device_id		device_id;
	cl_uint				device_count;
	cl_platform_id		platform_id;
	cl_uint				platform_count;
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			program;
	cl_kernel			kernel;

	cl_mem				scene_mem;
	cl_mem				screen_mem;
	cl_mem				pixel_ptr;

	size_t				global_size;
	size_t				local_size;
}				t_cl;

struct	s_rt
{
	t_sdl	sdl;
	t_scene	scene;
	t_pov	pov;
	t_cl	cl;
};

/*
**	CL		stuff
*/
int			init_cl(t_cl *cl);
int			create_program_and_kernels(t_cl *cl);
int			read_file(char *file_name, char **file_text); //in file parser
int			set_up_memory(t_rt rt, t_cl *cl);
int			freed_up_memory(t_cl *cl);

/*
**	SDL		stuff
*/
int			init_sdl(t_sdl *sdl);
int			close_sdl(t_sdl *sdl);

/*
**	Friendly user stuff
*/
int			error_message(char *mess);


/*
**	Render
*/
int			render_scene(t_rt *rt);

/*
**	User
*/
int			there_will_be_loop(t_rt *rt);

#endif
