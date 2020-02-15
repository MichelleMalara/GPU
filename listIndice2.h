#ifndef LISTINDICE
#define LISTINDICE

#include "listPoint.h"
#include "point.h"

typedef struct listIndice listIndice;

struct listIndice{
    int taille;
    int* indice;
};

int isRightSideList(listPoint2D listPoint, listIndice separator, int pointIndice);
int isLeftSideList(listPoint2D listPoint, listIndice separator, int pointIndice);
#endif


