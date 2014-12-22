#ifndef __SIMPLEX_H__
#define __SIMPLEX_H__
/*! \file simplex.h
 * \author M. Sainlot
 * \date 2014
 */

#include "dll.h"
#include "vertex.h"

typedef struct simplex
{
	Vertex *sommet[3];
	struct simplex *voisin[3];
	struct simplex *next_stk;
	Dllist *candidates;
	
	double na;
	double nb;
	double nc;

	int index_in_fdp;
} Simplex ;


/*----------------------------------------------------------------------------------*/
//                                  Simplex functions prototypes
/*----------------------------------------------------------------------------------*/

void init_simplex( Simplex *simp, Vertex *v0, Vertex *v1, Vertex *v2, Dllist *candidates ) ;

Simplex* create_simplex( Vertex *v0, Vertex *v1, Vertex *v2 ) ;

int inside_simplex( Simplex *simp, Vertex *vert) ;

int is_empty( Simplex *simp ) ;

int is_superior_simplex( Simplex *s1, Simplex *s2 ) ;

int get_vertex_index(Vertex *vert, Simplex *simp) ;

int get_neighbor_index( Simplex *neighbor, Simplex *simp) ;

#endif