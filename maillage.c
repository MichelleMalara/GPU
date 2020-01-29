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


// ------ fonction triangulation ------ //

maillage getTriangulation(listIndiceList paths, int nbProcess){
  // calcul de la triangulation parallélisé
  // retourner une liste de liste d'indice de taille 3
    maillage newMaillage;
    newMaillage.taille = nbProcess;
    newMaillage.listIndiceList = (listIndiceList*) malloc(nbProcess*sizeof(listIndiceList));
    newMaillage.listPoint = paths.listPoint;
    /*if(getTailleListIndice(paths) != getTailleListIndice(separatePath)-1){
        printf("getTriangulation : probleme de taille de path");
        exit(1);
    }*/

#pragma omp parallel
    {
        int th_id = omp_get_thread_num();
        listPoint2D projec;
        if(th_id < nbProcess){
            if(th_id == 0){
                setMaillage(&newMaillage, getOneTriangulation(paths.indiceList[th_id],paths.listPoint,NULL,&(paths.separatePath[0])),th_id);
            }
            else if(th_id == nbProcess -1){
                setMaillage(&newMaillage, getOneTriangulation(paths.indiceList[th_id],paths.listPoint,&(paths.separatePath[th_id-1]),NULL),th_id);
            }
            else{
                setMaillage(&newMaillage, getOneTriangulation(paths.indiceList[th_id],paths.listPoint,&(paths.separatePath[th_id-1]),&(paths.separatePath[th_id])),th_id);
            }
            //setMaillage(&newMaillage, getOneTriangulation(paths.indiceList[th_id],paths.listPoint),th_id);
        }
    }
    return newMaillage;
}
