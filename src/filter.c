/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myuliia <myuliia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 17:27:18 by myuliia           #+#    #+#             */
/*   Updated: 2019/09/26 16:01:11 by myuliia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"
#include "../includes/filter.h"

Uint32		*hsv_to_rgb(Uint32 *hsv, Uint32 *rgb)
{
	float r, g, b, h, s, v;
	h = hsv[0] / 256.0;
	s = hsv[1] / 256.0;
	v = hsv[2] / 256.0;

	if(s == 0)
	{
		r = g = b = v;
		// b = v;
		// g = b;
		// r = g; //??
	}
	else
    {
		float f, p, q, t;
		int i;
		h *= 6;
		i = (int)(floor(h));
		f = h - i;
		p = v * (1 - s);
		q = v * (1 - (s * f));
		t = v * (1 - (s * (1 - f)));
		switch(i)
		{
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			case 5: r = v; g = p; b = q; break;
		}
	}
	rgb[0] = (int)(r * 255.0);
    rgb[1] = (int)(g * 255.0);
    rgb[2] = (int)(b * 255.0);
	return (rgb);
}
	
Uint32		*rgb_to_hsv(Uint32 *rgb, Uint32 *hsv)
{
	float r, g, b, h = 0, s, v;
	float min_color;

	r = rgb[0];
	g = rgb[1];
	b = rgb[2];
	v = MAX(r, MAX(g, b));
	min_color = MIN(r, MIN(g, b));
	float delta = v - min_color;
	if (v == 0.0)
		s = 0;
	else
		s = delta / v;
	if (s == 0)
		h = 0.0;
	else
	{
		if (b == v)
			h = (g - b) / delta;
		else if (g == v)
			h = 2 + (b - r) / delta;
		else if (b == v)
			h = 4 + (r - g) / delta;
		h *= 60;
		if (h < 0.0)
			h = h + 360;
	}
	
	hsv[0] = h;
	hsv[1] = s;
	hsv[2] = v / 255;
		
		
	return (hsv);
	
	/*
	r = rgb[0] / 255.0;
	g = rgb[1] / 255.0;
	b = rgb[2] / 255.0;
	max_color = MAX(r, MAX(g, b));
	min_color = MIN(r, MIN(g, b));
	v = max_color;
	if (max_color == 0.0)
		s = 0;
	else
		s = (max_color - min_color) / max_color;
	if (s == 0)
	{
		h = 0; // try change
	}
	else
	{
		if (r == max_color)
			h = (g - b) / (max_color - min_color);
		else if (g == max_color)
			h = 2.0 + (b - r) / (max_color - min_color);
		else if (b == max_color)
			h = 4.0 + (r - g) / (max_color - min_color);
		h /= 6.0;
		if (h < 0)
			h++;
	}
	hsv[0] = (int)(h * 255.0);
	hsv[1] = (int)(s * 255.0);
	hsv[2] = (int)(v * 255.0);
	return (hsv);
	*/
}

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
	// rgb[0] = 255.0 * pow(rgb[0] / 255.0, 2.2);
	// rgb[1] = 255.0 * pow(rgb[1] / 255.0, 2.2);
	// rgb[2] = 255.0 * pow(rgb[2] / 255.0, 2.2);
	f->rgb.r = (f->rgb.r * 0.393) + (f->rgb.g * 0.769) + (f->rgb.b * 0.189);
	f->rgb.g = (f->rgb.r * 0.349) + (f->rgb.g * 0.689) + (f->rgb.b * 0.168);
	f->rgb.b = (f->rgb.r * 0.272) + (f->rgb.g * 0.534) + (f->rgb.b * 0.131);
	f->rgb.r = (f->rgb.r > 255) ? 255 : f->rgb.r; 
	f->rgb.g = (f->rgb.g > 255) ? 255 : f->rgb.g; 
	f->rgb.b = (f->rgb.b > 255) ? 255 : f->rgb.b; 
}

void		swap_colors(t_fil *f)
{
	Uint32 tmp_rgb = f->rgb.r;
	f->rgb.r = f->rgb.g;
	f->rgb.g = tmp_rgb;
}

void		remove_red(t_fil *f)
{
	if (f->rgb.r > 10)
		f->rgb.g = f->rgb.r; // swap red to green
	f->rgb.r = 0;
}

void		remove_green(t_fil *f)
{
	if (f->rgb.g > 10)
		f->rgb.b = f->rgb.g;
	f->rgb.g = 0;
}

void		remove_blue(t_fil *f)
{
	if (f->rgb.b > 10)
		f->rgb.r = f->rgb.b;
	f->rgb.b = 0;
}

void		filter_red(t_fil *f)
{
	if (f->rgb.b > 10)
		f->rgb.r = f->rgb.b;
	if (f->rgb.g > 10)
		f->rgb.r = f->rgb.g;
	f->rgb.g = 0;
	f->rgb.b = 0;
}

void		filter_green(t_fil *f)
{
	if (f->rgb.b > 10)
		f->rgb.g = f->rgb.b;
	if (f->rgb.r > 10)
		f->rgb.g = f->rgb.r;
	f->rgb.r = 0;
	f->rgb.b = 0;
}

void		filter_blue(t_fil *f)
{
	if (f->rgb.r > 10)
		f->rgb.b = f->rgb.r;
	if (f->rgb.g > 10)
		f->rgb.b = f->rgb.g;
	f->rgb.r = 0;
	f->rgb.g = 0;
}

void		filter_negative(t_fil *f)
{
	f->rgb.r = 255 - f->rgb.r;
	f->rgb.g = 255 - f->rgb.g;
	f->rgb.b = 255 - f->rgb.b;
}

void		filter_greyscale(t_fil *f)
{
	f->rgb.r = (0.2126 * f->rgb.r) + (0.7152 * f->rgb.g) + (0.0722 * f->rgb.b);
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

int			parallax(const float z)
{
	const float eye_separ = 400;
	const float mu = 0.33;
	int			result;

	result = eye_separ * ((1.0 - z * mu) / (2.0 - z * mu)) + 0.5;
	return (result);
}

void		add_filter(t_rt *rt)
{
	t_fil *f;
	
	f = (t_fil *)malloc(sizeof(t_fil) * 2);
	Uint32	*pixel;
	// t_fil *f_tmp;
	Uint32	*hsv;
	t_color array_filter[80];
	int i;
	int k;
	
	i = -1;
	pixel = (Uint32 *)rt->sdl.win_sur->pixels;
	hsv = (Uint32 *)malloc(sizeof(Uint32) * 4);
	while (++i != (rt->sdl.win_sur->w * rt->sdl.win_sur->h))
	{

		int_to_rgb(f, pixel[i]);
		// rgb_tmp = int_to_rgb(rgb_tmp, pixel[i]);
		
        /* BLUR*/
		if ((i % 2 == 0) && (i < (rt->sdl.win_sur->w * rt->sdl.win_sur->h - 20)))
		{
			k = -1;
			while (++k < 10)
				f->array_filtr[k] = int_to_rgb_arr(f->array_filtr[k], pixel[i + k]);
		}
		// blur(f, f->array_filtr);
		
		// if (rt->filters.zbuff[i] < 5)
		// {
		// 	f->rgb.b = 255;
		// }
		// if (rt->filters.zbuff[i] < 10 && rt->filters.zbuff[i] > 5)
		// {
			// f->rgb.r = 0;
			// f->rgb.g = 0;
			// // f->rgb.g = (rt->filters.zbuff[i] * 10);
			// f->rgb.b = 255 - (rt->filters.zbuff[i] * 10);
			// if (f->rgb.b > 255)
			// 	f->rgb.b = 0;
			// if (f->rgb.b < 0)
			// 	f->rgb.b = 0;
			// if (f->rgb.g > 200)
			// 	f->rgb.g = 0;

		// }

		
		/***** STEREOSCOPYC ********/
		// int		kki, j, par;
		// t_vector same;
		// size_t		more[100] = 0;
		
		// if ((i % 2) != 0) {
		// f->rgb.r = (double)(rand() % 256);
		// f->rgb.g = (double)((rand()%256)*(sin(i*2*M_PI/200)+1)/2);
		// f->rgb.b = (double)(rand() % 256);// }
		
		// j = -1;
		// while (++j < rt->sdl.win_sur->h)
		// {
		// 	kki = -1;
		// 	while (++kki < rt->sdl.win_sur->w)
		// 	{
		// 		par = parallax(rt->filters.zbuff[i]);
		// 	}
		// }
		    // framebuffer[(i+j*width)*3 + 0] 
		
		/***** STEREOSCOPYC ********/

	



		
		// hsv = rgb_to_hsv(rgb, hsv);
		// swap_colors(f);
		// filter1(f);
		// remove_red(f);
		// remove_green(f);
		// remove_blue(f);
		// filter_red(f);
		// filter_green(f);
		// filter_blue(f);
		// filter_negative(f);
		// filter_greyscale(f);
		// rgb = hsv_to_rgb(hsv, rgb); /**/
		pixel[i] = ((int)f->rgb.r << 16) + ((int)f->rgb.g << 8) + (int)f->rgb.b;
		
	}
		// if (i == 234)
		// 	printf("1 RGB    %u  %u  %u\n1 HSV    %u  %u  %u\n\n", rgb[0], rgb[1], rgb[2], hsv[0], hsv[1], hsv[2]);
		// if (i == 123456)
		// 	printf("2 RGB    %u  %u  %u\n2 HSV    %u  %u  %u\n\n", rgb[0], rgb[1], rgb[2], hsv[0], hsv[1], hsv[2]);
}