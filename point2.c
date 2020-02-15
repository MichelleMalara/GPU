// --------- constructeur --------- //

Point2D constructPoint2D(float x1, float y1){
    Point2D newPoint;
    newPoint.x = x1;
    newPoint.y = y1;
    return newPoint;
}


// --------- getteur --------- //

float getXPoint2D(Point2D point){
    return point.x;
}

float getYPoint2D(Point2D point){
    return point.y;
}


// --------- setteur --------- //

void setPoint2D(Point2D *point, float x2, float y2){
    point->x = x2;
    point->y = y2;
}

void setXPoint2D(Point2D *point, float x2){
    point->x = x2;
}

void setYPoint2D(Point2D *point, float y2){
    point->y = y2;
}


// ------ fonction triangulation ------ //

float sqrt_dif(Point2D p1, Point2D p2){
  // ||p1-p2||^2
  return (getXPoint2D(p1)-getXPoint2D(p2))*(getXPoint2D(p1)-getXPoint2D(p2))+(getYPoint2D(p1)-getYPoint2D(p2))*(getYPoint2D(p1)-getYPoint2D(p2));
}

int orientation(Point2D p1, Point2D p2, Point2D p3){
  // return True si (p1, p2, p3) est un triangle antihoraire
  float val1 = (getYPoint2D(p2) - getYPoint2D(p1)) * (getXPoint2D(p3) - getXPoint2D(p2));
  float val2 = (getXPoint2D(p2) - getXPoint2D(p1)) * (getYPoint2D(p3) - getYPoint2D(p2));
  return val1<val2;
}

float distanceBis(Point2D p1, Point2D p2){
  // retourne la distance euclidienne entre p1 et p2
  float dcarre = (getXPoint2D(p1)-getXPoint2D(p2))*(getXPoint2D(p1)-getXPoint2D(p2))
   + (getYPoint2D(p1)-getYPoint2D(p2))*(getYPoint2D(p1)-getYPoint2D(p2));
  return dcarre;
}

int isColineaire(Point2D p1, Point2D p2, Point2D p3, Point2D p4){
    return (getXPoint2D(p2)-getXPoint2D(p1))*(getYPoint2D(p4)-getYPoint2D(p3)) - (getYPoint2D(p2)-getYPoint2D(p1))*(getXPoint2D(p4)-getXPoint2D(p3)) == 0;
}

float getXPointProj(Point2D p1, Point2D p2){
    return p1.y - p2.y;
}

float getYPointProj(Point2D p1, Point2D p2){
    return sqrt_dif(p1,p2);
}

int orientationProj(Point2D p1, Point2D p2, Point2D p3, Point2D pProj){
  float val1 = (getYPointProj(p2,pProj) - getYPointProj(p1,pProj)) * (getXPointProj(p3, pProj) - getXPointProj(p2,pProj));
  float val2 = (getXPointProj(p2,pProj) - getXPointProj(p1,pProj)) * (getYPointProj(p3,pProj) - getYPointProj(p2, pProj));
  return val1<val2;
}


int isLeftSideListCL(Point2D *listPoint, int* separator, int pointIndice, int taillePoint, int *tailleSeparator, int nbPrc, int actualPrc){

    if(getYPoint2D(listPoint[separator[taillePoint*actualPrc]]) >= getYPoint2D(listPoint[pointIndice])){
        return (getXPoint2D(listPoint[separator[taillePoint*actualPrc]]) >= getXPoint2D(listPoint[pointIndice]));
    }
    if(getYPoint2D(listPoint[separator[tailleSeparator[actualPrc]-1]]) <= getYPoint2D(listPoint[pointIndice])){
        return (getXPoint2D(listPoint[separator[tailleSeparator[actualPrc]-1]]) >= getXPoint2D(listPoint[pointIndice]));
    }
    for(int i=0; i<tailleSeparator[actualPrc]; i++){
        if(separator[actualPrc*taillePoint+i]==pointIndice){
            return 1;
        }
    }
    for(int j=0; j<tailleSeparator[actualPrc]-1; j++){
        if(getYPoint2D(listPoint[separator[actualPrc*taillePoint+j]]) <= getYPoint2D(listPoint[pointIndice])
                && getYPoint2D(listPoint[separator[actualPrc*taillePoint+j+1]]) > getYPoint2D(listPoint[pointIndice])){
            return (orientation(listPoint[separator[actualPrc*taillePoint+j]],listPoint[separator[actualPrc*taillePoint+j+1]],listPoint[pointIndice]));
        }
    }
    return 0;
}

int isRightSideListCL(Point2D *listPoint, int* separator, int pointIndice, int taillePoint, int *tailleSeparator, int nbPrc, int actualPrc){

    if(getYPoint2D(listPoint[separator[taillePoint*actualPrc]]) >= getYPoint2D(listPoint[pointIndice])){
        return (getXPoint2D(listPoint[separator[taillePoint*actualPrc]]) <= getXPoint2D(listPoint[pointIndice]));
    }
    if(getYPoint2D(listPoint[separator[tailleSeparator[actualPrc]-1]]) <= getYPoint2D(listPoint[pointIndice])){
        return (getXPoint2D(listPoint[separator[tailleSeparator[actualPrc]-1]]) <= getXPoint2D(listPoint[pointIndice]));
    }
    for(int i=0; i<tailleSeparator[actualPrc]; i++){
        if(separator[actualPrc*taillePoint+i]==pointIndice){
            return 1;
        }
    }
    for(int j=0; j<tailleSeparator[actualPrc]-1; j++){
        if(getYPoint2D(listPoint[separator[actualPrc*taillePoint+j]]) <= getYPoint2D(listPoint[pointIndice])
                && getYPoint2D(listPoint[separator[actualPrc*taillePoint+j+1]]) > getYPoint2D(listPoint[pointIndice])){
            return (!orientation(listPoint[separator[actualPrc*taillePoint+j]],listPoint[separator[actualPrc*taillePoint+j+1]],listPoint[pointIndice]));
        }
    }
    return 0;
}

