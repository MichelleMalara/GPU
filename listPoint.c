#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "listPoint.h"

// --------- constructeur --------- //

listPoint2D constructListPoint2D(int taille2){
    listPoint2D newListPoint;
    newListPoint.taille = taille2;
    if(taille2 == 0){
        newListPoint.point = NULL;
    }
    else{
        newListPoint.point = (Point2D*) malloc(taille2*sizeof(Point2D));
    }
    return newListPoint;
}

listPoint2D constructListPoint2DFrom2Points(Point2D p1, Point2D p2){
    listPoint2D newListPoint;
    int taille2 = 2;
    newListPoint.taille = 2;
    newListPoint.point = (Point2D*) malloc(2*sizeof(Point2D));
    newListPoint.point[0]=p1;
    newListPoint.point[1]=p2;
    return newListPoint;
}

listPoint2D constructListPoint2DFromListPoint(listPoint2D list){
    listPoint2D newListPoint;
    int taille2 = list.taille;
    newListPoint.taille = taille2;
    if(taille2 == 0){
        newListPoint.point = NULL;
    }
    else{
        newListPoint.point = (Point2D*) malloc(taille2*sizeof(Point2D));
    }
    for(int i=0; i<list.taille; i++){
        newListPoint.point[i] = getPoint2D(list,i);
    }
    return newListPoint;
}

listPoint2D constructListPoint2DFromFile(char* fileName){
    FILE* file = NULL;
    file = fopen(fileName, "r");
    if (file != NULL){
        char a;
        int firstLine = 1, flag = 1, lecNb = 0;
        double valeur=0.;
        int composante = 0, compteurDec = 0, compteurPoint = 0, negativite = 1, tailleList = 0;
        int decimal = 0;
        listPoint2D newList;

        while(firstLine && flag){
            a = fgetc(file);
            if(a == 10){
                firstLine = 0;
            }
            else if(a>47 && a<58){
                tailleList = tailleList * 10 + (a-48);
            }
            else{
                printf("Le fichier du nuage de points n'est pas au bon format\n");
                exit(1);
            }
        }
        newList.point = (Point2D*) malloc(tailleList * sizeof(Point2D));
        newList.taille = tailleList;
        a = fgetc(file);
        while(a != EOF && flag){
            if(a>47 && a<58){
                lecNb = 1;
                if(decimal){
                    compteurDec = compteurDec+1;
                }
                if(compteurDec == 0){
                    valeur = valeur*10 + (a-48);
                }
                else{
                    valeur = valeur + pow(10.,-(double)compteurDec)*(a-48);
                }
            }
            else if(a == 32){
                if(lecNb){
                    if(composante == 0){
                        setXPoint2D(&newList.point[compteurPoint],negativite*valeur);
                    }
                    else{
                        printf("Il y a trop d'argument sur la ligne %d \n", compteurPoint);
                        exit(1);
                    }
                    composante = (composante + 1);
                    decimal = 0;
                    compteurDec = 0;
                    valeur = 0.;
                    negativite = 1;
                }
                lecNb = 0;
            }
            else if(a == 10){
                setYPoint2D(&newList.point[compteurPoint], negativite * valeur);
                composante = 0;
                valeur = 0.;
                decimal = 0;
                compteurDec = 0;
                compteurPoint ++;
                negativite = 1;
                lecNb = 0;
                if(compteurPoint >= newList.taille){
                    a = fgetc(file);
                    if(a != EOF){
                        printf("Le fichier est mal calibre\n");
                        exit(1);
                    }
                    flag = 0;
                }
            }
            else if(a == 46){
                decimal = 1;
            }
            else if(a == 45){
                negativite = -1;
            }
            else{
                printf("Un caractere du fichier est illisible : ");
                printf("%c\n", a);
            }
            a = fgetc(file);
        }
        if(compteurPoint != newList.taille){
            printf("Le fichier est mal calibre\n");
            exit(1);
        }
        fclose(file);
        return newList;

    }
    else{
        printf("Impossible d'ouvrir le fichier ");
        printf("%s\n", fileName);
        exit(1);
    }
}


// --------- destructeur --------- //

void freeListPoint2D(listPoint2D* listPoint){
    free(listPoint->point);
}

void displayListPoint2D(listPoint2D listPoint){
    for(int i=0; i<listPoint.taille; i++){
        displayPoint2D(listPoint.point[i]);
        printf("\n");
    }
}


// --------- getteur --------- //

int getTailleList2D(listPoint2D listPoint){
    return listPoint.taille;
}

Point2D getPoint2D(listPoint2D listPoint, int i){
    if(i >= listPoint.taille || i<0){
        printf("getPoint2D : le numero du point est invalide");
        exit(1);
    }
    return listPoint.point[i];
}

float getXListPoint2D(listPoint2D listPoint, int i){
    if(i >= listPoint.taille || i<0){
        printf("getXListPoint2D : le numero du point est invalide");
        exit(1);
    }
    return getXPoint2D(listPoint.point[i]);
}

float getYListPoint2D(listPoint2D listPoint, int i){
    if(i >= listPoint.taille || i<0){
        printf("getYListPoint2D : le numero du point est invalide");
        exit(1);
    }
    return getYPoint2D(listPoint.point[i]);
}


// --------- setteur --------- //

void setListPoint2DToZero(listPoint2D *listPoint){
    for(int i=0; i<listPoint->taille; i++){
        setPoint2D(&listPoint->point[i], 0., 0.);
    }
}

void setListPoint2D(listPoint2D *listPoint, float x2, float y2, int i){
    if(i >= listPoint->taille || i<0){
        printf("setListPoint2D : le numero du point est invalide");
        exit(1);
    }
    setPoint2D(&listPoint->point[i], x2, y2);
}

void setListPoint2DFromPoint(listPoint2D *listPoint, Point2D point, int i){
    if(i >= listPoint->taille || i<0){
        printf("setListPoint2DFromPoint : le numero du point est invalide\n");
        exit(1);
    }
    setXListPoint2D(listPoint, getXPoint2D(point), i);
    setYListPoint2D(listPoint, getYPoint2D(point), i);
}

void setXListPoint2D(listPoint2D *listPoint, float a, int i){
    if(i >= listPoint->taille || i<0){
        printf("setXListPoint2D : le numero du point est invalide\n");
        exit(1);
    }
    //setXPoint2D(&listPoint->point[i],a);
    listPoint->point[i].x = a;
}

void setYListPoint2D(listPoint2D *listPoint, float a, int i){
    if(i >= listPoint->taille || i<0){
        printf("setYListPoint2D : le numero du point est invalide\n");
        exit(1);
    }
    setYPoint2D(&listPoint->point[i],a);
}


// ------ fonction classe autre ------ //

void addPoint2D(listPoint2D *listPoint, float x2, float y2){
    listPoint->taille++;
    listPoint->point = realloc(listPoint->point, listPoint->taille*sizeof(listPoint2D));
    setListPoint2D(listPoint, x2, y2, listPoint->taille-1);
}

void addPoint2DFromPoint(listPoint2D *listPoint, Point2D point){
    listPoint->taille++;
    listPoint->point = realloc(listPoint->point, listPoint->taille*sizeof(listPoint2D));
    setListPoint2DFromPoint(listPoint, point, listPoint->taille-1);
}

void addPointList2DFromPointList(listPoint2D *listPoint, listPoint2D addListPoint){
// Ajoute la liste addListPoint à la fin de listPoint
    int taille2 = listPoint->taille;
    listPoint->taille = listPoint->taille + addListPoint.taille;
    listPoint->point = realloc(listPoint->point, listPoint->taille*sizeof(listPoint2D));
    for(int i=0; i<addListPoint.taille; i++){
        setListPoint2DFromPoint(listPoint, addListPoint.point[i], taille2+i);
    }
}


// ------ fonction triangulation ------ //

void triByX(listPoint2D *pts){
  // Tri par insertion en fonction des coordonées x des points
  int j;
  int n = getTailleList2D(*pts);
  Point2D save;
  for(int i=1 ; i<n ; i++) { // parcour de la liste
    if (getXListPoint2D(*pts,i) < getXListPoint2D(*pts,i-1)) {
      // point mal placé
      j = 0;
      while (getXListPoint2D(*pts,j) < getXListPoint2D(*pts,i)) j++;
      // j : nouvelle position du point
      save = getPoint2D(*pts, i); // on sauvegarde le point
      for(int k = i-1 ; k >= j ; k--){
        // translation des autres points
        setListPoint2DFromPoint(pts, getPoint2D(*pts,k), k+1);
      }
      setListPoint2DFromPoint(pts, save, j);
    }
  }
}

listPoint2D projection(listPoint2D listPoint, Point2D point){
  // Double projections sur parabole 3D centré en point puis sur plan orthogonal
  // au plan d'origine passant par la ligne verticale passant par point
  float py = getYPoint2D(point);
  int n = getTailleList2D(listPoint);
  listPoint2D nwList = constructListPoint2D(n);
  for(int i = 0; i<n ; i++){
    setListPoint2D(&nwList,getYListPoint2D(listPoint,i)-py, sqrt_dif(point, getPoint2D(listPoint, i)), i);
  }
  return nwList;
}

listPoint2D projectionWithIndice(listPoint2D listPoint, int indice){
  // Double projections sur parabole 3D centré en point puis sur plan orthogonal
  // au plan d'origine passant par la ligne verticale passant par point

  float py = getYPoint2D(listPoint.point[indice]);
  //printf("proj %d %f\n",indice , py);
  int n = getTailleList2D(listPoint);
  listPoint2D nwList = constructListPoint2D(n);
  for(int i = 0; i<n ; i++){
    setListPoint2D(&nwList,getYListPoint2D(listPoint,i)-py, sqrt_dif(listPoint.point[indice], getPoint2D(listPoint, i)), i);
    //printf("%d %f \n",i, getYListPoint2D(listPoint,i));
  }
  return nwList;
}

listPoint2D findPointsPath(listPoint2D pts, int nbproces){
  // Trouve les points optimaux qui sépare le nuage de points selon x
  triByX(&pts); // tri selon coordoné x
  listPoint2D pointsPath = constructListPoint2D(nbproces-1);
  for(int i=1 ; i<nbproces ; i++){
    int pas = floor(getTailleList2D(pts)/nbproces);
    setListPoint2DFromPoint(&pointsPath, getPoint2D(pts, i*pas), i-1);
  }
  return pointsPath;
}

listPoint2D Convex_Hull(listPoint2D pts){
  // Calcul de l'envelope convexe
  /* source :
  https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/
  */
  int n = getTailleList2D(pts);
  // Il faut plus de 3 points
  if (n < 3){
    printf("pas assez de point \n");
    exit(1);
  }
  // trouver le point le plus gauche
  int l = 0;
  for(int i = 1; i < n; i++) {
    if(getXListPoint2D(pts,i) < getXListPoint2D(pts,l)){
      l=i;
    }
  }
  // init res avec le point le plus à gauche
  listPoint2D hull = constructListPoint2D(0);
  // Construction hull
  int p = l, q;
  bool flag = false;
  do{
    addPoint2DFromPoint(&hull, getPoint2D(pts, p));
    q = (p + 1) % n;
    for (int x = 0; x < n; x++){
      // On cherche le point q le plus "counterclockwise"
      if (orientation(getPoint2D(pts,p), getPoint2D(pts,x), getPoint2D(pts,q))){
        q = x; // x est plus "counterclockwise" que q
      }
    }
    flag = (getXListPoint2D(pts, p) > getXListPoint2D(pts, q));
    p = q; // à la fin c'est q le plus "counterclockwise"
  } while (p != l && !flag);  // On continu jusqu'a revenir au 1er*/
  return hull;
}
