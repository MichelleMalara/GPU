#include <SDL2/SDL.h>
#include <stdio.h>
#include "listPoint.h"
#include "hedge.h"
#include <math.h>


SDL_Point* getSDLPoint(listPoint2D listPoint, float xmin, float ymin, float xmax, float ymax){
    int listTaille = getTailleList2D(listPoint);
    SDL_Point* list = (SDL_Point*) malloc(listTaille*sizeof(SDL_Point));
    for(int i=0; i<listTaille; i++){
        list[i].x = 640*(getXListPoint2D(listPoint,i) - xmin)/(xmax-xmin);
        list[i].y = 480*(getYListPoint2D(listPoint,i) - ymin)/(ymax-ymin);
    }
    return list;
}

void displayListPointInterface(SDL_Renderer* ren, listPoint2D listPoint, float xmin, float ymin, float xmax, float ymax){
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    if (SDL_RenderDrawPoints(ren, getSDLPoint(listPoint,xmin,ymin,xmax,ymax), getTailleList2D(listPoint)) != 0){
        fprintf(stderr,"SDL_RenderDrawPoints Error: %s",SDL_GetError());
        SDL_Quit();
    }
    //SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderPresent(ren);

}

void displayHedgeInterface(SDL_Renderer* ren, hedge edge, float xmin, float ymin, float xmax, float ymax){
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    listPoint2D actualEdge;
    for(int i=0; i<getTailleHedge(edge); i++){
        actualEdge = getOneHedge(edge, i);
        if (SDL_RenderDrawLines(ren, getSDLPoint(getOneHedge(edge,i),xmin,ymin,xmax,ymax),2) != 0){
            fprintf(stderr,"SDL_RenderDrawLine Error: %s",SDL_GetError());
            SDL_Quit();
        }
    }
    //SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderPresent(ren);
}


int main(int argc, char** argv)
{
    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    {
        /* Création de la fenêtre */
        SDL_Window* pWindow = NULL;
        pWindow = SDL_CreateWindow("Ma première application SDL2",SDL_WINDOWPOS_UNDEFINED,
                                                                  SDL_WINDOWPOS_UNDEFINED,
                                                                  640,
                                                                  480,
                                                                  SDL_WINDOW_SHOWN);

        if( pWindow )
        {
            SDL_Renderer *ren = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (ren == NULL){
                SDL_DestroyWindow(pWindow);
                fprintf(stderr,"SDL_CreateRenderer Error: %s",SDL_GetError());
                SDL_Quit();
                return 1;
            }
            /*SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
            SDL_Point point = {320, 240};
            if (SDL_RenderDrawPoints(ren, &point, 1) != 0){
                SDL_DestroyWindow(pWindow);
                fprintf(stderr,"SDL_RenderDrawPoints Error: %s",SDL_GetError());
                SDL_Quit();
                return 1;
            }*/

            listPoint2D list = constructListPoint2DFromFile("test3");
            float xmin = getXmin(list);
            float xmax = getXmax(list);
            float ymin = getYmin(list);
            float ymax = getYmax(list);
            //displayListPointInterface(ren, Convex_Hull(list), xmin, ymin, xmax, ymax);
            displayListPointInterface(ren, list, xmin, ymin, xmax, ymax);
            listIndiceList Q = separatePointList(list, 4);
            hedge edgeTest = getPath(list, 2);
            //displayHedgeInterface(ren, edgeTest, xmin, ymin, xmax, ymax);

            listIndice listIndiceTest = constructeurListIndiceTaille(getTailleList2D(list)); 
            for(int i=0; i<getTailleIndice(listIndiceTest); i++){
                setIndice(&listIndiceTest, i, i);
            }
            listIndiceList finTest = constructeurListIndiceListTaille(1,list);
            setListIndice(&finTest, listIndiceTest, 0);

            hedge newedge = calcHedgeDelaunay(Q,4);
            //hedge newedge = calcHedgeDelaunay(finTest,1);

            //displayListIndiceList(Q);
            displayHedgeInterface(ren, newedge, xmin, ymin, xmax, ymax);
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
   
            char cont = 1; /* Détermine si on continue la boucle principale */
            SDL_Event event;
            while ( cont != 0 )
            {
                while ( SDL_PollEvent(&event) )
                {
                    switch (event.type) 
                    {
                        case SDL_QUIT:
                            cont=0;
                            break;
                    }
                }

                /* On a traité les événements, on peut continuer le jeu */
            }

            SDL_DestroyWindow(pWindow);
        }
        else
        {
            fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
        }
    }

    SDL_Quit();

    return 0;
}
