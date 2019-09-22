#include "kernel.h"

double2	intersect_triangle(double3 eye, double3 dir, t_triangle_data triangle)
{
	double3 v0v1 = triangle.v1 - triangle.v0;
	double3 v0v2 = triangle.v2 - triangle.v0;
	double3	pvec = cross(dir, v0v2);
	double	det = dot(v0v1, pvec);
	if (fabs(det) < EPSILON)
		return (BIG_VALUE);
	det = 1 / det;
	double3	tvec = eye - triangle.v0;
	double	u = dot(tvec, pvec) * det;
	if (u < 0)
		return (BIG_VALUE);
	double3 qvec = cross(tvec, v0v1); 
	double	v = dot(dir, qvec) * det;
	if (v < 0 || u + v > 1)
		return (BIG_VALUE);
	double	t = dot(v0v2, qvec) * det;
	return (t);
}

double2	intersect_rectangle(double3 eye, double3 dir, t_rectangle_data rectangle)
{
	double2	res;
	t_triangle_data data;

	data = (t_triangle_data){rectangle.v0, rectangle.v1, rectangle.v2};
	res = intersect_triangle(eye, dir, data);
	if (res[0] > 0 && res[0] < BIG_VALUE - 1)
		return (res[0]);
	data = (t_triangle_data){rectangle.v0, rectangle.v2, rectangle.v3};
	res = intersect_triangle(eye, dir, data);
	return (res[0]);
}

double2	intersect_disk(double3 eye, double3 dir, t_disk_data disk)
{
	double3	oc = eye - disk.cent;
	double	a;
	double	b;

	a = dot(oc, disk.normal);
	b = dot(dir, disk.normal);
	if (b == 0 || a * b > 0)
		return (BIG_VALUE);
	double t = -a / b;
	double3	p = dir * t + oc;
	if (dot(p, p) > disk.radius * disk.radius)
		return (-BIG_VALUE);
	return ((double2)(-a / b, -BIG_VALUE));
}

double2	intersect_sphere(double3 eye, double3 dir, t_sphere_data sphere)
{
	double3	oc = eye - sphere.cent;
	double	a;
	double	b;
	double	c;
	double	d;
	double2	roots;
	
	a = dot(dir, dir);
	b = dot(oc, dir);
	c = dot(oc, oc) - sphere.radius * sphere.radius;
	
	d = b * b - a * c;
	if (d < 0)
		return (BIG_VALUE);
	d = sqrt(d);
	roots = (double2)((-b + d) / a, (-b - d) / a);
	return (roots);
}

double2	intersect_plane(double3 eye, double3 dir, t_plane_data plane)
{
	double3	oc = eye - plane.dot;
	double	a;
	double	b;

	a = dot(oc, plane.normal);
	b = dot(dir, plane.normal);
	if (b == 0 || a * b > 0)
		return (BIG_VALUE);
	return((double2)(-a / b, -BIG_VALUE));
}

double2	intersect_cylin(double3 eye, double3 dir, t_cylin_data cylin)
{
	double3	oc = eye - cylin.dot;
	double	a;
	double	b;
	double	c;
	double	d;
	double2	roots;
	double	scalar = dot(dir, cylin.dir);
	double	scalar2 = dot(oc, cylin.dir);
	double	m;
	
	a = dot(dir, dir) - scalar * scalar;
	b = dot(dir, oc) - (scalar * scalar2);
	c = dot(oc, oc) - scalar2 * scalar2 - cylin.radius * cylin.radius;
	d = b * b - a * c;
	if (d < 0)
		return (BIG_VALUE);
	d = sqrt(d);
	roots = ((double2)((-b + d),(-b - d))) / a;
	m = roots[0] * scalar + scalar2;
	if (m < cylin.mmin || m > cylin.mmax)
		roots[0] = -BIG_VALUE;
	m = roots[1] * scalar + scalar2;
	if (m < cylin.mmin || m > cylin.mmax)
		roots[1] = -BIG_VALUE;
	return(roots);
}

double2	intersect_cone(double3 eye, double3 dir, t_cone_data cone)
{
	double3	oc = eye - cone.vertex;
	double	a;
	double	b;
	double	c;
	double	d;
	double2	roots;
	double	scalar = dot(dir, cone.dir);
	double	scalar2 = dot(oc,cone.dir);
	double	tangent2 = cone.tangent * cone.tangent;
	double	m;


	a = dot(dir, dir) - (1.0 + tangent2) * (scalar * scalar);
	b = (dot(dir, oc) - (1.0 + tangent2) * (scalar * scalar2));
	c = dot(oc, oc) - (1.0 + tangent2) * scalar2 * scalar2 ;
	d = b * b - a * c;
	if (d < 0)
		return (BIG_VALUE);
	d = sqrt(d);
	roots = ((double2)((-b + d),(-b - d)) / a);
	m = roots[0] * scalar + scalar2;
	if (m < cone.mmin || m > cone.mmax)
		roots[0] = -BIG_VALUE;
	m = roots[1] * scalar + scalar2;
	if (m < cone.mmin || m > cone.mmax)
		roots[1] = -BIG_VALUE;
	return(roots);
}

double3		calculate_normal(t_fig fig, double3 intersect_point, t_raytrace_tree curr_node)
{
	double3		tmp;//dot - intersect.point
	double3		tmp_2;
	double3		normal;

	switch (fig.fig_type)
	{
		case SPHERE:
			normal = intersect_point - fig.shape.sphere.cent;
			normal = normalize(normal);
			break ;
		case PLANE:
			normal = fig.shape.plane.normal;
			break ;
		case CYLIN:
			tmp = fig.shape.cylin.dot - intersect_point;
			normal = normalize((dot(tmp, fig.shape.cylin.dir) * fig.shape.cylin.dir) - tmp);
			break ;
		case CONE:
			tmp = fig.shape.cone.vertex - intersect_point;
			normal = normalize(((dot(tmp, tmp) / dot(tmp, fig.shape.cone.dir)) * fig.shape.cone.dir) - tmp);
			break ;
		case TRIANGLE:
			tmp = fig.shape.triangle.v0 - fig.shape.triangle.v1;
			tmp_2 = fig.shape.triangle.v0 - fig.shape.triangle.v2;
			normal = normalize(cross(tmp, tmp_2));
			break ;
		case DISK:
			normal = fig.shape.disk.normal;
			break ;
		case RECTANGLE:
			tmp = fig.shape.rectangle.v0 - fig.shape.rectangle.v1;
			tmp_2 = fig.shape.rectangle.v0 - fig.shape.rectangle.v2;
			normal = normalize(cross(tmp, tmp_2));
			break ;
	}
	if (fig.fig_type == PLANE || fig.fig_type == DISK || fig.fig_type == RECTANGLE || fig.fig_type == TRIANGLE)
		if (dot(curr_node.dir , normal) >= 0)
			normal = -normal;
	return (normal);
}

double2		cut_result(double2 prev, t_fig fig, double3 eye, double3 dir)
{
	double			h_plane;
	double			h_point;
	double3			intersect_point;
	t_plane_data	cut;

	if (fig.cutting)
	{
		cut = fig.cutting_plane;
		h_plane = -(dot(cut.normal, (cut.dot + fig.shape.sphere.cent)));
		intersect_point = eye + dir * prev[0];
		h_point = -(dot(cut.normal, intersect_point));
		if (h_plane > h_point)
			prev[0] = -BIG_VALUE;
		intersect_point = eye + dir * prev[1];
		h_point = -(dot(cut.normal, intersect_point));
		if (h_plane > h_point)
			prev[1] = -BIG_VALUE;
	}
	return (prev);
}

t_obj_and_dist		check_closest_inter(double3 eye, double3 dir, \
										__global t_scene *scene, \
										double mini, double max)
{
	double2	res;
	double	closest_dist = BIG_VALUE;
	int		closest_obj = -1;
	int		i;
	t_fig 	fig;

	i = -1;
	while (++i < scene->count_obj)
	{
		fig = scene->obj[i];
		switch (fig.fig_type)
		{
			case SPHERE:	res = intersect_sphere(eye, dir, fig.shape.sphere);			break;
			case PLANE:		res = intersect_plane(eye, dir, fig.shape.plane);			break;
			case CYLIN:		res = intersect_cylin(eye, dir, fig.shape.cylin);			break;
			case CONE:		res = intersect_cone(eye, dir, fig.shape.cone);				break;
			case TRIANGLE:	res = intersect_triangle(eye, dir, fig.shape.triangle);		break;
			case DISK:		res = intersect_disk(eye, dir, fig.shape.disk);				break;
			case RECTANGLE:	res = intersect_rectangle(eye, dir, fig.shape.rectangle);	break;
		}

		res = cut_result(res, fig, eye, dir);		
		if (res[0] > mini && res[0] < max && res[0] < closest_dist)
		{
			closest_dist = res[0];
			closest_obj = i;
		}
		if (res[1] > mini && res[1] < max && res[1] < closest_dist)
		{
			closest_dist = res[1];
			closest_obj = i;
		}
	}
	return ((t_obj_and_dist){closest_obj, closest_dist});
}
