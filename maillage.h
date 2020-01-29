#ifndef MAILLAGE
#define MAILLAGE

#include "listPoint.h"
#include "listIndice.h"
#include "listIndiceList.h"


typedef struct maillage maillage;

struct maillage{
    int taille;
    listIndiceList* listIndiceList;
    listPoint2D listPoint;
};

// ------ constructeur
maillage constructeurMaillage(listPoint2D list);
maillage constructeurMaillageTaille(int taille2,listPoint2D list);
// ------ getteur
listIndiceList getListIndiceList(maillage leMaillage, int i);
listPoint2D getPointMaillage(maillage leMaillage);
int getTailleMaillage(maillage leMaillage);
// ------ setteur
void setMaillage(maillage* Liste, listIndiceList newlist, int i);
void displayMaillage(maillage leMaillage);
// ------ fonction classe autre
/*void displayListIndiceList(listIndiceList liste);
void addListIndiceList(listIndiceList *listindicelist, listIndice list);
void addListIndiceListWithListIndice(listIndiceList *listindicelist, listIndiceList list);
void removeListIndice(listIndiceList *list, int i);*/

// ------ fonction triangulation
maillage getTriangulation(listIndiceList list, int nbProcess);

#endif
