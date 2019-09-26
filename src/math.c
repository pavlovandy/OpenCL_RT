/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/12 13:49:51 by apavlov           #+#    #+#             */
/*   Updated: 2019/09/26 14:29:40 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

cl_double2	*reverse_matrix_2x2(cl_double2 *src)
{
	cl_double2	col1;
	cl_double2	col2;
	double		det;

	col1 = src[0];
	col2 = src[1];

	det = det_matrix_2x2(src);
	src[0].s[0] = col2.s[1] / det;
	src[1].s[0] = -col2.s[0] / det;
	src[0].s[1] = -col1.s[1] / det;
	src[1].s[1] = col1.s[0] / det;
	return (src);
}

double		det_matrix_2x2(cl_double2 *src)
{
	return (src[0].s[0] * src[1].s[1] - src[1].s[0] * src[0].s[1]);
}

cl_double3	add_double3(cl_double3 a, cl_double3 b)
{
	return ((cl_double3){{a.s[0] + b.s[0], a.s[1] + b.s[1], a.s[2] + b.s[2]}});
}

cl_double3	minus_double3(cl_double3 a, cl_double3 b)
{
	return ((cl_double3){{a.s[0] - b.s[0], a.s[1] - b.s[1], a.s[2] - b.s[2]}});
}

cl_double3	increase_double3(cl_double3 a, double multi)
{
	return ((cl_double3){{a.s[0] * multi, a.s[1] * multi, a.s[2] * multi}});
}

double		dot(cl_double3 a, cl_double3 b)
{
	return (a.s[0] * b.s[0] + a.s[1] * b.s[1] + a.s[2] * b.s[2]);
}

double		vector_len(cl_double3 a)
{
	return (sqrt(a.s[0] * a.s[0] + a.s[1] * a.s[1] + a.s[2] * a.s[2]));
}

cl_double3	ft_normalize(cl_double3 a)
{
	double	lenght;

	lenght = vector_len(a);
	return ((cl_double3){{a.s[0] / lenght, a.s[1] / lenght, a.s[2] / lenght}});
}

cl_double3		rotate_x(cl_double3 v, double angle)
{
	double y = v.s[1];
	double c = cos(angle);
	double s = sin(angle);

	v.s[1] = y * c - s * v.s[2];
	v.s[2] = y * s + c * v.s[2];
	return (v);
}

cl_double3		rotate_y(cl_double3 v, double angle)
{
	double x = v.s[0];
	double c = cos(angle);
	double s = sin(angle);

	v.s[0] = x * c + s * v.s[2];
	v.s[2] = -x * s + c * v.s[2];
	return (v);
}

cl_double3		rotate_z(cl_double3 v, double angle)
{
	double x = v.s[0];
	double c = cos(angle);
	double s = sin(angle);

	v.s[0] = x * c - s * v.s[1];
	v.s[1] = x * s + c * v.s[1];
	return (v);
}

cl_double3		rotate_xyz(cl_double3 v, cl_double3 a)
{
	return (rotate_x(rotate_y(rotate_z(v, a.s[2]), a.s[1]), a.s[0]));
}

t_rotation_matrix build_rotation_matrix_form_angles(cl_double3 rot)
{
	t_rotation_matrix	res;
	
	res.e1 = (cl_double3){{1.0, 0, 0}};
	res.e2 = (cl_double3){{0, 1.0, 0}};
	res.e3 = (cl_double3){{0, 0, 1.0}};

	res.e1 = rotate_xyz(res.e1, rot);
	res.e2 = rotate_xyz(res.e2, rot);
	res.e3 = rotate_xyz(res.e3, rot);

	res.e1 = ft_normalize(res.e1);
	res.e2 = ft_normalize(res.e2);
	res.e3 = ft_normalize(res.e3);
	return (res);
}

cl_double3		cross(cl_double3 a, cl_double3 b)
{
	return ((cl_double3){{a.s[1] * b.s[2] - a.s[2] * b.s[1], \
							a.s[2] * b.s[0] - a.s[0] * b.s[2], \
							a.s[0] * b.s[1] - a.s[1] * b.s[0]}});
}

int				check_rectangle_in_plane(t_rectangle_data rectange)
{
	cl_double	h;
	cl_double3	n;
	cl_double3	v1;
	cl_double3	v2;

	v1 = minus_double3(rectange.v1, rectange.v0);
	v2 = minus_double3(rectange.v2, rectange.v0);
	n = cross(v1, v2);
	h = -n.s[0] * v1.s[0] - n.s[1] * v1.s[1] - n.s[2] * v1.s[2];
	
	if (comp_real(dot(n, rectange.v3) + h, 0, 0.01))
		return (1);
	return (0);
}

t_rotation_matrix build_rotation_matrix_for_dir(cl_double3 dir)
{
	t_rotation_matrix	rm;

	rm.e3 = dir;
	if (rm.e3.s[2] == 0 && rm.e3.s[1] != 0)
		rm.e1 = ft_normalize((cl_double3){{1, 0, 0}});
	else if (rm.e3.s[2] == 0 && rm.e3.s[1] == 0)
		rm.e1 = ft_normalize((cl_double3){{0, 0, 1}});
	else
	{
		rm.e1 = ft_normalize((cl_double3){{1, 1, 0}});
		rm.e1.s[2] = -(rm.e3.s[0] * rm.e1.s[0] + rm.e3.s[1] * rm.e1.s[1]) / rm.e3.s[2];	
	}
	rm.e2 = cross(rm.e3, rm.e1);

	rm.e1 = ft_normalize(rm.e1);
	rm.e2 = ft_normalize(rm.e2);
	rm.e3 = ft_normalize(rm.e3);
	return (rm);
}

cl_double3	new_basis(cl_double3 point, t_rotation_matrix m)
{
	cl_double3		res;

	res.s[0] = m.e1.s[0] * point.s[0] + m.e1.s[1] * point.s[1] + m.e1.s[2] * point.s[2];
	res.s[1] = m.e2.s[0] * point.s[0] + m.e2.s[1] * point.s[1] + m.e2.s[2] * point.s[2];
	res.s[2] = m.e3.s[0] * point.s[0] + m.e3.s[1] * point.s[1] + m.e3.s[2] * point.s[2];
	return (res);
}