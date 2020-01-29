#ifndef MATRICETRIANGLE
#define MATRICETRIANGLE

#include "listIndice.h"
#include "listIndiceList.h"
#include "listPoint.h"
#include "maillage.h"


typedef struct matriceTriangle matriceTriangle;

struct matriceTriangle{
    int taille;
    listIndice* indiceList;
};

// ------ constructeur
matriceTriangle constructeurMatriceTriangle(int taille2);

int getTailleMatrice(matriceTriangle matTri);
void displayMatriceTriangle(matriceTriangle matTri);
listIndice getLigne(matriceTriangle matTri, int i);
void addPath(matriceTriangle* matTri, int indice1, int indice2);
matriceTriangle calcmatTriDelaunay(listIndiceList list, int nbProcess);

#endif
