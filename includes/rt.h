/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yruda <yruda@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 13:39:54 by apavlov           #+#    #+#             */
/*   Updated: 2019/09/27 17:56:16 by yruda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CL_SILENCE_DEPRECATION
#  define CL_SILENCE_DEPRECATION
#endif

#ifndef CL_USE_DEPRECATED_OPENCL_1_2_APIS
# define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif


#ifndef RT_H
# define RT_H

# include <fcntl.h>

# include "../libft/libft.h"
# ifdef __APPLE__
#  include "../frameworks/SDL2.framework/Headers/SDL.h"
#  include "../frameworks/SDL2_image.framework/Headers/SDL_image.h"
#  include "../frameworks/mxml-3.0/include/mxml.h" // Add path mxml header ++
#  include <OpenCL/opencl.h>
# else
#  include <SDL2/SDL.h>
#  include <CL/cl.h>
#  include <SDL2/SDL_image.h>
#  include <mxml.h>
# endif
# include <stdio.h>
# include "terminal_colors.h"



# define DEVICE_TYPE	CL_DEVICE_TYPE_CPU
# ifdef __APPLE__
# define WIN_WIDTH		800
# define WIN_HEIGHT		600
# else
#  define WIN_WIDTH		1200
#  define WIN_HEIGHT	800
# endif
# define MAX_OBJ_COUNT 20
# define MAX_LIGHTING_COUNT 10
# define MAX_NEGATIVE_OBJ_COUNT 5
# define MAX_CUB_COUNT 5
# define RGB(v) (((int)v[0] << 16) + ((int)v[1] << 8) + (int)v[2])
# define MIN(a,b)				(((a) < (b)) ? (a) : (b))
# define MAX(a,b)				(((a) > (b)) ? (a) : (b))
# define CLAMP(a, mi,ma)		MIN(MAX(a,mi),ma)
# define MIN_IOR 1.0004
# define MAX_IOR 2.0
# define D	0.1

# define BIG_VALUE 1000000
# define COUNT_BUTT 3
# define RENDER_ITARATION 20
# define MAX_TEXTURE_COUNT 20

typedef	struct s_fig	t_fig;
typedef	struct s_sdl	t_sdl;
typedef	struct s_scene	t_scene;
typedef	struct s_rt		t_rt;
typedef	struct s_pov	t_pov;
typedef	double	t_vector __attribute__((vector_size(sizeof(double)*4)));

enum	e_but
{
	SENT = 1, SAVE, SCREEN
};

enum	e_fig
{
	SPHERE = 1, PLANE, CONE, CYLIN, RECTANGLE, TRIANGLE, DISK, ELLIPSE, TORUS
};
enum	e_light
{
	AMBIENT = 1, DIRECT, POINT
};

typedef struct	s_sphere_data
{
	cl_double3	cent;
	cl_double	radius;
}				t_sphere_data;

typedef struct	s_plane_data
{
	cl_double3	normal;
	cl_double3	dot;
}				t_plane_data;

typedef struct	s_cone_data
{
	cl_double3	vertex;
	cl_double3	dir;
	cl_double	tangent;
	cl_double	mmax; //add to parser. inf = BIG_VALUE	---done
	cl_double	mmin; //add to parser. inf = -BIG_VALUE	---done
}				t_cone_data;

typedef struct	s_rectangle_data //add to parser	---done
{
	cl_double3	v0;
	cl_double3	v1;
	cl_double3	v2;
	cl_double3	v3;
}				t_rectangle_data;

typedef struct	s_disk_data  //add to parser	---done
{
	cl_double3	cent;
	cl_double3	normal;
	cl_double	radius;
}				t_disk_data;

typedef struct	s_torus_data 
{
	cl_double3	cent; //any
	cl_double3	dir; //unit vector
	cl_double	big_radius; // > 0
	cl_double	inner_radius; // > 0
}				t_torus_data;

typedef struct	s_ellipse_data
{
	cl_double3	cent; //any
	cl_double	dist_btwn_cent; // > 0
	cl_double3	dir; // unit vector
	cl_double	radius_sum; // > 0
}				t_ellipse_data;

typedef struct	s_triangle_data  //add to parser	---done
{
	cl_double3	v0;
	cl_double3	v1;
	cl_double3	v2;
}				t_triangle_data;

typedef struct	s_cylin_data
{
	cl_double3	dir;
	cl_double3	dot;
	cl_double	radius;
	cl_double	mmax; //add to parser. inf = BIG_VALUE	---done
	cl_double	mmin; //add to parser. inf = -BIG_VALUE	---done
}				t_cylin_data;

typedef union	u_shape
{
	t_sphere_data		sphere;
	t_cone_data			cone;
	t_plane_data		plane;
	t_cylin_data		cylin;
	t_rectangle_data	rectangle;
	t_triangle_data		triangle;
	t_torus_data		torus;
	t_ellipse_data		ellipse;
	t_disk_data			disk;
}				t_shape;

typedef struct	s_rotation_matrix
{
	cl_double3		e1;
	cl_double3		e2;
	cl_double3		e3;
}				t_rotation_matrix;

/*
**	figure's parametres:
**
**	specular - (-1) is off; [0 - 1000]
**	reflective - [0 - 1]
**	trans - transparency [0 - 1]
**	rotation - rotation of the texture
**	ior - Index of refraction [1.0004 - 2.0] or [MIN_IOR - MAX_IOR]
**	cutting - if cutting = 0, then no cutting plane
*/

typedef struct	s_cube
{
	cl_int				no;
	cl_double3			cent;
	cl_double3			rotation;
	t_rotation_matrix	rotation_matrix;
	cl_double			dist;
}				t_cube;

struct	s_fig
{
	cl_int				fig_type;
	t_shape				shape;

	cl_double3			color;
	cl_int				color_index;
	cl_int				specular;
	cl_double			reflective;
	cl_double			trans;
	cl_double3			rotation;
	t_rotation_matrix	rotation_martix;
	cl_double			ior;
	cl_int				noise;//+ -1 - 1
	cl_int				text_no;//+ -1 if off find out
	cl_int				normal_map_no;// -1 if off 6 find out
	cl_int				transparancy_map_no;//+ find out
	cl_double2			txt_offset;
	cl_double2			txt_scale;

	cl_int				cutting;
	t_plane_data		cutting_plane;  //add to parser. 	---done
	cl_int				complex_fig;  //is in the complex sturct
};

struct	s_sdl
{
	SDL_Window	*win;
	SDL_Surface	*win_sur;
};

struct	s_pov
{
	cl_double3			coord;
	cl_double3			dir;
	t_rotation_matrix	pov_rm;
	cl_double			d;
	cl_double			vh;
	cl_double			vw;
	cl_int				w;
	cl_int				h;
};

typedef struct	s_light
{
	cl_int		type_num;
	cl_double3	intensity;
	cl_double3	v;
}				t_light;

typedef struct	s_negative_fig //add to parser. currently supported sphere planes
{
	cl_int		fig_type;
	t_shape		shape;
	t_rotation_matrix	rotation_matrix; //only for cylinder
}				t_negative_fig;

struct	s_scene
{
	cl_int			count_obj;
	t_fig			obj[MAX_OBJ_COUNT];
	cl_int			count_light;
	t_light			light[MAX_LIGHTING_COUNT];
	cl_int			count_neg_obj; //default 0	---done
	t_negative_fig	neg_obj[MAX_NEGATIVE_OBJ_COUNT];
	t_cube			cubs[MAX_CUB_COUNT];
	cl_int			count_cubs;
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

	/*		kernels		*/
	
	cl_kernel			rt_kernel;
	cl_kernel			click_kernel;

	/*		memory objects		*/
	
	cl_mem				scene_mem;
	cl_mem				pixel_ptr;
	cl_mem				zbuff;
	cl_mem				texture_mem;
	cl_mem				txt_param_mem;
	cl_mem				id_obj;
	
	size_t				global_size;
	size_t				local_size;
}				t_cl;

typedef struct	s_txt_params
{
	cl_int		w;
	cl_int		h;
	cl_int		start_pos;
}				t_txt_params;

typedef struct	s_envi
{
	cl_int			txt_count; //number of textures
	cl_uint			*txt; //could be uint16 : rgb565. to save more space for kernel
	cl_int			textures_size; //the sumary size of all textures
	t_txt_params	txt_par[MAX_TEXTURE_COUNT]; //w, h and start point for each texture in txt array
}				t_envi;

typedef struct	s_edi
{
	int		chosen_obj;
}				t_edi;

typedef struct	s_button
{
	int			type;
	SDL_Surface	*image;
	SDL_Rect	offset;
	int			available;
}				t_button;

typedef struct	s_obj_movement
{
	int			move; //bool
	cl_double3	dir;
}				t_obj_movement;

typedef struct	s_filters
{
	int			motion;
	cl_double3	*colors;
	cl_uint		*buff;
	cl_float	*zbuff;
	t_obj_movement	obj_movement[MAX_OBJ_COUNT];
	int			index_filter;
}				t_filters;


struct	s_rt
{
	t_sdl		sdl;
	t_scene		scene;
	t_pov		pov;
	t_cl		cl;
	t_envi		envi;
	t_filters	filters;
	t_edi		edi;
	t_button	butt[COUNT_BUTT];
};

/*
**	Init
*/

int			init_start_params(t_rt *rt);
int			read_textures(t_rt *rt);

/*
**	CL		stuff
*/
int			init_cl(t_cl *cl);
int			create_program_and_kernels(t_cl *cl);
int			set_up_memory(t_rt *rt, t_cl *cl);
int			freed_up_memory(t_cl *cl, t_rt *rt);
int			set_global_and_local_item_size(t_cl *cl, t_rt *rt);
int			read_cl_files(char	**source_str, t_cl *cl);
int			build_program(t_cl *cl);

/*
**	SDL		stuff
*/

int			init_sdl(t_sdl *sdl, int w, int h);
int			close_sdl(t_sdl *sdl);

/*
**	Friendly user stuff
*/
int			error_message(char *mess);
int			print_usage(void);


/*
**	Render
*/
int			render_scene(t_rt *rt);

/*
**	User
*/
int			there_will_be_loop(t_rt *rt);
/*
**	Button
*/
int			init_but(t_rt *rt);
void		apply_surface(SDL_Surface *dest, t_rt *rt);
int			pres_buttn(t_rt *rt, int x, int y);
/*
**	Edit
*/

int			ft_edit(t_fig *fig, t_rt *rt, SDL_Event ev);

void		add_filter(t_rt *rt);
void		change_filters(t_rt *rt, SDL_Event ev);

# include "parse.h"
# include "mymath.h"
# include "xml.h"
# include "editor.h"
# include "functions.h"
# include "filter.h"

#endif
