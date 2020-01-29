#ifndef LISTINDICE
#define LISTINDICE

#include "listPoint.h"
#include "point.h"

typedef struct listIndice listIndice;

struct listIndice{
    int taille;
    int* indice;
};

// ------ constructeur
listIndice constructeurListIndice();
listIndice constructeurListIndiceTaille(int taille);
listIndice constructeurListIndiceBtw(int deb, int fin);
// ------ getteur
int getIndice(listIndice listInd, int i);
int getTailleIndice(listIndice listeIndice);
//------- Setteur
void setIndice(listIndice *liste, int newIndice, int place);
// ------ fonction classe autre
void addIndice(listIndice *listInd, int i);
void displayListIndice(listIndice liste);

// ------ fonction triangulation
listIndice findPointsPathIndice(listPoint2D pts, int nbproces);
listIndice Convex_HullIndice(listPoint2D pts);
int isLeftSideList(listPoint2D listPoint, listIndice separator, int pointIndice);
int isRightSideList(listPoint2D listPoint, listIndice separator, int pointIndice);
listIndice getLeftSideList(listPoint2D listPoint, listIndice separator);
listIndice getRightSideList(listPoint2D listPoint, listIndice separator);
listIndice getMiddleSideList(listPoint2D listPoint, listIndice separatorLeft, listIndice separatorRight);
// calcul du cercle
Point2D calcCentre(listIndice ids, listPoint2D pts);
int isTriangleOnPath(listIndice triangle, listIndice path);
int isAlignedOnPath(listIndice triangle, listIndice path);
int isTriangleOnPathValidRight(listIndice triangle, listIndice path, listPoint2D listPoint);
int isTriangleOnPathValidLeft(listIndice triangle, listIndice path, listPoint2D listPoint);
void getTriangleLeftEdge(listIndice* newTriangle, listIndice triangle, listPoint2D listPoint);
void getTriangleRightEdge(listIndice* newTriangle, listIndice triangle, listPoint2D listPoint);
#endif

