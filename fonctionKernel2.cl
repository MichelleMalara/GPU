#include "point2.h"
#include "listPoint2.h"
#include "listIndice.h"
inline int doubleMoi(int x);
inline float sqrt_diff(Point2D p1, Point2D p2);

__kernel void getPath(__global Point2D *listPoint, __global int *pointForPath, __global int *taillePoint, __global int *nbPr, __global int *paths, __global int *pathTaille)
{
    int id = get_global_id(0);

    int l = 0;
    for(int i = 1; i < taillePoint[0]; i++) {
        if(getXPointProj(listPoint[i],listPoint[pointForPath[id]]) < getXPointProj(listPoint[l],listPoint[pointForPath[id]])){
          l=i;
        }
        else if(getYPointProj(listPoint[i],listPoint[pointForPath[id]]) <getYPointProj(listPoint[l],listPoint[pointForPath[id]]) &&getXPointProj(listPoint[i],listPoint[pointForPath[id]]) == getXPointProj(listPoint[l],listPoint[pointForPath[id]])){
            l=i;
        }
    }

    int p = l, q;
    int flag = 0;
    int compteur =0;
    //paths[id]=pointForPath[id];
    do{
        paths[id*taillePoint[0]+compteur] = p;
        compteur++;
        q = (p + 1) % taillePoint[0];
        for (int x = 0; x < taillePoint[0]; x++){

          if (orientationProj(listPoint[p], listPoint[x], listPoint[q], listPoint[pointForPath[id]])){
            q = x; // x est plus "counterclockwise" que q
          }
        }
        flag = (getXPointProj(listPoint[p], listPoint[pointForPath[id]]) >= getXPointProj(listPoint[q], listPoint[pointForPath[id]]));
        p = q; // à la fin c'est q le plus "counterclockwise"
    } while (p != l && !flag);  // On continu jusqu'a revenir au 1er*/
    pathTaille[id] = compteur; 
}

__kernel void getGroup(__global Point2D *listPoint, __global int *paths,__global int *taillePath, __global int *taillePoint, __global int *nbPrc, __global int *groupPath, __global int *groupPathTaille)
{
    int id = get_global_id(0);
    int compteur = 0;

    if(id == nbPrc[0]- 1){
        //group = getRightSideList(copyList, path.indiceList[id-1]);
        int isRight = 0;
        int flag = 0;
        for(int i=0; i<taillePoint[0]; i++){
            isRight = 0;
            flag = 0;
            if(getYPoint2D(listPoint[paths[(id-1)*taillePoint[0]]]) >= getYPoint2D(listPoint[i])){
                flag = 1;
                isRight = (getXPoint2D(listPoint[paths[(id-1)*taillePoint[0]]]) <= getXPoint2D(listPoint[i]));
            }
            if(getYPoint2D(listPoint[paths[(id-1)*taillePoint[0]+taillePath[(id-1)]-1]]) <= getYPoint2D(listPoint[i])){
                flag = 1;
                isRight = (getXPoint2D(listPoint[paths[(id-1)*taillePoint[0]+taillePath[(id-1)]-1]]) <= getXPoint2D(listPoint[i]));
            }
            for(int j=0; j<taillePath[id-1]; j++){
                if(paths[(id-1)*taillePoint[0]+j]==i){
                    flag = 1;
                    isRight = 1;
                }
            }
            if(flag == 0){
                for(int j=0; j<taillePath[id-1]-1; j++){
                    if(getYPoint2D(listPoint[paths[(id-1)*taillePoint[0]+j]]) <= getYPoint2D(listPoint[i])
                            && getYPoint2D(listPoint[paths[(id-1)*taillePoint[0]+j+1]]) > getYPoint2D(listPoint[i])){
                        isRight = (!orientation(listPoint[paths[(id-1)*taillePoint[0]+j]],listPoint[paths[(id-1)*taillePoint[0]+j+1]],listPoint[i]));
                    }
                }
            }
            if(isRight){
                groupPath[id*taillePoint[0]+compteur]=i;
                compteur++;
                //addIndice(&newList, i);
            }
        }
        groupPathTaille[id]=compteur;
    }
    if(id==0){
        //group = getLeftSideList(copyList, path.indiceList[0]);


        int isLeft = 0;
        int flag = 0;
        for(int i=0; i<taillePoint[0]; i++){
            isLeft = 0;
            flag = 0;
            if(getYPoint2D(listPoint[paths[id*taillePoint[0]]]) >= getYPoint2D(listPoint[i])){
                flag = 1;
                isLeft = (getXPoint2D(listPoint[paths[id*taillePoint[0]]]) >= getXPoint2D(listPoint[i]));
            }
            if(getYPoint2D(listPoint[paths[id*taillePoint[0]+taillePath[id]-1]]) <= getYPoint2D(listPoint[i])){
                flag = 1;
                isLeft = (getXPoint2D(listPoint[paths[id*taillePoint[0]+taillePath[id]-1]]) >= getXPoint2D(listPoint[i]));
            }
            for(int j=0; j<taillePath[id]; j++){
                if(paths[id*taillePoint[0]+j]==i){
                    flag = 1;
                    isLeft = 1;
                }
            }
            if(flag == 0){
                for(int j=0; j<taillePath[id]-1; j++){
                    if(getYPoint2D(listPoint[paths[id*taillePoint[0]+j]]) <= getYPoint2D(listPoint[i])
                            && getYPoint2D(listPoint[paths[id*taillePoint[0]+j+1]]) > getYPoint2D(listPoint[i])){
                        isLeft = (orientation(listPoint[paths[id*taillePoint[0]+j]],listPoint[paths[id*taillePoint[0]+j+1]],listPoint[i]));
                    }
                }
            }
            if(isLeft){
                groupPath[id*taillePoint[0]+compteur]=i;
                compteur++;
                //addIndice(&newList, i);
            }
        }
        groupPathTaille[id]=compteur;

    }
    if(id != 0 && id < nbPrc[0]-1){
        //group = getMiddleSideList(copyList, path.indiceList[id-1], path.indiceList[id]);
        int isRight = 0;
        int isLeft = 0;
        int flag = 0;
        for(int i=0; i<taillePoint[0]; i++){
            isLeft = 0;
            flag = 0;
            if(getYPoint2D(listPoint[paths[id*taillePoint[0]]]) >= getYPoint2D(listPoint[i])){
                flag = 1;
                isLeft = (getXPoint2D(listPoint[paths[id*taillePoint[0]]]) >= getXPoint2D(listPoint[i]));
            }
            if(getYPoint2D(listPoint[paths[id*taillePoint[0]+taillePath[id]-1]]) <= getYPoint2D(listPoint[i])){
                flag = 1;
                isLeft = (getXPoint2D(listPoint[paths[id*taillePoint[0]+taillePath[id]-1]]) >= getXPoint2D(listPoint[i]));
            }
            for(int j=0; j<taillePath[id]; j++){
                if(paths[id*taillePoint[0]+j]==i){
                    flag = 1;
                    isLeft = 1;
                }
            }
            if(flag == 0){
                for(int j=0; j<taillePath[id]-1; j++){
                    if(getYPoint2D(listPoint[paths[id*taillePoint[0]+j]]) <= getYPoint2D(listPoint[i])
                            && getYPoint2D(listPoint[paths[id*taillePoint[0]+j+1]]) > getYPoint2D(listPoint[i])){
                        isLeft = (orientation(listPoint[paths[id*taillePoint[0]+j]],listPoint[paths[id*taillePoint[0]+j+1]],listPoint[i]));
                    }
                }
            }


            isRight = 0;
            flag = 0;
            if(getYPoint2D(listPoint[paths[(id-1)*taillePoint[0]]]) >= getYPoint2D(listPoint[i])){
                flag = 1;
                isRight = (getXPoint2D(listPoint[paths[(id-1)*taillePoint[0]]]) <= getXPoint2D(listPoint[i]));
            }
            if(getYPoint2D(listPoint[paths[(id-1)*taillePoint[0]+taillePath[id-1]-1]]) <= getYPoint2D(listPoint[i])){
                flag = 1;
                isRight = (getXPoint2D(listPoint[paths[(id-1)*taillePoint[0]+taillePath[id-1]-1]]) <= getXPoint2D(listPoint[i]));
            }
            for(int j=0; j<taillePath[id-1]; j++){
                if(paths[(id-1)*taillePoint[0]+j]==i){
                    flag = 1;
                    isRight = 1;
                }
            }
            if(flag == 0){
                for(int j=0; j<taillePath[id-1]-1; j++){
                    if(getYPoint2D(listPoint[paths[(id-1)*taillePoint[0]+j]]) <= getYPoint2D(listPoint[i])
                            && getYPoint2D(listPoint[paths[(id-1)*taillePoint[0]+j+1]]) > getYPoint2D(listPoint[i])){
                        isRight = (!orientation(listPoint[paths[(id-1)*taillePoint[0]+j]],listPoint[paths[(id-1)*taillePoint[0]+j+1]],listPoint[i]));
                    }
                }
            }

            if(isRight && isLeft){
                groupPath[id*taillePoint[0]+compteur]=i;
                compteur++;
            }
        }
        groupPathTaille[id]=compteur;
    }
    //setListIndice(&newListIndiceList, group, id);
    
}


__kernel void getMaillageKernel(__global Point2D *listPoint, __global int *paths,__global int *taillePath, __global int *taillePoint, __global int *nbPrc, __global int *groupPath, __global int *groupPathTaille, __global int *maillage, __global int *maillageTaille)
{
    int id = get_global_id(0);
    int compteur = 0;
    int n = groupPathTaille[id];
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int flag = 0;
    float xCentre = 0.;
    float yCentre = 0.;
    float rayon = 0.;
    float distance = 0.;
    for(int i=0; i<n-2; i++){
        for(int j=i+1; j<n-1; j++){
            for(int k=j+1; k<n; k++){

                a = groupPath[id*taillePoint[0]+i];
                b = groupPath[id*taillePoint[0]+j];
                c = groupPath[id*taillePoint[0]+k];

                calcCentreCL(&xCentre, &yCentre, listPoint[a], listPoint[b], listPoint[c]);
                rayon = distanceBisFloat(listPoint[a], xCentre, yCentre);
                flag = 1;
                for(int l=0; l<n; l++){
                    d = groupPath[id*taillePoint[0]+l];
                    if(d != a && d != b && d != c){
                         distance = distanceBisFloat(listPoint[d], xCentre, yCentre);
                         if(distance < rayon){
                             flag = 0;
                         }
                    }
                }
                if(flag){
                    if(compteur < 2*taillePoint[0]-2){
                        maillage[id*6*taillePoint[0]+3*compteur]=a;
                        maillage[id*6*taillePoint[0]+3*compteur+1]=b;
                        maillage[id*6*taillePoint[0]+3*compteur+2]=c;
                        compteur++;
                    }
                }
            }
        }
    }
    maillageTaille[id] = compteur;
}
