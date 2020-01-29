#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "point.h"

// --------- constructeur --------- //

Point2D constructPoint2D(float x1, float y1){
    Point2D newPoint;
    newPoint.x = x1;
    newPoint.y = y1;
    return newPoint;
}


// --------- affichage --------- //

void displayPoint2D(Point2D point){
    printf("%f %f\n", point.x, point.y);
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
  return pow(getXPoint2D(p1)-getXPoint2D(p2),2.)+pow(getYPoint2D(p1)-getYPoint2D(p2),2.);
}

bool orientation(Point2D p1, Point2D p2, Point2D p3){
  // return True si (p1, p2, p3) est un triangle antihoraire
  float val1 = (getYPoint2D(p2) - getYPoint2D(p1)) * (getXPoint2D(p3) - getXPoint2D(p2));
  float val2 = (getXPoint2D(p2) - getXPoint2D(p1)) * (getYPoint2D(p3) - getYPoint2D(p2));
  return val1<val2;
}

float distance(Point2D p1, Point2D p2){
  // retourne la distance euclidienne entre p1 et p2
  float dcarre = (getXPoint2D(p1)-getXPoint2D(p2))*(getXPoint2D(p1)-getXPoint2D(p2))
   + (getYPoint2D(p1)-getYPoint2D(p2))*(getYPoint2D(p1)-getYPoint2D(p2));
  return dcarre;
}

bool isColineaire(Point2D p1, Point2D p2, Point2D p3, Point2D p4){
    return (getXPoint2D(p2)-getXPoint2D(p1))*(getYPoint2D(p4)-getYPoint2D(p3)) - (getYPoint2D(p2)-getYPoint2D(p1))*(getXPoint2D(p4)-getXPoint2D(p3)) == 0;
}
