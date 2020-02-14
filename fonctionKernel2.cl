#include "point2.h"
#include "listPoint2.h"
#include "listIndice.h"
inline int doubleMoi(int x);
__kernel void auCarre(__global listPoint2D *listPoint, __global listIndice *pointForPath, __global float *output)
{
int id = get_global_id(0);
output[id] = 5.;
}

