/*! \file fdp.c
 * \author M. Sainlot
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "fdp.h"
/*----------------------------------------------------------------------------------*/
//                                  Priority Queue
/*----------------------------------------------------------------------------------*/

void init_fdp( FDP *fdp, Simplex **tab, int size )
{
	fdp->table = tab;
	fdp->nb = 0;
}

FDP* create_fdp( int size )
{
	FDP *new_fdp = (FDP *) malloc(sizeof(FDP));
	Simplex **new_tab = (Simplex **)malloc(size*sizeof(Simplex *));

	if (new_fdp != NULL && new_tab != NULL)
		init_fdp( new_fdp, new_tab, size );
	else
		printf("Error in create_fdp function\n");

	return new_fdp;
}

void switch_cells_fdp( FDP *fdp, const int a, const int b )
{
	int ind = fdp->table[a]->index_in_fdp;
	fdp->table[a]->index_in_fdp = fdp->table[b]->index_in_fdp;
	fdp->table[b]->index_in_fdp = ind;

	Simplex *c = fdp->table[a];
	fdp->table[a] = fdp->table[b];
	fdp->table[b] = c;
}

int get_number_of_sons( const int i, const int n )
{
	int nb_leafs = (n + 1) / 2;
	if ( i > n - nb_leafs) return 0;
	else if (i == n - nb_leafs && !(n % 2)) return 1;
	else return 2;
}

int is_superior_fdp( FDP *fdp, const int a, const int b )
{
	fdp->nb_comp++;
	if ( !is_empty(fdp->table[a]) && !is_empty(fdp->table[b]) )
		return is_superior_simplex(fdp->table[a], fdp->table[b]);
	else if ( !is_empty(fdp->table[a]) )
		return 1;
	else 
		return 0;
}

void up_heap( FDP *fdp, int son, int father )
{
	while(father > 0)
	{
		if (is_superior_fdp(fdp, son, father))
		{
			switch_cells_fdp(fdp, son, father);
			son = father;
			father = son/2;
		}
		else break;
	}
}

void down_heap( FDP *fdp, int son, int father )
{
	int LOOP = 1;

	while(LOOP)
	{
		switch( get_number_of_sons(father, fdp->nb) )
		{
			case 2: 
				son += is_superior_fdp(fdp, son+1, son);
				if ( is_superior_fdp(fdp, son, father) ) switch_cells_fdp(fdp, son, father);
				break;
			case 1: 
				if ( is_superior_fdp(fdp, son, father) ) switch_cells_fdp(fdp, son, father);
				break;
			case 0: 
				LOOP = 0;
				break;
			default: printf("Error in function down_heap\n");
				break;
		}

		father = son;
		son *= 2;
	}
}

void insert_in_fdp( FDP *fdp, Simplex *simp )
{
	simp->index_in_fdp = fdp->nb + 1;
	fdp->table[fdp->nb + 1] = simp;
	fdp->nb++;
	up_heap(fdp, fdp->nb, fdp->nb / 2);
}

void heap_sort( FDP *fdp )
{
	int son, father;

	for(int i = 2; i <= fdp->nb; i++)
	{
		son = i;
		father = son/2;
		up_heap(fdp, son, father);
	}

	while(fdp->nb > 1)
	{
		switch_cells_fdp(fdp, 1, fdp->nb);

		fdp->nb--;

		down_heap(fdp, 2, 1);
	}
}
