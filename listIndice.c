#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "listIndice.h"
#include "listPoint.h"
#include "listIndiceList.h"

// --------- constructeur --------- //

listIndice constructeurListIndice(){
    listIndice newList;
    newList.taille = 0;
    newList.indice = NULL;
    return newList;
}

listIndice constructeurListIndiceTaille(int taille){
    listIndice newList;
    newList.taille = taille;
    newList.indice = (int*)malloc(taille*sizeof(int));
    return newList;
}

listIndice constructeurListIndiceBtw(int deb, int fin){
  // construit une liste d'indice comptenant les i in [deb, fin[
    listIndice newList;
    if(fin<deb){
      printf("le début doit etre plus petit que la fin \n");
      exit(1);
    }
    newList.taille = fin-deb;
    newList.indice = (int*) malloc((fin-deb)*sizeof(int));
    for (int i = deb; i < fin; i++) {
      newList.indice[i-deb]=i;
    }
}


// -------- getteur -------- //

int getIndice(listIndice listInd, int i){
    if(i<0 || i > listInd.taille-1){
        printf("getIndice : indice non valide");
        exit(1);
    }
  return listInd.indice[i];
}

int getTailleIndice(listIndice listeIndice){
    return listeIndice.taille;
}


// -------- setteur -------- //

void setIndice(listIndice *liste, int newIndice, int place){
    if(place >= liste->taille){
        printf("setIndice : la place voulue n'est pas valable");
        exit(1);
    }
    liste->indice[place] = newIndice;
}

// ------ fonction classe autre ------ //

void displayListIndice(listIndice liste){
    if(liste.indice == NULL){
        printf("displayListIndice : liste.indice == NULL");
        exit(1);
    }
    for(int i=0; i<liste.taille; i++){
        printf("%d ", liste.indice[i]);
    }
    printf("\n");
}

void addIndice(listIndice *listInd, int i){
    listInd->taille = listInd->taille+1;
    listInd->indice = realloc(listInd->indice, listInd->taille*sizeof(int));
    listInd->indice[listInd->taille-1] = i;
}


// ------ fonction triangulation ------ //

listIndice findPointsPathIndice(listPoint2D pts, int nbproces){
  // Trouve les points optimaux qui sépare le nuage de points selon x
  triByX(&pts); // tri selon coordoné x
  listIndice listeIndice = constructeurListIndice();
  for(int i=1 ; i<nbproces ; i++){
    int pas = floor(getTailleList2D(pts)/nbproces);
    addIndice(&listeIndice, i*pas);
  }
  return listeIndice;
}

listIndice Convex_HullIndice(listPoint2D pts){
  // Calcul de l'envelope convexe
  int n = getTailleList2D(pts);
  // Il faut plus de 3 points
  if (n < 3){
    printf("Pas assez de point \n");
    exit(1);
  }
  // trouver le point le plus gauche
  int l = 0;
  for(int i = 1; i < n; i++) {
    if(getXListPoint2D(pts,i) < getXListPoint2D(pts,l)){
      l=i;
    }
    else if(getYListPoint2D(pts,i) < getYListPoint2D(pts,l) && getXListPoint2D(pts,i) == getXListPoint2D(pts,l)){
        l=i;
    }
  }
  // init res avec le point le plus à gauche
  listIndice hull = constructeurListIndice();
  // Construction hull
  int p = l, q;
  bool flag = false;
  do{
    addIndice(&hull, p);
    q = (p + 1) % n;
    for (int x = 0; x < n; x++){
      // On cherche le point q le plus "counterclockwise"
      if (orientation(getPoint2D(pts,p), getPoint2D(pts,x), getPoint2D(pts,q))){
        q = x; // x est plus "counterclockwise" que q
      }
    }
    flag = (getXListPoint2D(pts, p) >= getXListPoint2D(pts, q));
    p = q; // à la fin c'est q le plus "counterclockwise"
  } while (p != l && !flag);  // On continu jusqu'a revenir au 1er*/
  return hull;
}


int isLeftSideList(listPoint2D listPoint, listIndice separator, int pointIndice){

    if(getYPoint2D(listPoint.point[separator.indice[0]]) >= getYPoint2D(listPoint.point[pointIndice])){
        return (getXPoint2D(listPoint.point[separator.indice[0]]) >= getXPoint2D(listPoint.point[pointIndice]));
    }
    if(getYPoint2D(listPoint.point[separator.indice[separator.taille-1]]) <= getYPoint2D(listPoint.point[pointIndice])){
        return (getXPoint2D(listPoint.point[separator.indice[separator.taille-1]]) >= getXPoint2D(listPoint.point[pointIndice]));
    }
    for(int i=0; i<separator.taille; i++){
        if(separator.indice[i]==pointIndice){
            return 1;
        }
    }
    for(int j=0; j<separator.taille-1; j++){
        if(getYPoint2D(listPoint.point[separator.indice[j]]) <= getYPoint2D(listPoint.point[pointIndice])
                && getYPoint2D(listPoint.point[separator.indice[j+1]]) > getYPoint2D(listPoint.point[pointIndice])){
            return (orientation(listPoint.point[separator.indice[j]],listPoint.point[separator.indice[j+1]],listPoint.point[pointIndice]));
        }
    }
    return 0;
}

int isRightSideList(listPoint2D listPoint, listIndice separator, int pointIndice){
    if(getYPoint2D(listPoint.point[separator.indice[0]]) >= getYPoint2D(listPoint.point[pointIndice])){
        return (getXPoint2D(listPoint.point[separator.indice[0]]) <= getXPoint2D(listPoint.point[pointIndice]));
    }
    if(getYPoint2D(listPoint.point[separator.indice[separator.taille-1]]) <= getYPoint2D(listPoint.point[pointIndice])){
        return (getXPoint2D(listPoint.point[separator.indice[separator.taille-1]]) <= getXPoint2D(listPoint.point[pointIndice]));
    }
    for(int i=0; i<separator.taille; i++){
        if(separator.indice[i]==pointIndice){
            return 1;
        }
    }
    for(int j=0; j<separator.taille-1; j++){
        if(getYPoint2D(listPoint.point[separator.indice[j]]) <= getYPoint2D(listPoint.point[pointIndice])
                && getYPoint2D(listPoint.point[separator.indice[j+1]]) > getYPoint2D(listPoint.point[pointIndice])){
            return (!orientation(listPoint.point[separator.indice[j]],listPoint.point[separator.indice[j+1]],listPoint.point[pointIndice]));
        }
    }
    return 0;
}

listIndice getLeftSideList(listPoint2D listPoint, listIndice separator){
    listIndice newList = constructeurListIndice();
    for(int i=0; i<listPoint.taille; i++){
        if(isLeftSideList(listPoint, separator, i)){
            addIndice(&newList, i);
        }
    }
    return newList;
}

listIndice getRightSideList(listPoint2D listPoint, listIndice separator){
    listIndice newList = constructeurListIndice();
    for(int i=0; i<listPoint.taille; i++){
        if(isRightSideList(listPoint, separator, i)){
            addIndice(&newList, i);
        }
    }
    return newList;
}

listIndice getMiddleSideList(listPoint2D listPoint, listIndice separatorLeft, listIndice separatorRight){
    listIndice newList = constructeurListIndice();
    for(int i=0; i<listPoint.taille; i++){
        if(isRightSideList(listPoint, separatorLeft, i) &&
                isLeftSideList(listPoint, separatorRight,i)){
            addIndice(&newList, i);
        }
    }
    return newList;
}

Point2D calcCentre(listIndice ids, listPoint2D pts){
  if(getTailleIndice(ids)!=3){
    printf("calcCentre : la liste d'indice doit être de taille 3 (triangle)\n");
    exit(1);
  }
  float a1,b1,c1,a2,b2,c2,x,y;
  Point2D p1,p2,p3,centre;
  // recupération points
  p1 = getPoint2D(pts, getIndice(ids,0));
  p2 = getPoint2D(pts, getIndice(ids,1));
  p3 = getPoint2D(pts, getIndice(ids,2));
  // calc coef
  a1 = 2*(getXPoint2D(p2)-getXPoint2D(p1));
  b1 = 2*(getYPoint2D(p2)-getYPoint2D(p1));
  c1 = pow(getXPoint2D(p1),2.)+pow(getYPoint2D(p1),2.)-pow(getXPoint2D(p2),2.)-pow(getYPoint2D(p2),2.);
  a2 = 2*(getXPoint2D(p2)-getXPoint2D(p1));
  b2 = 2*(getYPoint2D(p2)-getYPoint2D(p1));
  c2 = pow(getXPoint2D(p1),2.)+pow(getYPoint2D(p1),2.)-pow(getXPoint2D(p2),2.)-pow(getYPoint2D(p2),2.);
  // calc coordoné centre
  x = (b1*c2-b2*c1)/(a1*b2-b1*a2);
  y = (a2*c1-a1*c2)/(a1*b2-b1*a2);
  // creation point
  centre = constructPoint2D(x,y);
  return centre;
}
