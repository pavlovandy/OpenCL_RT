/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 16:54:35 by apavlov           #+#    #+#             */
/*   Updated: 2019/08/19 16:54:35 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

int			read_file(char *file_name, char **file_text, size_t *size)
{
	int		fd;
	char	buf[BUFFER_SIZE];
	int		ret;
	char	*tmp;

	fd = open(file_name, O_RDONLY);
	*file_text = 0;
	tmp = 0;
	size = 0;
	while ((ret = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		if (*file_text != 0)
		{
			tmp = ft_strjoin(*file_text, buf);
			free(*file_text);
		}
		else
			tmp = ft_strdup(buf);
		*file_text = tmp;
		size += ret;
	}
	return (ret < 0 ? 1 : 0);
}
