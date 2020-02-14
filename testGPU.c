#include <stdlib.h>
#include <stdio.h>
#include "CL/opencl.h"
#include "listPoint.h"
#include "listIndiceList.h"
#include "hedge.h"
#include <math.h>

#define QTE_DONNEES 20
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
    /*const char *maFonctionAuCarre = {
        "__kernel void auCarre(__global int *input, __global int *output)\n"
        "{\n"
        " int id = get_global_id(0);\n"
        " int yo = 5;\n"
        " output[id] = input[id] * input[id];"
        "}\n"
    };*/
    const char *getPath = {
        "#include \"point.h\"\n"
        "#include \"listIndice.h\"\n"
        "#include \"listPoint.h\"\n"
        "#include \"listIndiceList.h\"\n"
        "__kernel void getPath(__global listPoint2D *input, __global listIndiceList *output, __global int *nbProcess, __global listIndice *pointForPath)\n"
        "{\n"
        " int id = get_global_id(0);\n"
        " if(id<*nbProcess-1){\n"
        " listPoint2D projec;\n"
            " projec = projectionWithIndice(*input,getIndice(*pointForPath,id));\n"
            //" setListIndice(output, Convex_HullIndice(projec), id);\n"
          "}\n"
        "}\n"
    };

    // Variable pour le noyau qui exécutera
    // le programme contenant la fonction parallèle.
    cl_kernel noyau;
    // Variables qui contiendront les données.
    int* inputData;
    float* outputData;
    cl_mem listPoint_buffer;
    cl_mem pointForPath_buffer;
    cl_mem output_buffer;
    // Initialisation des variables de données
    inputData = (int*)malloc(QTE_DONNEES * sizeof(int));
    outputData = (float*)malloc(QTE_DONNEES * sizeof(float));
    for (int i = 0; i < QTE_DONNEES; i++) {
        inputData[i] = i;
        outputData[i] = 0.;
    }

    int nbProcess = 4;

    listPoint2D list = constructListPoint2DFromFile("test3");
    listPoint2D copyList = constructListPoint2DFromListPoint(list);
    triByX(&copyList);
    listIndice pointForPath = findPointsPathIndice(copyList, nbProcess);
    listIndiceList paths = constructeurListIndiceListTaille(nbProcess-1, list);

    char*  include_path = "-I .";

    // --------------------------------------------------------------------
    // ----------------- Fin de la section des variables ------------------
    // -------------------------------------------------------------------- 

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

    /*static const char *sources[5] = {
        read_file("point.c"),
        read_file("listPoint.c"),
        read_file("listIndice.c"),
        read_file("listIndiceList.c"),
        getPath
    };*/
    /*const char *sources[5] = {
        read_file("point.c"),
        read_file("listPoint.c"),
        read_file("listIndice.c"),
        read_file("listIndiceList.c"),
        getPath
    };*/

    const char *sourcesCarre[4] = {read_file("fonctionKernel2.cl"), read_file("fonctionKernel.c"), read_file("point2.c"), read_file("listPoint2.c")};
    //const char *sourcesCarre2[2] = {read_file("fonctionKernel.cl"), read_file("fonctionKernel.c")};

    //printf(read_file("fonctionKernel.c"));
    // Construire le programme avec la fonction auCarre pour le CPU
    //programme = clCreateProgramWithSource(contexte, 5, (const char**)&sources, NULL, &codeErreur);
    //programme = clCreateProgramWithSource(contexte, 1, (const char**)&getPath, NULL, &codeErreur);
    programme = clCreateProgramWithSource(contexte, 4, (const char**)sourcesCarre, NULL, &codeErreur);

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
    /*input_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * QTE_DONNEES, inputData, &codeErreur);*/
    listPoint_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(listPoint2D), &copyList, &codeErreur);
    pointForPath_buffer = clCreateBuffer(contexte, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(listIndice), &pointForPath, &codeErreur);
    output_buffer = clCreateBuffer(contexte, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(float) * QTE_DONNEES, outputData, &codeErreur);


    // Construire le noyau
    noyau = clCreateKernel(programme, "auCarre", &codeErreur);
    //noyau = clCreateKernel(programme, "getPath", &codeErreur);

    // Associer les tampons d'échanges avec
    // les arguments des fonctions à paralléliser
    codeErreur = clSetKernelArg(noyau, 0, sizeof(listPoint_buffer), &listPoint_buffer);
    codeErreur = clSetKernelArg(noyau, 1, sizeof(pointForPath_buffer), &pointForPath_buffer);
    codeErreur = clSetKernelArg(noyau, 2, sizeof(output_buffer), &output_buffer);
    /*codeErreur = clSetKernelArg(noyau, 0, sizeof(list_buffer), &list_buffer);
    codeErreur = clSetKernelArg(noyau, 1, sizeof(path_buffer), &path_buffer);
    codeErreur = clSetKernelArg(noyau, 2, sizeof(nbProcess_buffer), &nbProcess_buffer);
    codeErreur = clSetKernelArg(noyau, 3, sizeof(pointForPath_buffer), &pointForPath_buffer);*/

    // Mettre le noyau dans la file d'execution
    size_t dimensions_globales[] = { QTE_DONNEES, 0, 0 };
    codeErreur = clEnqueueNDRangeKernel(file_execution, noyau, 1, NULL, 
            dimensions_globales, NULL, 0, NULL, NULL);

    // Récupération des résultats dans le tampon
    //clEnqueueReadBuffer(file_execution, path_buffer, CL_TRUE,
    //        0, sizeof(int) * QTE_DONNEES, &paths, 0, NULL, NULL);
    clEnqueueReadBuffer(file_execution, output_buffer, CL_TRUE,
            0, sizeof(float) * QTE_DONNEES, outputData, 0, NULL, NULL);

    // Affichage des résultats
    printf("\nOuahhh\n");
    printf("\n\n\n >>>>> Affichage des resultats <<<<<");
    printf("\n\nContenu de la variable --inputData--\n");
    for (int i = 0; i < QTE_DONNEES; i++) {
        printf("%d; ", inputData[i]);
    }
    printf("\n\nContenu de la variable --outputData--\n");
    printf("Fonction --auCarre--\n");
    for (int i = 0; i < QTE_DONNEES; i++) {
        printf("%f; ", outputData[i]);
    }
    printf("\n");
    // Libération des ressources
    free(inputData);
    free(outputData);
    clReleaseMemObject(listPoint_buffer);
    clReleaseMemObject(pointForPath_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseProgram(programme);
    clReleaseKernel(noyau);
    clReleaseCommandQueue(file_execution);
    clReleaseContext(contexte);
    return 0;
}
