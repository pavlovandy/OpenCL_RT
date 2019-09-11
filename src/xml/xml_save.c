/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xml_save.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozhyhadl <ozhyhadl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 13:32:14 by ozhyhadl          #+#    #+#             */
/*   Updated: 2019/09/10 19:58:05 by ozhyhadl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

const char *whitespace_cb(mxml_node_t *node, int where)
{
	const char *element;

  element = mxmlGetElement(node);

	if (!strcmp(element, "RT"))
		return ("\n");
	if (!strcmp(element, "spher"))
	{
		if (where == MXML_WS_BEFORE_OPEN)
			return ("\t");
		if (where == MXML_WS_AFTER_CLOSE)
			return ("\n");
		if (where == MXML_WS_BEFORE_CLOSE)
			return ("\n\t");
	}
	if (!strcmp(element, "centre"))
		if (where == MXML_WS_BEFORE_OPEN)
			return ("\n\t\t");

	return (NULL);
}

char	*double_to_char(double nbr)
{
	int		i;
	char	*first;
	char	*second;
	char	*tmp;
	char c;

	first = ft_itoa((int)nbr);
	tmp = first;
	first = ft_strjoin(first, ".");
	ft_strdel(&tmp);
	i = (int)nbr;
	nbr = nbr - i;
	while (nbr != 0.0)
	{

		// printf ("i = %d\n", i);
		printf("zashel nbr = %f \n", nbr);

		nbr = (nbr * 10.0);
		nbr = round(nbr);
		printf("umnozh nbr = %.20f \n", nbr);

		second = ft_itoa((int) nbr);

		tmp = first;
		first = ft_strjoin(first, second);
		ft_strdel(&tmp);
		ft_strdel(&second);
		printf("cast %lld\n", (long long)nbr);
		c = (char)((long long)nbr % 10 + '0');
		printf("char = %c\n", c);
		nbr -= (long long)nbr;
		printf("minus nbr = %f \n\n", nbr);

	}
	printf("\n suk = %s\n", first);
	return (first);
}

void	ft_write_3param(cl_double3 param, mxml_node_t *node, const char *name)
{
	char	*tmp;

	node = mxmlNewElement(node, name);
	mxmlNewText(node, 1, (tmp = double_to_char(param.s[0])));
	ft_strdel(&tmp);
	mxmlNewText(node, 1, "2" );
	mxmlNewText(node, 1, "3");

}
void	ft_write_spher(t_sphere_data spher, mxml_node_t *data)
{
	mxml_node_t *node;	/* <node> */

	node = mxmlNewElement(data, "spher");
	ft_write_3param(spher.cent, node, (const char *)"centre");

}

void	ft_write_to_xml(t_fig fig, mxml_node_t *data)
{

	if (fig.fig_type == SPHERE)
		ft_write_spher(fig.shape.sphere, data);

}

int		ft_xml_save(char *name_file, t_scene *scene, t_pov *pov)
{
	mxml_node_t *xml;    /* <?xml ... ?> */
	mxml_node_t *data;   /* <data> */
	FILE 		*fp;
	t_fig		fig;
	int			i;

	i = 0;
	xml = mxmlNewXML("1.0");
	data = mxmlNewElement(xml, "RT");
	while (i < scene->count_obj)
	{
		fig = scene->obj[i];
		ft_write_to_xml(fig, data);
		i++;
	}
	if ((fp = fopen(name_file, "w")) == NULL)
		return (error_message(RED"XML: file to save can't open"COLOR_OFF));
	mxmlSaveFile(xml, fp, whitespace_cb);
	fclose(fp);
	return (0);
}