int getTailleList2D(listPoint2D listPoint){
    return listPoint.taille;
}

float getXListPoint2D(listPoint2D listPoint, int i){
    if(i >= listPoint.taille || i<0){
        return 0.;
    }
    return getXPoint2D(listPoint.point[i]);
}

float getYListPoint2D(listPoint2D listPoint, int i){
    if(i >= listPoint.taille || i<0){
        return 0.;
    }
    return getYPoint2D(listPoint.point[i]);
}
