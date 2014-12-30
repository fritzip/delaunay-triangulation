Triangulation de Delaunay
=========================

### Dépendances ###

 * Librairies de base du C
 * gcc
 * Librairie Graphique : OpenGL

### Dossiers ###

Le projet est divisé en plusieurs dossiers :

* SRC : contient toute les sources du projet (*.c *.h)
* BIN : contient l'executable généré après compilation (delaunay). Un lien symbolique est également créé dans le repertoire source du projet afin de pourvoir exécuter immédiatement après compilation sans changer de dossier. _Généré automatiquement si inexistant._ 
* OBJ : contient les fichiers binaires compilés mais non linkés (.o). _Généré automatiquement si inexistant._ 
* DATA : contient des images (heightmap) pouvant être donné en entrée de l'algorithme pour obtenir une génération de points non aléatoire sur z. 
* DOC : contient l'ensemble des fichiers de documentation. _Dossier et fichiers générés automatiquement à l'aide de doxygen_.
* RAPPORT : rapport LaTeX du projet.

### Fichiers SRC/ ###

Les sources du projet sont divisées en plusieurs fichiers (chaque *.c ayant sont *.h associé):

* vertex : Structure et fonctions relatives aux sommets
* simplex : Structure et fonctions relatives aux triangles
* dll : Structure et fonctions relatives aux listes doublement chainées
* fdp : Structure et fonctions relatives aux files de prioritées
* triangulation : Structure et fonctions relatives à la triangulation et la génération du maillage
* math_fn : Fonctions mathématiques (géométriques) utiles au programme (prédicat orientation, signe, déterminant…)
* main : Point de départ de l'algorithme et gestion de l'affichage.
* ressources.h : #define du projet. _Pas de *.c associé_

_Se reporter à la documentation pour plus d'information_

### Fichiers / ###

* Makefile
* Doxyfile
* README.md


### Utilisation du programme ###

* Compilation 

        make

* Génération de la documentation (optionel)

        make dox

* Lancement du programme

        ./delaunay [-i INPUT_FILE] [-n NB_PTS] [-s NB_SIMPX | -g GOF(%)] [-c DISPLAY_MODE]

    * `-i INPUT_FILE` : de la forme `DATA/heightmap`. Où `heightmap` est une image d'extension quelconque (elle sera automatiquement convertie en `heightmap.pgm`). _Defaut : NULL_

    * `-n NB_PTS` : nombre de points générés aléatoirement en x, y, (et z si l'option -i n'est pas renseignée). _Defaut : 10000_ 

    * `-s NB_SIMPX` et `-g GOF(%)` : conditions d'arrêt, mutuellement exclusives. 

        * `-s` arrête la triangulation une fois le nombre de facettes générées égal à `NB_SIMPX` (ou si le nombre maximum de facettes est atteint). 

        * `-g` arrête la triangulation une fois le point le plus éloigné (en projection verticale) du triangle auquel il appartient inferieur à `zmax*GOF/100` (z étant normalisé, zmax = 1)

        * _Defaut : arrêt quand tout les points ont été insérés._


    * `-c DISPLAY_MODE` : 0 ≤ DISPLAY_MODE ≤ 4. _Defaut : 3_

        * 0 : GL_POINTS
        * 1 : GL_LINES
        * 2 : GL_LINE_STRIP
        * 3 : GL_LINE_LOOP
        * 4 : GL_POLYGON

* Interaction avec le programme


### Crédits ###

Maxime Sainlot

Master 2 Informatique Graphique et Image - Projet Géométrie Algorithmique
