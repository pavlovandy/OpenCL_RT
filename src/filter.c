/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myuliia <myuliia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 17:27:18 by myuliia           #+#    #+#             */
/*   Updated: 2019/09/27 15:26:04 by myuliia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"
#include "../includes/filter.h"

t_color		int_to_rgb_arr(t_color array_filtr, Uint32 pixel)
{
	array_filtr.r = (pixel & 0x00ff0000) >> 16;
	array_filtr.g = (pixel & 0x0000ff00) >> 8;
	array_filtr.b = (pixel & 0x000000ff);
	return (array_filtr);
}

void		int_to_rgb(t_fil *f, Uint32 pixel)
{
	f->rgb.r = (pixel & 0x00ff0000) >> 16;
	f->rgb.g = (pixel & 0x0000ff00) >> 8;
	f->rgb.b = (pixel & 0x000000ff);
}

void	filter1(t_fil *f)
{
	f->rgb.r = (f->rgb.r * 0.544) + (f->rgb.g * 0.765) + (f->rgb.b * 0.764);
	f->rgb.g = (f->rgb.r * 0.349) + (f->rgb.g * 0.689) + (f->rgb.b * 0.168);
	f->rgb.b = (f->rgb.r * 0.272) + (f->rgb.g * 0.534) + (f->rgb.b * 0.131);
	f->rgb.r = (f->rgb.r > 255) ? 255 : f->rgb.r; 
	f->rgb.g = (f->rgb.g > 255) ? 255 : f->rgb.g; 
	f->rgb.b = (f->rgb.b > 255) ? 255 : f->rgb.b; 
}

void		filter_rand_noise(t_fil *f, int i)
{
	if ((i % 2) != 0)
	{
	f->rgb.r = (double)(rand() % 256);
	f->rgb.g = (double)((rand()%256));
	f->rgb.b = (double)(rand() % 256); 
	}
}

void		filter_negative(t_fil *f)
{
	f->rgb.r = 255 - f->rgb.r;
	f->rgb.g = 255 - f->rgb.g;
	f->rgb.b = 255 - f->rgb.b;
}

void		filter_greyscale(t_fil *f)
{
	f->rgb.r = (0.2126 * f->rgb.r) + (0.7152 * f->rgb.g)
	+ (0.0722 * f->rgb.b);
	f->rgb.g = f->rgb.r;
	f->rgb.b = f->rgb.r;
}

void		blur(t_fil *f, t_color *array_filter)
{
	f->rgb.r = (array_filter[0].r + array_filter[1].r + array_filter[2].r + \
	array_filter[3].r + array_filter[4].r + array_filter[5].r + array_filter[6].r \
	+ array_filter[7].r + array_filter[8].r) / 9; 
	f->rgb.g = (array_filter[0].g + array_filter[1].g + array_filter[2].g + \
	array_filter[3].g + array_filter[4].g + array_filter[5].g + array_filter[6].g \
	+ array_filter[7].g + array_filter[8].g) / 9; 
	f->rgb.b = (array_filter[0].b + array_filter[1].b + array_filter[2].b + \
	array_filter[3].b + array_filter[4].b + array_filter[5].b + array_filter[6].b \
	+ array_filter[7].b + array_filter[8].b) / 9;
}

void		filter_blur(t_rt *rt, t_fil *f, int i, Uint32	*pixel)
{
	int		k;
	
	if ((i % 2 == 0) && (i < (rt->sdl.win_sur->w * rt->sdl.win_sur->h - 20)))
		{
			k = -1;
			while (++k < 10)
				f->array_filtr[k] =
				int_to_rgb_arr(f->array_filtr[k], pixel[i + k]);
		}
		blur(f, f->array_filtr);
}

void		add_filter(t_rt *rt)
{
	t_fil	*f;
	Uint32	*pixel;
	int		i;
	//index_filter  INITIALIZE
	
	i = -1;
	pixel = (Uint32 *)rt->sdl.win_sur->pixels;
	f = (t_fil *)malloc(sizeof(t_fil));
	while (++i != (rt->sdl.win_sur->w * rt->sdl.win_sur->h))
	{

		int_to_rgb(f, pixel[i]);
		
		if (rt->filters.index_filter == 1)
			filter1(f);
		else if (rt->filters.index_filter == 2)
			filter_greyscale(f);
		else if (rt->filters.index_filter == 3)
			filter_rand_noise(f, i);
		else if (rt->filters.index_filter == 4)
			filter_negative(f);
		else if (rt->filters.index_filter == 5)
			filter_blur(rt, f, i, pixel);
		pixel[i] = ((int)f->rgb.r << 16) + ((int)f->rgb.g << 8) + (int)f->rgb.b;
	}
}