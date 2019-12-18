#ifndef LISTPOINT
#define LISTPOINT

#include "point.h"

typedef struct listPoint2D listPoint2D;

struct listPoint2D{
    int taille;
    Point2D* point;
};

// ------ constructeur
listPoint2D constructListPoint2D(int taille2);
listPoint2D constructListPoint2DFrom2Points(Point2D p1, Point2D p2);
listPoint2D constructListPoint2DFromListPoint(listPoint2D list);
listPoint2D constructListPoint2DFromFile(char* fileName);
// ------ destructeur
void freeListPoint2D(listPoint2D* listPoint);
// ------ getteur
int getTailleList2D(listPoint2D listPoint);
Point2D getPoint2D(listPoint2D listPoint, int i);
float getXListPoint2D(listPoint2D listPoint, int i);
float getYListPoint2D(listPoint2D listPoint, int i);
// ------ setteur
void setListPoint2DToZero(listPoint2D *listPoint);
void setListPoint2D(listPoint2D *listPoint, float x2, float y2, int i);
void setListPoint2DFromPoint(listPoint2D *listPoint, Point2D point, int i);
void setXListPoint2D(listPoint2D *listPoint, float a, int i);
void setYListPoint2D(listPoint2D *listPoint, float a, int i);
// ------ fonction classe autre
void displayListPoint2D(listPoint2D listPoint);
void addPoint2D(listPoint2D *listPoint, float x2, float y2);
void addPoint2DFromPoint(listPoint2D *listPoint, Point2D point);
void addPointList2DFromPointList(listPoint2D *listPoint, listPoint2D addListPoint);

// ------ fonction triangulation
void triByX(listPoint2D *pts);
listPoint2D projection(listPoint2D listPoint, Point2D point);
listPoint2D projectionWithIndice(listPoint2D listPoint, int indice);
listPoint2D findPointsPath(listPoint2D pts, int nbproces);
listPoint2D Convex_Hull(listPoint2D pts);

#endif
