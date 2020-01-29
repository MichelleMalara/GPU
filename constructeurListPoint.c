#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "listPoint.h"
#include "hedge.h"
#include "listIndice.h"
#include "listIndiceList.h"
#include "maillage.h"
#include "matriceTriangle.h"
#include <time.h>

int main()

{

    int nbPoint = 200;
    FILE* file = NULL;
    file = fopen("test2", "w+");
    int MAX = 10000;
    int MIN = 0;
    int nombre = 0;
    int nombre2 = 0;
    if (file != NULL){

        fprintf(file, "%d\n", nbPoint);
        srand(time(NULL));
        for(int i=0; i<nbPoint; i++){
            nombre = (rand() % (MAX - MIN + 1)) + MIN;
            nombre2 = (rand() % (MAX - MIN + 1)) + MIN;
            fprintf(file, "%f %f\n", ((float) nombre)/1000., ((float) nombre2)/1000.);
        }
    }
    fclose(file);
    return 0;

}

