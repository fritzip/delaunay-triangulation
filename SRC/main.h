#ifndef __MAIN_H__
#define __MAIN_H__
/*! \file main.h
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include "linked_list.h"
#include "print_fn.h"
#include "ressources.h"

/*! variable externe permettant de lire les parametres sur le ligne de commande.*/
extern char *optarg;

/*! bascule pour autoriser ou interdire (0) les messages d'erreurs envoyes par getopt. */
extern int opterr;

/* Display option */
int displayChoice = 3;
char *lineOption[] = { "GL_POINTS", "GL_LINES", "GL_LINE_STRIP", "GL_LINE_LOOP", "GL_POLYGON"};


/*----------------------------------------------------------------------------------*/
//                                  Structures
/*----------------------------------------------------------------------------------*/


#if(DEBUG == 1)
	void debug()

#endif

void affiche(Dllist *dll, int LNK, int GL_DRAW_STYLE, double r, double g, double b);

void display (void);

#endif