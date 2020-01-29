#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "listPoint.h"
#include "hedge.h"
#include "listIndice.h"
#include "listIndiceList.h"
#include "maillage.h"
#include "matriceTriangle.h"


int main()

{
    printf(" - - - - - - Triangulation de Denaulay - - - - - - \n");
    // Choix nombre de thread
    int nthreads = 4; // m = nombre de threads
    omp_set_num_threads(nthreads);
    // Definition des variable
    listPoint2D P; // Ensemble des points
    listIndiceList Q; // list de lonqueure m, Q[i]=liste indice des pts
                      // de la ième partition de l'espace
    maillage T; // list des liste des triangles de Delaunay par path
                // triangle = list de 3 indices
    hedge H; // list des aretes de maillage de Delaunay
                      // arete = list de 2 point
    // Chargement des points dans le fichier test
    P = constructListPoint2DFromFile("test3");
    printf(" - - - - - - - fichier téléchargé - - - - - - - \n");
    // Partition des points
    Q = separatePointList(P, nthreads);
    printf(" - - - - - ensemble des points partitioné - - - - - \n");
    // T = getTriangulation(Q, nthreads);
    // printf(" - - - - - triangulation calculée - - - - - \n");
    //H = calcHedgeDelaunay(Q, nthreads, Q);
    printf(" - - - - - maillage calculée - - - - - \n");
    //printf("%d arretes\n",getTailleHedge(H));
    //displayHedge(H);
  //   // Definition des variable
  //   listPoint2D P; // Ensemble des points
  //   listIndice pPath; // Indices des nthreads m-1 de séparation
  //   listPoint2D proj; // projection parabole 3D + plan orthogonal
  //   listIndiceList H; // list de lonqueure m-1, H[i]=liste des indice des points
  //                     // de la frontière convexe entre Q[i-1] et Q[i]
  //   H = constructeurListIndiceList(P);
  //   listIndiceList Q; // list de lonqueure m, Q[i]=liste indice des pts
  //                     // de la ième partition de l'espace
  //   Q = constructeurListIndiceListTaille(nthreads, P);
   //
  //   // Chargement des points dans le fichier test
  //   P = constructListPoint2DFromFile("test");
  //   // Tri les points selon leure coordonée x
  //   triByX(&P);
  //   // Trouver les points de la partition
  //   pPath = findPointsPathIndice(P,nthreads);
   //
  //   printf("calcul envelopes convexes.\n" );
  //   for(int i = 0 ; i < getTailleIndice(pPath) ; i++){
  //     // calcul des frontières pour tout i
  //      proj = projectionWithIndice(P, getIndice(pPath, i));
  //      addListIndiceList(&H, Convex_HullIndice(proj));
  //   }
   //
  //   // paralélisation de la partition
  //   listIndice part;
  //   printf("partition.\n" );
  //   #pragma omp parallel private(id,part) shared(nthreads, Q)
  //  {
  //         id = omp_get_thread_num();
  //         part = partition(P, pPath, H, id, nthreads);
  //         setListIndice(&Q, part, id);
  //  }
  //  printf("tailla Q %d \n",Q.taille);
  //  exit(1);
  //  displayListIndiceList(Q);
  //  // Triangulation
  //  printf("triangulation des partitions.\n" );
   //
  printf("    FIN      \n");
  return 0;

}
