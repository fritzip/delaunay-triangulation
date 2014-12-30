Triangulation de Delaunay
=========================

### Dossiers ###

Le projet est divisé en plusieurs dossiers :

* SRC : contient toute les sources du projet (*.c *.h)
* BIN : contient l'executable généré après compilation (delaunay). Un lien symbolique est également créé dans le repertoire source du projet afin de pourvoir exécuter immédiatement après compilation sans changer de dossier. Généré automatiquement si inexistant. 
* OBJ : contient les fichiers binaires compilés mais non linkés (*.o). Généré automatiquement si inexistant. 
* DATA : contient des images (heightmap) pouvant être donné en entrée de l'algorithme pour obtenir une génération de points non aléatoire sur z. 
* DOC : contient l'ensemble des fichiers de documentation automatiquement générés par doxygen.
* RAPPORT : rapport LaTeX du projet.

### Fichiers SRC ###

Les sources du projet sont divisées en plusieurs fichiers (chaque *.c ayant sont *.h associé):

* vertex
* simplex
* dll
* fdp
* triangulation
* math_fn
* main
* ressources.h


### Lancement du programme ###


### Crédits ###

Maxime Sainlot

Master 2 Informatique Graphique et Image - Projet Géométrie Algorithmique
