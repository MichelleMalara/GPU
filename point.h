#ifndef POINT
#define POINT

#include <stdbool.h>

typedef struct Point2D Point2D;

struct Point2D{
    float x;
    float y;
};

// ------ constructeur
Point2D constructPoint2D(float x1, float y1);
// ------ affichage
void displayPoint2D(Point2D point);
// ------ getteur
float getXPoint2D(Point2D point);
float getYPoint2D(Point2D point);
// ------ setteur
void setPoint2D(Point2D *point, float x2, float y2);
void setXPoint2D(Point2D *point, float x2);
void setYPoint2D(Point2D *point, float y2);

// ------ fonction triangulation
float sqrt_dif(Point2D p1, Point2D p2);
bool orientation(Point2D p1, Point2D p2, Point2D p3);
float distance(Point2D p1, Point2D p2);

#endif
