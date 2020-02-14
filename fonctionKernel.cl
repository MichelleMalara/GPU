#include "point2.h"
#include "listPoint2.h"
inline int doubleMoi(int x);
__kernel void auCarre(__global Point2D *input, __global float *output)
{
int id = get_global_id(0);
output[id] = input[0].y;
}
