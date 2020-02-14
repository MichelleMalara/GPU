#ifndef LISTINDICE
#define LISTINDICE

#include "listPoint.h"
#include "point.h"

typedef struct listIndice listIndice;

struct listIndice{
    int taille;
    int* indice;
};

listIndice Convex_HullIndice(listPoint2D pts);
#endif


