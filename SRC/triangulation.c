/*! \file triangulation.c
 * \author M. Sainlot
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "triangulation.h"
#include "math_fn.h"


/*----------------------------------------------------------------------------------*/
//                                  Triangulation functions
/*----------------------------------------------------------------------------------*/

Grid* create_grid( const int nb_pts, const int size_fdp, const int init_size_new_tab, const PGMData *pic, const double GOF )
{
	Grid *grid = (Grid *) malloc(sizeof(Grid));

	// 4 corners vertex
	Vertex *ul = create_vertex(0.0, 1.0, 0.0);
	Vertex *ur = create_vertex(1.0, 1.0, 0.0);
	Vertex *dr = create_vertex(1.0, 0.0, 0.0);
	Vertex *dl = create_vertex(0.0, 0.0, 0.0);

	// 2 init triangles
	Simplex *simp[2];
	simp[0] = create_simplex(ur, ul, dl);
	simp[1] = create_simplex(dl, dr, ur);
	simp[0]->voisin[1] = simp[1];
	simp[1]->voisin[1] = simp[0];

	// init vertex
	Vertex *new_vert;
	int i, j;
	for (i = 0; i < nb_pts; i++)
	{
		if (pic->matrix != NULL)
			new_vert = create_vertex_img_based(pic);
		else
			new_vert = create_vertex(randf(), randf(), randf());
		for (j = 0; j < 2; j++)
		{
			if ( inside_simplex( simp[j], new_vert ) )
				break;
		}

		new_vert->zdist = compute_zdist( simp[j], new_vert );

		if ( simp[j]->candidates->length > 0 && is_superior_vertex( new_vert, simp[j]->candidates->root->links[STD][FWD]) )
			add_begin_dll( simp[j]->candidates, new_vert, STD );
		else
			add_end_dll( simp[j]->candidates, new_vert, STD );
	}

	grid->fdp = create_fdp( size_fdp );

	grid->candidates_to_redistribute = create_dll();

	grid->top_of_stack = NULL;
	grid->stack_size = 0;

	grid->new = (Simplex **)malloc(init_size_new_tab*sizeof(Simplex *));
	grid->new_current_size = 0;
	grid->new_max_size = init_size_new_tab;

	grid->gof = GOF;

	insert_in_fdp(grid->fdp, simp[0]);
	insert_in_fdp(grid->fdp, simp[1]);

	return grid;
}

void redistribute_candidates( Dllist *dll, Simplex *tab[], const int nb_simp, const int LNK )
{
	int i, j;
	int nb_candidates = dll->length;
	Vertex *current = dll->root->links[LNK][FWD];
	Vertex *next = NULL;

	// redistributes candidates through 3 new triangles and update zdist
	for (i = 0; i < nb_candidates; i++)
	{
		for (j = 0; j < nb_simp; j++)
			if ( inside_simplex( tab[j], current ) ) break;

		current->zdist = compute_zdist( tab[j], current );
		next = current->links[STD][FWD];
		if ( tab[j]->candidates->length > 0 && is_superior_vertex( current, tab[j]->candidates->root->links[STD][FWD]) )
			add_begin_dll( tab[j]->candidates, current, STD );
		else
			add_end_dll( tab[j]->candidates, current, STD );
		current = next;
	}
}

void add_end_array( Simplex **tab, int *c_size, int *m_size, Simplex *simp )
{
	if ( *c_size >= *m_size )
	{
		printf("OFFSET\n");
		tab = (Simplex **) realloc(tab, (*m_size + OFFSET) * sizeof(Simplex *));
		*m_size += OFFSET;
	}

	tab[*c_size] = simp;
	*c_size = *c_size + 1;
}

void stack( Grid *grid, Simplex *simp )
{
	simp->next_stk = grid->top_of_stack;
	grid->top_of_stack = simp;
	grid->stack_size++;
}

Simplex* unstack( Grid *grid )
{
	Simplex *top = grid->top_of_stack;
	grid->top_of_stack = top->next_stk;
	top->next_stk = NULL;
	grid->stack_size--;
	return top;
}

void split_in_3(Grid *grid, Simplex *simp, Vertex *vert)
{
	// define new vertices of new simplex
	Simplex *new_simp1 = create_simplex(vert, simp->sommet[1], simp->sommet[2]);
	Simplex *new_simp2 = create_simplex(vert, simp->sommet[2], simp->sommet[0]);
	simp->sommet[2] = vert;

	// define new neighbors
	new_simp1->voisin[0] = simp->voisin[0];
	new_simp1->voisin[1] = new_simp2;
	new_simp1->voisin[2] = simp;

	new_simp2->voisin[0] = simp->voisin[1];
	new_simp2->voisin[1] = simp;
	new_simp2->voisin[2] = new_simp1;

	simp->voisin[0] = new_simp1;
	simp->voisin[1] = new_simp2;

	int ind_neighbor;
	if (new_simp1->voisin[0] !=NULL)
	{
		ind_neighbor = get_neighbor_index( simp, new_simp1->voisin[0]);
		new_simp1->voisin[0]->voisin[ind_neighbor] = new_simp1;
	}

	if (new_simp2->voisin[0] !=NULL)
	{
		ind_neighbor = get_neighbor_index( simp, new_simp2->voisin[0]);
		new_simp2->voisin[0]->voisin[ind_neighbor] = new_simp2;
	}

	compute_plan(simp);

	insert_in_fdp(grid->fdp, new_simp1);
	insert_in_fdp(grid->fdp, new_simp2);

	stack( grid, new_simp1 );
	stack( grid, new_simp2 );
	stack( grid, simp );
}

void flip(Grid *grid, Simplex *current, int ind_vert, Simplex *opp, int ind_opp)
{
	int ind_neighbor;

	current->voisin[ind_vert] = opp->voisin[(ind_opp+1)%3];
	if (opp->voisin[(ind_opp+1)%3] !=NULL)
	{
		ind_neighbor = get_neighbor_index( opp, opp->voisin[(ind_opp+1)%3]);
		opp->voisin[(ind_opp+1)%3]->voisin[ind_neighbor] = current;
	}
	
	opp->voisin[ind_opp] = current->voisin[(ind_vert+1)%3];
	if (current->voisin[(ind_vert+1)%3] != NULL)
	{
		ind_neighbor = get_neighbor_index( current, current->voisin[(ind_vert+1)%3] );
		current->voisin[(ind_vert+1)%3]->voisin[ind_neighbor] = opp;
	}

	current->voisin[(ind_vert+1)%3] = opp;
	opp->voisin[(ind_opp+1)%3] = current;

	current->sommet[(ind_vert+2)%3] = opp->sommet[ind_opp];
	opp->sommet[(ind_opp+2)%3] = current->sommet[ind_vert];

	compute_plan(current);
	compute_plan(opp);

	stack( grid, current);
	stack( grid, opp);
}

void delauney( Grid *grid )
{
	Simplex *simp =	grid->fdp->table[1] ;

	// Vertex to insert (first candidat)
	Vertex *vert = simp->candidates->root->links[STD][FWD];
	
	vert->zdist = 0;

	rm_begin_dll(simp->candidates, STD);

	add_dll_end_dll(grid->candidates_to_redistribute, simp->candidates->root->links[STD][FWD], simp->candidates->root->links[STD][BWD], simp->candidates->length, STD );

	// reinit dll
	init_dll(simp->candidates, simp->candidates->root);

	split_in_3(grid, simp, vert);

	Simplex *current = NULL;
	Simplex *opp = NULL;
	int ind_vert;
	int ind_opp;

	while (grid->stack_size > 0)
	{
		current = unstack(grid);
		ind_vert = get_vertex_index(vert, current);
		opp = current->voisin[ind_vert];

		if ( opp !=NULL && in_circle( opp->sommet[0], opp->sommet[1], opp->sommet[2], vert ) )
		{
			ind_opp = (get_vertex_index(current->sommet[(ind_vert + 1)%3], opp) + 1) %3 ;
			add_dll_end_dll(grid->candidates_to_redistribute, opp->candidates->root->links[STD][FWD], opp->candidates->root->links[STD][BWD], opp->candidates->length, STD );
			init_dll(opp->candidates, opp->candidates->root);

			flip(grid, current, ind_vert, opp, ind_opp);
		}
		else
		{
			add_end_array(grid->new, &grid->new_current_size, &grid->new_max_size, current );
		}
	}

	redistribute_candidates(grid->candidates_to_redistribute, grid->new, grid->new_current_size, STD );

	for (int i = 0; i < grid->new_current_size; i++)
	{
		up_heap( grid->fdp, grid->new[i]->index_in_fdp, grid->new[i]->index_in_fdp/2 );
		down_heap( grid->fdp, grid->new[i]->index_in_fdp*2, grid->new[i]->index_in_fdp );
	}

	// reinit
	init_dll(grid->candidates_to_redistribute, grid->candidates_to_redistribute->root);

	int n = grid->new_current_size;
	for (int i = 0; i < n; i++)
	{
		grid->new[i] = NULL;
		grid->new_current_size--;
	}
}
