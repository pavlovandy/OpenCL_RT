/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myuliia <myuliia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 17:27:18 by myuliia           #+#    #+#             */
/*   Updated: 2019/09/26 18:58:50 by myuliia          ###   ########.fr       */
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
	printf("%f \n", ((1.0 - z * mu) / (2.0 - z * mu)) + 0.5);
	return (result);
}

void		add_filter(t_rt *rt)
{
	t_fil *f;
	
	f = (t_fil *)malloc(sizeof(t_fil) * 2);
	Uint32	*pixel;
	// t_fil *f_tmp;
	Uint32	*hsv;
	// t_color array_filter[80];
	int i;
	// int k;
	
	i = -1;
	pixel = (Uint32 *)rt->sdl.win_sur->pixels;
	hsv = (Uint32 *)malloc(sizeof(Uint32) * 4);
	while (++i != (rt->sdl.win_sur->w * rt->sdl.win_sur->h))
	{

		int_to_rgb(f, pixel[i]);
		// rgb_tmp = int_to_rgb(rgb_tmp, pixel[i]);
		
/* BLUR*/
		// if ((i % 2 == 0) && (i < (rt->sdl.win_sur->w * rt->sdl.win_sur->h - 20)))
		// {
		// 	k = -1;
		// 	while (++k < 10)
		// 		f->array_filtr[k] = int_to_rgb_arr(f->array_filtr[k], pixel[i + k]);
		// }
		// blur(f, f->array_filtr);
/* BLUR*/
		
	
		// if (rt->filters.zbuff[i] < 10 && rt->filters.zbuff[i] > 5)
		// {
			// f->rgb.r = f->rgb.b;
			// f->rgb.g = f->rgb.b;
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
		// size_t		more[100];
		
		// //if ((i % 2) != 0) {
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
		// 		printf("par: %f\n", i+j*3);
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
/****
 <plane>
		<dot>0 0 10</dot>
		<normal>3 1 1</normal>
		<RGB>0 255 255</RGB>
		<reflective>0</reflective>
		<transparency>143567890987687654</transparency>
		<specular>1345678987654647890876543</specular>
		<texture>-0987654325467589</texture>
		<bump>-09876543345678</bump>
		<cutting>0</cutting>
		<cut_dot>0 0.3 -0.3</cut_dot>
		<cut_normal>0.7 0.7 0</cut_normal>
		<rotation>0 0 0</rotation>
		<ior>1.1</ior>
		<transp_map_no>-1</transp_map_no>
		<txt_offset>0 0</txt_offset>
		<txt_scale>1 1</txt_scale>
	</plane>
 * **/