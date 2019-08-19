/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlov <apavlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 15:02:32 by apavlov           #+#    #+#             */
/*   Updated: 2019/08/19 15:02:33 by apavlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rt.h"

int			init_cl(t_cl *cl)
{
	cl_device_fp_config	tmp;
	cl_int				ret;

	ret = clGetPlatformIDs(1, &cl->platform_id, &cl->platform_count);
	if (ret != CL_SUCCESS)
		return (error_message(RED"clGetPlatformIDs exception"COLOR_OFF));
	ret = clGetDeviceIDs(cl->platform_id, DEVICE_TYPE, 1, &cl->device_id, &cl->device_count);
	if (!cl->device_id)
		return (error_message(RED"clGetDeviceIDs problem"COLOR_OFF));
	if (ret != CL_SUCCESS)
		return (error_message(RED"clGetDeviceIDs exception"COLOR_OFF));
	clGetDeviceInfo(cl->device_id, CL_DEVICE_DOUBLE_FP_CONFIG, sizeof(tmp), &tmp, 0);
	if (!(tmp & (CL_FP_FMA | CL_FP_ROUND_TO_NEAREST | CL_FP_ROUND_TO_ZERO |
			CL_FP_ROUND_TO_INF | CL_FP_INF_NAN | CL_FP_DENORM)))
		return (error_message(RED"CL_DEVICE_DOUBLE_FP_CONFIG exception"COLOR_OFF));
	cl->context = clCreateContext( NULL, 1, &cl->device_id, NULL, NULL, &ret);
	if (ret != CL_SUCCESS)
		return (error_message(RED"clCreateContext exception"COLOR_OFF));
	cl->command_queue = clCreateCommandQueue(cl->context, cl->device_id, 0, &ret);
	if (ret != CL_SUCCESS)
		return (error_message(RED"clCreateCommandQueue exception"COLOR_OFF));
	return (0);
}

int			create_program_and_kernels(t_cl *cl)
{
	char	*source_str;
	size_t	source_size;
	cl_int	ret;

	if (read_file("src/cl/test.cl", &source_str, &source_size))
		return (error_message(RED"read_file problem"COLOR_OFF));
	cl->program = clCreateProgramWithSource(cl->context, 1, \
		(const char **)&source_str, (const size_t *)&source_size, &ret);
	free(source_str);
	if (ret != CL_SUCCESS)
		return (error_message(RED"clCreateProgramWithSource exception"COLOR_OFF));
	ret = clBuildProgram(cl->program, 1, &cl->device_id, NULL, NULL, NULL);
	if (ret != CL_SUCCESS)
		return (error_message(RED"clBuildProgram exception"COLOR_OFF));
	cl->kernel = clCreateKernel(cl->program, "test_kernel", &ret);
	if (ret != CL_SUCCESS)
		return (error_message(RED"clCreateKernel exception"COLOR_OFF));
	return (0);
}
