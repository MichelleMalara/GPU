#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "listIndice.h"
#include "listIndiceList.h"
#include "maillage.h"
#include "listPoint.h"

// --------- constructeur --------- //

maillage constructeurMaillage(listPoint2D listPoint2){
    maillage newList;
    newList.taille = 0;
    newList.listIndiceList = NULL;
    newList.listPoint = listPoint2;
    return newList;
}


maillage constructeurMaillageTaille(int taille2,listPoint2D list){
  maillage newList;
  newList.taille = taille2;
  if(taille2 == 0){
      newList.listIndiceList = NULL;
  }
  else{
    newList.listIndiceList = (listIndiceList*) malloc(taille2*sizeof(listIndiceList));
  }
  newList.listPoint = list;
  return newList;
}

maillage constructMaillageFromCL(int *maillage2, int *maillageTaille, int nbPrc, listPoint2D listPoint){
    maillage newList;
    newList.taille = nbPrc;
    newList.listPoint = listPoint;
    newList.listIndiceList = (listIndiceList*) malloc(sizeof(listIndiceList)*nbPrc);
    listIndice triangle;
    listIndiceList res;
    int a=0, b=0, c=0;
    for(int i=0; i<nbPrc; i++){
        res = constructeurListIndiceList(listPoint);
        for(int j=0; j<maillageTaille[i]; j++){
            a = maillage2[6*i*getTailleList2D(listPoint)+3*j];
            b = maillage2[6*i*getTailleList2D(listPoint)+3*j+1];
            c = maillage2[6*i*getTailleList2D(listPoint)+3*j+2];
            triangle = constructeurTriangle(a,b,c);
            addListIndiceList(&res,triangle);
            printf("Ajout triangle");
        }
        setMaillage(&newList, res, i);
    }
    return newList;
}

// --------- setteur --------- //

void setMaillage(maillage* Liste, listIndiceList newlist, int i){
    if(i<0 || i>Liste->taille-1){
        printf("setMaillage : l'indice n'est pas valide");
        exit(1);
    }
    Liste->listIndiceList[i] = newlist;
}


// --------- getteur --------- //

listIndiceList getListIndiceList(maillage leMaillage, int i){
    if(i<0 || i>leMaillage.taille-1){
        printf("getListIndiceList : le numero de la liste d'indice n'existe pas");
        exit(1);
    }
    else{
        return leMaillage.listIndiceList[i];
    }
}

void displayMaillage(maillage leMaillage){
    for(int i=0; i<getTailleMaillage(leMaillage); i++){
        displayListIndiceList(leMaillage.listIndiceList[i]);
    }
}


listPoint2D getPointMaillage(maillage leMaillage){
   return leMaillage.listPoint;
}

int getTailleMaillage(maillage leMaillage){
    return leMaillage.taille;
}
