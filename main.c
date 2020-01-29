#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "listPoint.h"
#include "hedge.h"
#include "listIndice.h"
#include "listIndiceList.h"
#include "maillage.h"
#include "matriceTriangle.h"

int main()

{

    printf("Hello world!\n");
    listPoint2D list = constructListPoint2DFromFile("test3");
    listIndiceList essai = separatePointList(list, 4);
    //matriceTriangle solution = calcmatTriDelaunay(essai, 4, essai);
    //displayMatriceTriangle(solution);
    /*printf("\n");
    //displayListPoint2D(list);
    printf("\n");
    listPoint2D list2 = Convex_Hull(list);
    displayListIndiceList(essai);
    printf("ouah\n\n\n");
    listPoint2D proj2 = projectionWithIndice(list, 6);
    listIndice path2 = Convex_HullIndice(proj2);
    displayListIndice(path2);
    //displayListPoint2D(proj2);*/

    /*listPoint2D proj = projectionWithIndice(list, 13);
    listPoint2D proj2 = projectionWithIndice(list, 6);
    listIndice path = Convex_HullIndice(proj);
    listIndice path2 = Convex_HullIndice(proj2);
    displayListIndice(path2);
    displayListIndice(path);
    listIndice result = getMiddleSideList(list, path2, path);
    displayListIndice(result);*/
    freeListPoint2D(&list);
    //freeListPoint2D(&list2);

    return 0;

}
