#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "listIndice.h"
#include "listIndiceList.h"
#include "listPoint.h"

// --------- constructeur --------- //

listIndiceList constructeurListIndiceList(listPoint2D listPoint2){
    listIndiceList newList;
    newList.taille = 0;
    newList.indiceList = NULL;
    newList.listPoint = listPoint2;
    newList.separatePath = NULL;
    return newList;
}

listIndiceList constructeurListIndiceListTaille(int taille2,listPoint2D list){
    listIndiceList newList;
    newList.taille = taille2;
    if(taille2 == 0){
        newList.indiceList = NULL;
        newList.separatePath = NULL;
    }// ------ fonction triangulation ------ //
    else{
        newList.indiceList = (listIndice*) malloc(taille2*sizeof(listIndice));
        newList.separatePath = (listIndice*) malloc((taille2-1)*sizeof(listIndice));
    }
    newList.listPoint = list;
    return newList;
}

listIndiceList constructeurPossibleMaillage(listPoint2D listPoint){
    listIndiceList newPossibleMaillage;
    newPossibleMaillage.taille = listPoint.taille;
    newPossibleMaillage.indiceList = (listIndice*) malloc(listPoint.taille*sizeof(listIndice));
    newPossibleMaillage.separatePath = (listIndice*) malloc((listPoint.taille-1)*sizeof(listIndice));
    newPossibleMaillage.listPoint = listPoint;
}



// --------- getteur --------- //

listPoint2D getPoints(listIndiceList listindicelist){
  return listindicelist.listPoint;
}


listIndice getListIndice(listIndiceList listindicelist, int i){
    if(i<0 || i>listindicelist.taille-1){
        printf("getListIndice : le numero de la liste d'indice n'existe pas");
        exit(1);
    }
    else{
        return listindicelist.indiceList[i];
    }
}

int getTailleListIndice(listIndiceList listindicelist){
    return listindicelist.taille;
}


// --------- setteur --------- //

void setListIndice(listIndiceList* Liste, listIndice newlist, int i){
    if(i<0 || i>Liste->taille-1){
        printf("setListIndice : l'indice n'est pas valide");
        exit(1);
    }
    Liste->indiceList[i] = newlist;
}


// ------ fonction classe autre ------ //

void displayListIndiceList(listIndiceList liste){
    if(liste.indiceList == NULL){
        printf("displayListIndiceList : listeIndice.indice == NULL");
        exit(1);
    }
    for(int i=0; i<liste.taille; i++){
        displayListIndice(liste.indiceList[i]);
    }
    printf("\n");
}
void displayListIndiceListPath(listIndiceList liste){
    if(liste.indiceList == NULL){
        printf("displayListIndiceList : listeIndice.indice == NULL");
        exit(1);
    }
    for(int i=0; i<liste.taille; i++){
        displayListIndice(liste.separatePath[i]);
    }
    printf("\n");
}

void addListIndiceList(listIndiceList *listindicelist, listIndice list){
    listindicelist->taille++;
    listindicelist->indiceList = realloc(listindicelist->indiceList, listindicelist->taille*sizeof(listIndice));
    listindicelist->indiceList[listindicelist->taille-1] = list;
}

void addListIndiceListWithListIndice(listIndiceList *listindicelist, listIndiceList list){
    int tailleTamp = listindicelist->taille;
    listindicelist->taille = tailleTamp + list.taille;
    listindicelist->indiceList = realloc(listindicelist->indiceList, listindicelist->taille*sizeof(listIndice));
    for(int i=0; i<list.taille; i++){
        listindicelist->indiceList[tailleTamp + i] = list.indiceList[i];
    }
}

void removeListIndice(listIndiceList *list, int i){
    if(i<0 || i > list->taille -1){
        printf("removeListIndice : l'indice en entre n'est pas valable");
        exit(1);
    }
    else{
        list->taille--;
        list->indiceList[i] = getListIndice(*list, list->taille);
    }
}



// ------ fonction triangulation ------ //


listIndiceList getAllTrianglePossible(listIndice inds, listPoint2D pts){
  // crée tout les triplet possible avec les indices de la liste inds
    listIndice newTriangle;
    listIndiceList newList = constructeurListIndiceList(pts);
    int n = getTailleIndice(inds);
    for(int i=0; i<n-2; i++){
        for(int j=i+1; j<n-1; j++){
            for(int k=j+1; k<n; k++){
                newTriangle = constructeurListIndiceTaille(3);
                setIndice(&newTriangle, getIndice(inds,i), 0);
                setIndice(&newTriangle, getIndice(inds,j), 1);
                setIndice(&newTriangle, getIndice(inds,k), 2);
                addListIndiceList(&newList, newTriangle);
            }
        }
    }
    return newList;
}

listIndiceList getOneTriangulation(listIndice inds, listPoint2D pts, listIndice* leftPath, listIndice *rightPath){
  // triangulation pour un path
    // variables
    int flag;
    listIndice triangle;
    Point2D centre, pt;
    float rayon, dist;
    int pt_indice;
    listIndiceList res = constructeurListIndiceList(pts);
    // contruction de tout les triplets = triangles
    listIndiceList listTrig = getAllTrianglePossible(inds,pts);
    listIndice tampon;
    int temoinPath=0;
    int temoinPath2 = 0;
    for(int t=0 ; t<getTailleListIndice(listTrig) ; t++){
      // parcour des triangles
      flag = 1;
      triangle = getListIndice(listTrig,t);
      centre = calcCentre(triangle,pts);
      rayon = distanceBis(centre, getPoint2D(pts,getIndice(triangle,0)));
      for(int p=0 ; p<getTailleIndice(inds) ; p++){
        // parcour des points
        pt_indice = getIndice(inds,p);
        pt = getPoint2D(pts,pt_indice);
        if(pt_indice!=getIndice(triangle,0) || pt_indice!=getIndice(triangle,1) || pt_indice!=getIndice(triangle,2)){
          // point pas dans triangle
          dist = distanceBis(centre,pt);
          //displayPoint2D(centre);
          //printf("%f %f\n", dist, distanceBis(centre, pt));
          if(dist < rayon-0.0001){ // !!!! Probleme d'approximation : changer le "0.001"
            flag = 0;
            break;
          }
        }
      }
      if (flag) {
          if(leftPath==NULL && rightPath==NULL){
            addListIndiceList(&res,triangle);
          }
          else if(leftPath == NULL){
              temoinPath = isTriangleOnPath(triangle, *rightPath);
              if(!temoinPath){
                  addListIndiceList(&res,triangle);
              }
              else if(isTriangleOnPathValidRight(triangle, *rightPath, pts)){
                  addListIndiceList(&res,triangle);
              }
          }

          else if(rightPath == NULL){
              temoinPath = isTriangleOnPath(triangle, *leftPath);
              if(!temoinPath){
                  addListIndiceList(&res,triangle);
              }
              else if(isTriangleOnPathValidLeft(triangle, *leftPath, pts)){
                  addListIndiceList(&res,triangle);
              }
          }
          else{
              temoinPath = isTriangleOnPath(triangle, *rightPath);
              temoinPath2 = isTriangleOnPath(triangle, *leftPath);
              if(!temoinPath && !temoinPath2){
                  addListIndiceList(&res,triangle);
              }
              else{
                  if(temoinPath && isTriangleOnPathValidRight(triangle, *rightPath, pts)){
                      addListIndiceList(&res,triangle);
                  }
                  else if(temoinPath2 && isTriangleOnPathValidLeft(triangle, *leftPath, pts)){
                      addListIndiceList(&res,triangle);
                  }
              }
          }
      }
    }
    return res;
}

