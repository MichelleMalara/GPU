# Nous voulons un cmake "récent" pour utiliser les dernières fonctionnalités
cmake_minimum_required(VERSION 3.0)

# Notre projet est étiqueté hello
project(hello)

# On inclut notre bibliothèque dans le processus de CMake
add_subdirectory(SDL2)

# Crée des variables avec les fichiers à compiler
set(SRCS
    testSDL.c
    listPoint.c
    point.c
    )
set(HEADERS
    listPoint.h
    point.h
    )

# Notre exécutable
add_executable(main ${SRCS} ${HEADERS})

# Et pour que l'exécutable fonctionne, il faut lui indiquer la bibliothèque dont il dépend
target_link_libraries(main m)
