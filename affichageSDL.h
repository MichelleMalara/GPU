#ifndef AFFICHAGESDL
#define AFFICHAGESDL

#include "listPoint.h"
#include "hedge.h"


SDL_Point* getSDLPoint(listPoint2D listPoint, float xmin, float ymin, float xmax, float ymax);
void displayListPointInterface(SDL_Renderer* ren, listPoint2D listPoint, float xmin, float ymin, float xmax, float ymax);
void displayHedgeInterface(SDL_Renderer* ren, hedge edge, float xmin, float ymin, float xmax, float ymax);

#endif


