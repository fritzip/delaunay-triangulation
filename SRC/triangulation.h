#ifndef __GRID_H__
#define __GRID_H__
/*! \file primitives.h
 * \author M. Sainlot
 * \date 2014
 */

#include "dll.h"
#include "simplex.h"
#include "vertex.h"
#include "fdp.h"
#include "pgm.h"

typedef struct grid
{
	FDP *fdp;

	Dllist *candidates_to_redistribute;
	
	Simplex *top_of_stack;
	int stack_size;
	
	Simplex **new;
	int new_current_size;
	int new_max_size;

	double gof;
} Grid ;


/*----------------------------------------------------------------------------------*/
//                                  Triangulation prototypes
/*----------------------------------------------------------------------------------*/

void init_grid( Grid *grid, Dllist *dll, FDP *fdp, Simplex **tab, const int init_size_new_tab, const int nb_pts, const PGMData *pic );

Grid* create_grid( const int nb_pts, const int size_fdp, const int init_size_new_tab, const PGMData *pic );

void redistribute_candidates( Dllist *dll, Simplex **tab, const int nb_simp, const int LNK );

void add_end_array( Simplex **tab, int *c_size, int *m_size, Simplex *simp );

void stack( Grid *grid, Simplex *simp ) ;

Simplex* unstack( Grid *grid ) ;

void split_in_3(Grid *grid, Simplex *simp, Vertex *vert) ;

void flip(Grid *grid, Simplex *current, int ind_vert, Simplex *opp, int ind_opp);

void delauney( Grid *grid ) ;


#endif