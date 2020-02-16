# GPU : Triangulation de Delaunay
## Contexte
Ce projet consiste à calculer la triangulation de Delaunay pour un ensemble de points en utilisant la programmation sur carte graphique.


## Présrequis
Installation d'openCL


## Interface, bibliothèque
* openCL
* sdl2/SDL
* stdio
* math



## Fonctionnement 
### Organisation
Le fichier fonctionKernel2.cl contient la fonction exécutée par le kernel. Elle utilise des fonctions codées dans les fichiers point2.c et point2.h.
### Création des ensembles de points
* Fichier sous le format :

[nombre de points]

[coordonnée x] [coordonnée y] 

[coordonnée x] [coordonnée y] 

[coordonnée x] [coordonnée y] 

...


* Création automatique :

Fichier : constructeurListPoint.c

1 - mettre le nombre de points désiré dans la variable : nbPoint

2 - choisir les valeurs de coordonnées max et min dans les variables : MAX et MIN

3 - choisir le nom du fichier sortant : file = fopen("NOM_FICHIER", "w+");


Compiler et exécuter
```
gcc constructeurListPoint.c -o constructeurListPoint
./constructeurListPoint.o
rm constructeurListPoint.o
```


### Exécution du programme
Choix du fichier qui contient l'ensemble de points et du nombre de threads dans le fichier testGPU.c (pour l'affichage)

```
make
./maillageGPU
```
Résultats : calcul la triangulation et affiche le maillage dans une fenêtre graphique.


## Auteurs
* **Rémy Lavainne**
* **Michelle Malara**
