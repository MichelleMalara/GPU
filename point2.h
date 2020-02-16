#ifndef POINT
#define POINT

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
int orientation(Point2D p1, Point2D p2, Point2D p3);
float distanceBis(Point2D p1, Point2D p2);
int isColineaire(Point2D p1, Point2D p2, Point2D p3, Point2D p4);

float getXPointProj(Point2D p1, Point2D p2);
float getYPointProj(Point2D p1, Point2D p2);
int orientationProj(Point2D p1, Point2D p2, Point2D p3, Point2D pProj);
int isRightSideListCL(Point2D *listPoint, int* separator, int pointIndice, int taillePoint, int *tailleSeparator, int nbPrc, int actualPrc);
int isLeftSideListCL(Point2D *listPoint, int* separator, int pointIndice, int taillePoint, int *tailleSeparator, int nbPrc, int actualPrc);
void calcCentreCL(float *x, float *y, Point2D p1, Point2D p2, Point2D p3);
float distanceBisFloat(Point2D p, float x, float y);
#endif
