#include <stdlib.h>
#include <stdio.h>
#include "CL/opencl.h"
#include "listPoint.h"
#include <SDL2/SDL.h>
#include "listIndiceList.h"
#include "maillage.h"
#include "matriceTriangle.h"
#include "hedge.h"
#include <math.h>
#include <time.h>

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


int affichageMailleSDL(listPoint2D list, hedge maille)
{
    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    else{
        /* Création de la fenêtre */
        SDL_Window* pWindow = NULL;
        pWindow = SDL_CreateWindow("Maillage",SDL_WINDOWPOS_UNDEFINED,
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
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
            SDL_Point point = {320, 240};
            if (SDL_RenderDrawPoints(ren, &point, 1) != 0){
                SDL_DestroyWindow(pWindow);
                fprintf(stderr,"SDL_RenderDrawPoints Error: %s",SDL_GetError());
                SDL_Quit();
                return 1;
            }
            float xmin = getXmin(list);
            float xmax = getXmax(list);
            float ymin = getYmin(list);
            float ymax = getYmax(list);
            //displayListPointInterface(ren, Convex_Hull(list), xmin, ymin, xmax, ymax);
            displayListPointInterface(ren, list, xmin, ymin, xmax, ymax);
            //displayHedgeInterface(ren, edgeTest, xmin, ymin, xmax, ymax);



            //displayListIndiceList(Q);
            displayHedgeInterface(ren, maille, xmin, ymin, xmax, ymax);
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
   
            char cont = 1; // Détermine si on continue la boucle principale
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


char* read_file(char* fileName){

    FILE* fichier = NULL;
    char chaine[5000] = "";
    char* result = NULL;
    fichier = fopen(fileName, "r");
    int i=0;
    int compteur=0;
    int compteur2=0;

    if (fichier != NULL)
    {
        while (fgets(chaine, 5000, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
        {
            i=0;
            while(chaine[i]!='\n'){
                i++;
            }
            compteur=compteur+i+1;
            result = realloc(result, sizeof(char)*compteur);
            i=0;
            while(chaine[i]!='\n'){
                result[compteur2+i]=chaine[i];
                i++;
            }
            result[compteur-1]=chaine[i];
            compteur2 = compteur;
        }

        fclose(fichier);
    }
    else{
        printf("Impossible d'ouvrir le fichier");
        exit(1);
    }
    compteur++;
    result = realloc(result, sizeof(char)*compteur);
    result[compteur-1]='\0';
    return result;
}
int main (int argc, const char * argv[])
{

 // Variables pour la Plateforme de travail
    cl_platform_id plateformeID;
    cl_uint qtePlateformes;
    cl_int codeErreur;
    // Variables pour les périphériques contenant des unités de calcul
    cl_device_id TOUS_peripheriqueID;
    cl_uint TOUS_qtePeripheriques;
    // Variables pour les contextes de travail
    cl_context contexte;
    cl_context_properties proprietes[3];
    // Variable pour les files d'exécution des commandes
    cl_command_queue file_execution;
    // Variable pour la fonction à paralléliser
    cl_program programme;

    // Variable pour le noyau qui exécutera
    // le programme contenant la fonction parallèle.
    cl_kernel noyau;
    // Variables qui contiendront les données.
    cl_mem listPoint_buffer;
    cl_mem pointForPath_buffer;
    cl_mem taillePoint_buffer;
    cl_mem nbPrc_buffer;
    cl_mem paths_buffer;
    cl_mem taillePath_buffer;
    // Initialisation des variables de données

    int nbProcess = 5;

    listPoint2D list = constructListPoint2DFromFile("test3");
    listPoint2D copyList = constructListPoint2DFromListPoint(list);
    triByX(&copyList);
    int tailleCopyList = getTailleList2D(copyList); 
    listIndice pointForPath = findPointsPathIndice(copyList, nbProcess);
    int *paths = malloc(sizeof(int)*tailleCopyList*(nbProcess-1));
    for(int i=0; i<tailleCopyList*(nbProcess-1); i++){
        paths[i]=-1;
    }
    int *taillePath = malloc(sizeof(int)*(nbProcess-1));
    for(int i=0; i<nbProcess-1; i++){
        taillePath[i]=0;
    }
    char*  include_path = "-I .";

    // --------------------------------------------------------------------
    // ----------------- Fin de la section des variables ------------------
    // -------------------------------------------------------------------- 

  float tps;
  clock_t t1, t2;
    t1 = clock();
    // Création de la plateforme de travail
    codeErreur = clGetPlatformIDs(1, &plateformeID, &qtePlateformes);

    // Rechercher les PÉRIPHÉRIQUES dans le système, tout type confondu.
    codeErreur = clGetDeviceIDs(plateformeID, CL_DEVICE_TYPE_ALL, 1, &TOUS_peripheriqueID, &TOUS_qtePeripheriques);

    // Création du contexte de travail
    proprietes[0] = (cl_context_properties) CL_CONTEXT_PLATFORM;
    proprietes[1] = (cl_context_properties) plateformeID;
    proprietes[2] = 0;
    contexte = clCreateContext(proprietes, TOUS_qtePeripheriques, &TOUS_peripheriqueID, NULL, NULL, &codeErreur);

    // Création de la file d'exécution pour le CPU
    file_execution = clCreateCommandQueue(contexte, TOUS_peripheriqueID, 0, &codeErreur);

    //Creation du fichier source pour le kernel
    const char *sourcesCarre[2] = {read_file("fonctionKernel2.cl"), read_file("point2.c")};

    // Construire le programme avec la fonction auCarre pour le CPU
    programme = clCreateProgramWithSource(contexte, 2, (const char**)sourcesCarre, NULL, &codeErreur);

    // Compilation du programme
    codeErreur = clBuildProgram(programme, TOUS_qtePeripheriques, &TOUS_peripheriqueID, include_path, NULL, NULL);
    if (codeErreur != CL_SUCCESS) {
        // Si erreur de compilation, alors... Affichage du LOG de compilation
        char logErreurs[4096];
        size_t longueur_Log;
        clGetProgramBuildInfo (programme, TOUS_peripheriqueID, CL_PROGRAM_BUILD_LOG,
                sizeof(logErreurs), logErreurs, &longueur_Log);
        printf("\n[IMPRESSION DU JOURNAL DE COMPILATION]\n\n");
        printf("%s\n\n", logErreurs);
        printf("[FIN DU JOURNAL]\n\n"); 
        return 1;
    }

    // Association des variables de données avec le tampon d'échange
    listPoint_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(Point2D)*tailleCopyList, copyList.point, &codeErreur);
    pointForPath_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int)*(nbProcess-1), pointForPath.indice, &codeErreur);
    taillePoint_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int), &copyList.taille, &codeErreur);
    nbPrc_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int), &nbProcess, &codeErreur);
    paths_buffer = clCreateBuffer(contexte, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * tailleCopyList* (nbProcess-1), paths, &codeErreur);
    taillePath_buffer = clCreateBuffer(contexte, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * (nbProcess-1), taillePath, &codeErreur);


    // Construire le noyau
    noyau = clCreateKernel(programme, "getPath", &codeErreur);

    // Associer les tampons d'échanges avec
    // les arguments des fonctions à paralléliser
    codeErreur = clSetKernelArg(noyau, 0, sizeof(listPoint_buffer), &listPoint_buffer);
    codeErreur = clSetKernelArg(noyau, 1, sizeof(pointForPath_buffer), &pointForPath_buffer);
    codeErreur = clSetKernelArg(noyau, 2, sizeof(taillePoint_buffer), &taillePoint_buffer);
    codeErreur = clSetKernelArg(noyau, 3, sizeof(nbPrc_buffer), &nbPrc_buffer);
    codeErreur = clSetKernelArg(noyau, 4, sizeof(paths_buffer), &paths_buffer);
    codeErreur = clSetKernelArg(noyau, 5, sizeof(taillePath_buffer), &taillePath_buffer);

    // Mettre le noyau dans la file d'execution

    size_t dimensions_globales[] = { nbProcess-1, 0, 0 };
    codeErreur = clEnqueueNDRangeKernel(file_execution, noyau, 1, NULL, 
            dimensions_globales, NULL, 0, NULL, NULL);

    // Récupération des résultats dans le tampon
    //clEnqueueReadBuffer(file_execution, path_buffer, CL_TRUE,
    //        0, sizeof(int) * QTE_DONNEES, &paths, 0, NULL, NULL);
    clEnqueueReadBuffer(file_execution, paths_buffer, CL_TRUE,
            0, sizeof(int) *tailleCopyList* (nbProcess-1), paths, 0, NULL, NULL);
    clEnqueueReadBuffer(file_execution, taillePath_buffer, CL_TRUE,
            0, sizeof(int) * (nbProcess-1), taillePath, 0, NULL, NULL);









    printf("\n\n\n");

    cl_mem groupPaths_buffer;
    cl_mem groupPathTaille_buffer;
    int *groupPaths = malloc(sizeof(int)*tailleCopyList*(nbProcess));
    for(int i=0; i<tailleCopyList*(nbProcess); i++){
        groupPaths[i]=-1;
    }
    int *groupPathTaille = malloc(sizeof(int)*(nbProcess));
    for(int i=0; i<nbProcess; i++){
        groupPathTaille[i]=0;
    }

    // Association des variables de données avec le tampon d'échange
    paths_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * tailleCopyList* (nbProcess-1), paths, &codeErreur);
    taillePath_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * (nbProcess-1), taillePath, &codeErreur);
    groupPaths_buffer = clCreateBuffer(contexte, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * tailleCopyList* (nbProcess), groupPaths, &codeErreur);
    groupPathTaille_buffer = clCreateBuffer(contexte, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * (nbProcess), groupPathTaille, &codeErreur);


    // Construire le noyau
    noyau = clCreateKernel(programme, "getGroup", &codeErreur);

    // Associer les tampons d'échanges avec
    // les arguments des fonctions à paralléliser
    codeErreur = clSetKernelArg(noyau, 0, sizeof(listPoint_buffer), &listPoint_buffer);
    codeErreur = clSetKernelArg(noyau, 1, sizeof(paths_buffer), &paths_buffer);
    codeErreur = clSetKernelArg(noyau, 2, sizeof(taillePath_buffer), &taillePath_buffer);
    codeErreur = clSetKernelArg(noyau, 3, sizeof(taillePoint_buffer), &taillePoint_buffer);
    codeErreur = clSetKernelArg(noyau, 4, sizeof(nbPrc_buffer), &nbPrc_buffer);
    codeErreur = clSetKernelArg(noyau, 5, sizeof(groupPaths_buffer), &groupPaths_buffer);
    codeErreur = clSetKernelArg(noyau, 6, sizeof(groupPathTaille_buffer), &groupPathTaille_buffer);

    // Mettre le noyau dans la file d'execution

    size_t dimensions_globales2[] = { nbProcess, 0, 0 };
    codeErreur = clEnqueueNDRangeKernel(file_execution, noyau, 1, NULL, 
            dimensions_globales2, NULL, 0, NULL, NULL);

    // Récupération des résultats dans le tampon
    clEnqueueReadBuffer(file_execution, groupPaths_buffer, CL_TRUE,
            0, sizeof(int) *tailleCopyList* (nbProcess), groupPaths, 0, NULL, NULL);
    clEnqueueReadBuffer(file_execution, groupPathTaille_buffer, CL_TRUE,
            0, sizeof(int) * (nbProcess), groupPathTaille, 0, NULL, NULL);






    
    cl_mem maillage_buffer;
    cl_mem maillageTaille_buffer;
    int *maillage2 = malloc(6*sizeof(int)*tailleCopyList*(nbProcess));
    for(int i=0; i<6*tailleCopyList*(nbProcess); i++){
        maillage2[i]=-1;
    }
    int *maillageTaille = malloc(sizeof(int)*(nbProcess));
    for(int i=0; i<nbProcess; i++){
        maillageTaille[i]=0;
    }
    // Association des variables de données avec le tampon d'échange
    groupPaths_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * tailleCopyList* (nbProcess), groupPaths, &codeErreur);
    groupPathTaille_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * (nbProcess), groupPathTaille, &codeErreur);
    maillage_buffer = clCreateBuffer(contexte, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * 6*tailleCopyList* (nbProcess), maillage2, &codeErreur);
    maillageTaille_buffer = clCreateBuffer(contexte, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * (nbProcess), maillageTaille, &codeErreur);


    // Construire le noyau
    noyau = clCreateKernel(programme, "getMaillageKernel", &codeErreur);

    // Associer les tampons d'échanges avec
    // les arguments des fonctions à paralléliser
    codeErreur = clSetKernelArg(noyau, 0, sizeof(listPoint_buffer), &listPoint_buffer);
    codeErreur = clSetKernelArg(noyau, 1, sizeof(paths_buffer), &paths_buffer);
    codeErreur = clSetKernelArg(noyau, 2, sizeof(taillePath_buffer), &taillePath_buffer);
    codeErreur = clSetKernelArg(noyau, 3, sizeof(taillePoint_buffer), &taillePoint_buffer);
    codeErreur = clSetKernelArg(noyau, 4, sizeof(nbPrc_buffer), &nbPrc_buffer);
    codeErreur = clSetKernelArg(noyau, 5, sizeof(groupPaths_buffer), &groupPaths_buffer);
    codeErreur = clSetKernelArg(noyau, 6, sizeof(groupPathTaille_buffer), &groupPathTaille_buffer);
    codeErreur = clSetKernelArg(noyau, 7, sizeof(maillage_buffer), &maillage_buffer);
    codeErreur = clSetKernelArg(noyau, 8, sizeof(maillageTaille_buffer), &maillageTaille_buffer);

    // Mettre le noyau dans la file d'execution
    size_t dimensions_globales3[] = { nbProcess, 0, 0 };
    codeErreur = clEnqueueNDRangeKernel(file_execution, noyau, 1, NULL, 
            dimensions_globales3, NULL, 0, NULL, NULL);

    // Récupération des résultats dans le tampon
    clEnqueueReadBuffer(file_execution, maillage_buffer, CL_TRUE,
            0, sizeof(int) *6*tailleCopyList* (nbProcess), maillage2, 0, NULL, NULL);
    clEnqueueReadBuffer(file_execution, maillageTaille_buffer, CL_TRUE,
            0, sizeof(int) * (nbProcess), maillageTaille, 0, NULL, NULL);


    t2 = clock();
    tps = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("Le temps est de %f", tps);
    clReleaseMemObject(listPoint_buffer);
    clReleaseMemObject(pointForPath_buffer);
    clReleaseMemObject(taillePoint_buffer);
    clReleaseMemObject(nbPrc_buffer);
    clReleaseMemObject(paths_buffer);
    clReleaseMemObject(taillePath_buffer);
    clReleaseMemObject(maillage_buffer);
    clReleaseMemObject(maillageTaille_buffer);
    clReleaseMemObject(groupPaths_buffer);
    clReleaseMemObject(groupPathTaille_buffer);

    free(paths);
    free(taillePath);
    free(groupPaths);
    free(groupPathTaille);
    maillage leMaillageCL = constructMaillageFromCL(maillage2, maillageTaille, nbProcess, copyList);
    matriceTriangle matAdj = calcmatTriDelaunay(nbProcess, leMaillageCL);
    hedge resultat = calcHedgeDelaunay(copyList, nbProcess, matAdj);


    // Affichage des résultats
    int init = affichageMailleSDL(copyList, resultat);


    printf("\n");
    // Libération des ressources
    clReleaseProgram(programme);
    clReleaseKernel(noyau);
    clReleaseCommandQueue(file_execution);
    clReleaseContext(contexte);
    return 0;
}
