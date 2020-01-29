#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "listPoint.h"
#include "listIndice.h"
#include "maillage.h"
#include "hedge.h"
#include "matriceTriangle.h"

// --------- constructeur --------- //

hedge constructeurHedge(int taille2){
    if(taille2<0){
      printf("constructeurHedge : la taille doit être positive\n");
      exit(1);
    }
    else{
      hedge newList;
      newList.taille = taille2;
      if(taille2 == 0){
          newList.hedgeList = NULL;
      }
      else{
        newList.hedgeList = (listPoint2D*) malloc(taille2*sizeof(listPoint2D));
      }
      return newList;
    }
}


// --------- getteur --------- //

listPoint2D getOneHedge(hedge H, int i){
    if(i<0 || i>H.taille-1){
        printf("getHedge : le numero de l'arrete n'existe pas");
        exit(1);
    }
    else{
        return H.hedgeList[i];
    }
}

int getTailleHedge(hedge H){
  return H.taille;
}


// ------ fonction classe autre ------ //

void displayHedge(hedge H){
    if(H.hedgeList == NULL){
        printf("displayHedge : hedgeList == NULL");
        exit(1);
    }
    listPoint2D h;
    for(int i=0; i<H.taille; i++){
        h=getOneHedge(H,i);
        printf("(%f, %f) ; (%f, %f) \n",getXListPoint2D(h,0),getYListPoint2D(h,0),getXListPoint2D(h,1),getYListPoint2D(h,1));
    }
    printf("\n");
}

void addHedge(hedge *H, listPoint2D listPoint){
  if(getTailleList2D(listPoint)!=2){
    printf("addHedge : On ne peut ajouter qu'une liste de 2 point \n");
    exit(1);
  }
    H->taille++;
    H->hedgeList = realloc(H->hedgeList, H->taille*sizeof(listPoint2D));
    H->hedgeList[H->taille-1] = listPoint;
}

void addHedgeByPoints(hedge *H, Point2D p1, Point2D p2){
  listPoint2D arrete = constructListPoint2DFrom2Points(p1, p2);
  addHedge(H, arrete);
}


// ------ fonction triangulation ------ //

hedge calcHedgeDelaunay(listIndiceList list, int nbProcess){
// calcul de le arrets du maillage en combinant les triangles et les convexHull
// retourner une liste de listes de 2 points
  hedge newHedge = constructeurHedge(0);
  matriceTriangle matAdj = calcmatTriDelaunay(list, nbProcess);
  //displayMatriceTriangle(matAdj);
  listPoint2D pts = getPoints(list);

  listIndice ligne;
  for(int i=0 ; i<getTailleMatrice(matAdj) ; i++){ //chaque ligne
    ligne = getLigne(matAdj,i);
    for(int j=0 ; j<i+1 ; j++){ // chaque colonne
      if(getIndice(ligne,j)==1){
        addHedgeByPoints(&newHedge, getPoint2D(pts,i),  getPoint2D(pts,j));
      }
    }
  }
  return newHedge;
}

// int isSameHedge(hedge edge){
//     for(int i=0; i<edge.taille; i++){
//         for(int j=0; j<edge.taille; j++){
//             if(getXListPoint2D(*edge.hedgeList, i) == getXListPoint2D(*edge.hedgeList, j) &&
//                     getYListPoint2D(*edge.hedgeList, i) == getYListPoint2D(*edge.hedgeList, j)){
//                 printf("%d %d", i, j);
//                 return 1;
//             }
//         }
//     }
//     return 1;
// }

/*listPointList separatePointList(listPoint2D listPoint, int nbProcess){
    listPointList newListPointList;
    newListPointList.taille = nbProcess;
    newListPointList.pointList = (listPoint2D*) malloc(nbProcess*sizeof(listPoint2D));
    int th_id;
    triByX(&listPoint);
    listIndice pointForPath = findPointsPathIndice(listPoint, nbProcess);
#pragma omp parallel private(th_id)
    {
        th_id = omp_get_thread_num();
        if(th_id<nbProcess){
            listPoint2D projec, group;
            if(th_id == nbProcess - 1){
                projec = projectionWithIndice(listPoint,th_id-1);
            }
            else{
                projec = projectionWithIndice(listPoint,th_id);
            }
            listIndice path = Convex_HullIndice(projec);
            if(th_id == nbProcess - 1){
                group = getRightSideList(listPoint, path);
            }
            else{
                group = getLeftSideList(listPoint, path);
            }
            addListPointList(&newListPointList, group, th_id);
        }
    }
    return newListPointList;
}*/

//
// void addListPointList(listPointList *list, listPoint2D listPoint, int i){
//     if(i > list->taille-1){
//         printf("addListPointList : L'indice donné ne correspond pas à la liste");
//         exit(1);
//     }
//     else{
//         freeListPoint2D(&list->pointList[i]);
//         int taille2 = getTailleList2D(listPoint);
//         list->taille = taille2;
//         list->pointList[i] = constructListPoint2D(taille2);
//         for(int j=0; j<taille2; j++){
//             setListPoint2DFromPoint(&list->pointList[i], getPoint2D(listPoint, j), j);
//         }
//     }
// }

void addPathEdge(hedge *edge, listIndice list, listPoint2D listPoint){
    int* indice_tri = (int*)malloc(getTailleIndice(list)*sizeof(int));
    int tampon;
    for(int i=0; i<getTailleIndice(list); i++){
        indice_tri[i] = getIndice(list,i); 
    }
    for(int i=0; i<getTailleIndice(list)-1; i++){
        for(int j=0; j<getTailleIndice(list)-i-1; j++){
            if(getYListPoint2D(listPoint, indice_tri[j])>getYListPoint2D(listPoint, indice_tri[j+1])){
                tampon = indice_tri[j];
                indice_tri[j] = indice_tri[j+1];
                indice_tri[j+1] = tampon;
            }
        }
    }
    for(int i=0; i<getTailleIndice(list)-1; i++){
        addHedge(edge,constructListPoint2DFrom2Points(getPoint2D(listPoint, indice_tri[i]),getPoint2D(listPoint, indice_tri[i+1])));  
    }
}

hedge getPath(listPoint2D listPoint, int nbProcess){

    //displayListPoint2D(listPoint);

    listIndiceList newListIndiceList;
    listPoint2D copyList = constructListPoint2DFromListPoint(listPoint);
    newListIndiceList.listPoint = copyList;
    newListIndiceList.taille = nbProcess;
    newListIndiceList.indiceList = (listIndice*) malloc(nbProcess*sizeof(listIndice));
    triByX(&copyList);
    listIndice pointForPath = findPointsPathIndice(copyList, nbProcess);
    /*printf("\n");
    displayListIndice(pointForPath);
    printf("\n");*/
    listIndiceList path = constructeurListIndiceListTaille(nbProcess-1, copyList);
#pragma omp parallel
    {
        int th_id = omp_get_thread_num();
        listPoint2D projec;
        if(th_id < nbProcess-1){
            projec = projectionWithIndice(copyList,getIndice(pointForPath,th_id));
            setListIndice(&path, Convex_HullIndice(projec), th_id);
        }
    }
    hedge paths = constructeurHedge(0);
    for(int i=0; i<getTailleListIndice(path); i++){
        addPathEdge(&paths, getListIndice(path, i), copyList);
    }
    //displayHedge(paths);
    return paths;

}
