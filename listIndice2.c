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

