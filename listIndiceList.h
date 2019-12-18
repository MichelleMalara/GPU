#ifndef LISTINDICELIST
#define LISTINDICELIST

#include "listPoint.h"
#include "listIndice.h"


typedef struct listIndiceList listIndiceList;

struct listIndiceList{
    int taille;
    listIndice* indiceList;
    listPoint2D listPoint;
};

// ------ constructeur
listIndiceList constructeurListIndiceList(listPoint2D list);
listIndiceList constructeurListIndiceListTaille(int taille2,listPoint2D list);
// ------ getteur
listIndice getListIndice(listIndiceList listindicelist, int i);
int getTailleListIndice(listIndiceList listindicelist);
// ------ setteur
void setListIndice(listIndiceList* Liste, listIndice newlist, int i);
// ------ fonction classe autre
void displayListIndiceList(listIndiceList liste);
void addListIndiceList(listIndiceList *listindicelist, listIndice list);
void addListIndiceListWithListIndice(listIndiceList *listindicelist, listIndiceList list);
void removeListIndice(listIndiceList *list, int i);

// ------ fonction triangulation
listIndiceList separatePointList(listPoint2D listPoint, int nbProcess);
//listIndice partition(listPoint2D pts, listIndice ptsPath, listIndiceList hulls, int num, int nbProces);
listIndiceList getAllTrianglePossible(listIndice inds, listPoint2D pts);
listIndiceList getOneTriangulation(listIndice inds, listPoint2D pts);

#endif
