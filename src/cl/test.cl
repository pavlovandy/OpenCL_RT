#include "kernel.h"

__kernel void  test_kernel(t_scene i)
{
     t_sphere_data  data;
     int   id = get_global_id(0);
}
