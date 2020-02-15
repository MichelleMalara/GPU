#include "point2.h"
#include "listPoint2.h"
#include "listIndice.h"
inline int doubleMoi(int x);
inline float sqrt_diff(Point2D p1, Point2D p2);

__kernel void auCarre(__global listPoint2D *listPoint, __global listIndice *pointForPath, __global listPoint2D *output)
{
    int id = get_global_id(0);
    float py = getYPoint2D(listPoint[0].point[pointForPath[0].indice[id]]);
    int n = getTailleList2D(listPoint[0]);
    for(int i = 0; i<n ; i++){
        output[id].point[i].x = getYListPoint2D(listPoint[0],i)-py;
        output[id].point[i].y = sqrt_diff(listPoint[0].point[pointForPath[0].indice[id]], listPoint[0].point[i]);
    }
}

