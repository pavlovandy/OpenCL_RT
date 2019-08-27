/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_event.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 14:15:40 by apavlov           #+#    #+#             */
/*   Updated: 2019/08/21 14:15:40 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

int			there_will_be_loop(t_rt *rt)
{
	SDL_Event	ev;

	while (1)
	{
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_KEYDOWN)
			{
				if (ev.key.keysym.sym == SDLK_ESCAPE)
					return (error_message(GREEN"Bye bye"COLOR_OFF) - 1);
			}
			else if (ev.type == SDL_QUIT)
				return (error_message(GREEN"Bye bye"COLOR_OFF) - 1);
		}
	}
	return (0);
}
