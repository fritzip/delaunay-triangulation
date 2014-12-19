#ifndef __MAIN_H__
#define __MAIN_H__
/*! \file main.h
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include "primitives.h"
#include "print_fn.h"
#include "ressources.h"


/*----------------------------------------------------------------------------------*/
//                                  Structures
/*----------------------------------------------------------------------------------*/


void affiche_dll(Dllist *dll, int LNK, int GL_DRAW_STYLE, double r, double g, double b);

void affiche_simplex(Simplex *simp, double r, double g, double b);

void affiche_grid(Grid *grid, double r, double g, double b);

void display (void);

#endif