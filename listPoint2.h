#ifndef LISTPOINT
#define LISTPOINT

#include "point.h"

typedef struct listPoint2D listPoint2D;

struct listPoint2D{
    int taille;
    Point2D* point;
};

// ------ constructeur
int getTailleList2D(listPoint2D listPoint);
float getXListPoint2D(listPoint2D listPoint, int i);
float getYListPoint2D(listPoint2D listPoint, int i);
#endif

