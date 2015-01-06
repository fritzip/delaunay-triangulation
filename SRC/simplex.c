/*! \file simplex.c
 * \author M. Sainlot
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "simplex.h"
#include "math_fn.h"

/*----------------------------------------------------------------------------------*/
//                                  Simplex functions
/*----------------------------------------------------------------------------------*/

void init_simplex( Simplex *simp, Vertex *v0, Vertex *v1, Vertex *v2, Dllist *candidates )
{
	simp->sommet[0] = v0;
	simp->sommet[1] = v1;
	simp->sommet[2] = v2;

	compute_plan(simp);

	for(int i=0; i<3; i++)
		simp->voisin[i] = NULL;

	simp->candidates = candidates;
	
	for (int i = 0; i < NB_STK; ++i)
	{
		simp->next_in_stack[i] = NULL;
	}

	simp->index_in_fdp = 0;
}

Simplex* create_simplex( Vertex *v0, Vertex *v1, Vertex *v2 )
{
	Simplex *new_simp = (Simplex *) malloc(sizeof(Simplex));
	Dllist *new_dll = create_dll();

	if (new_simp != NULL && new_dll != NULL)
		init_simplex( new_simp, v0, v1, v2, new_dll);
	else
		printf("Error in create_simplex function\n");

	return new_simp;
}

Simplex* create_empty_simplex()
{
	Simplex *simp = (Simplex *)malloc(sizeof(Simplex));
	simp->candidates = create_dll();

	for(int i=0; i<3; i++)
	{
		simp->sommet[i] = NULL;
		simp->voisin[i] = NULL;
	}
	
	for (int i = 0; i < NB_STK; ++i)
	{
		simp->next_in_stack[i] = NULL;
	}

	simp->index_in_fdp = 0;

	return simp;
}

int inside_simplex( Simplex *simp, Vertex *vert )
{
	int ori = 0;
	for (int i = 0; i < 3; i++)
	{
		ori += orientation(simp->sommet[i], simp->sommet[(i+1)%3], vert);
	}
	return ( fabs(ori) == 3 ) ;
}

int is_empty( Simplex *simp )
{
	return 1 - sgn(simp->candidates->length);
}

int is_superior_simplex( Simplex *s1, Simplex *s2 )
{
	return is_superior_vertex(s1->candidates->root->links[STD][FWD], s2->candidates->root->links[STD][FWD] );
}

int get_vertex_index(Vertex *vert, Simplex *simp)
{
	int i;
	for (i = 0; i < 3; i++)
		if (vert == simp->sommet[i]) break;

	return i;
}

int get_neighbor_index( Simplex *neighbor, Simplex *simp)
{
	int i;
	for (i = 0; i < 3; i++)
		if (neighbor == simp->voisin[i]) break; 

	return i;
}
