#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "listIndice.h"
#include "listIndiceList.h"
#include "listPoint.h"
#include "matriceTriangle.h"
#include "maillage.h"

matriceTriangle constructeurMatriceTriangle(int taille2){
    matriceTriangle newMatrice;
    newMatrice.taille = taille2;
    newMatrice.indiceList = (listIndice*) malloc(taille2*sizeof(listIndice));
    for(int i=0; i<taille2; i++){
        newMatrice.indiceList[i] = constructeurListIndiceTaille(i+1);
        for(int j=0; j<i+1; j++){
            setIndice(&newMatrice.indiceList[i], 0, j);
        }
    }
    return newMatrice;
}

int getTailleMatrice(matriceTriangle matTri){
  return matTri.taille;
}


listIndice getLigne(matriceTriangle matTri, int i){
  if(i<0 || i>=matTri.taille){
    printf("getLigne : indice invalide");
    exit(1);
  }
  return matTri.indiceList[i];
}

void addPath(matriceTriangle* matTri, int indice1, int indice2){
    if(indice1 <0 || indice1 > matTri->taille-1 || indice2<0 || indice2 > matTri->taille-1){
        printf("addPath : Ce chemin n'existe pas");
        exit(1);
    }
    else{
        int minIndice, maxIndice;
        if(indice1 < indice2){
            minIndice = indice1;
            maxIndice = indice2;
        }
        else{
            minIndice = indice2;
            maxIndice = indice1;
        }
        setIndice(&matTri->indiceList[maxIndice], 1, minIndice);
    }
}

void displayMatriceTriangle(matriceTriangle matTri){
    for(int i=0; i<matTri.taille; i++){
        displayListIndice(matTri.indiceList[i]);
    }
}

matriceTriangle calcmatTriDelaunay(listIndiceList list, int nbProcess){
  // calcul la matrice d'adgacence
    maillage allTriangles = getTriangulation(list,nbProcess);
    listPoint2D pts = getPointMaillage(allTriangles);
    matriceTriangle newMatTri = constructeurMatriceTriangle(getTailleList2D(pts));
    // listIndiceList test = constructeurListIndiceList(pts);
    listIndiceList pathTriangles;
    listIndice triangle;
    int a, b, c;
    //printf("%d \n",getTailleMatrice(newMatTri));
    for(int j=0 ; j<getTailleMaillage(allTriangles) ; j++){ // chaque path
      pathTriangles = getListIndiceList(allTriangles,j);
      for(int i=0 ; i<getTailleListIndice(pathTriangles) ; i++){ // chaque triangle
        triangle = getListIndice(pathTriangles,i);
        a = getIndice(triangle,0);
        b = getIndice(triangle,1);
        c = getIndice(triangle,2);
        //printf("%d, %d, %d\n",a,b,c );
        if(a>=0 && c>=0){
            addPath(&newMatTri, a, b);
            addPath(&newMatTri, a, c);
            addPath(&newMatTri, c, b);
        }
        else{
            addPath(&newMatTri, -a, b);
            addPath(&newMatTri, b, -c);
        }
      }
    }
    return newMatTri;
}
