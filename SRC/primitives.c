/*! \file primitives.c
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "primitives.h"
#include "math_fn.h"
#include "print_fn.h"

/*----------------------------------------------------------------------------------*/
//                                  DLL Functions
/*----------------------------------------------------------------------------------*/

void init_dll(Dllist *dll, Vertex* root)
{
	dll->root = root;

	for(int i=0; i<NBL; i++)
	{
		dll->root->links[i][BWD] = dll->root;	
		dll->root->links[i][FWD] = dll->root;
		dll->length = 0;
	}
}

Dllist* create_dll( )
{
	Dllist *new_dll = (Dllist *) malloc(sizeof(Dllist));
	Vertex *new_vert = create_vertex(0.0, 0.0, 0.0);

	if (new_dll != NULL && new_vert != NULL)
		init_dll(new_dll, new_vert);
	else
		printf("Error in create_dll function\n");

	return new_dll;
}

void add_end_dll(Dllist *dll, Vertex *vert, const int LNK)
{
	// root <=> a <=> b <=> c <=> d      <=>     root
	// root <=> a <=> b <=> c <=> d <=> vert <=> root
	vert->links[LNK][BWD] = dll->root->links[LNK][BWD];
	vert->links[LNK][FWD] = dll->root;
	dll->root->links[LNK][BWD]->links[LNK][FWD] = vert;
	dll->root->links[LNK][BWD] = vert;
	dll->length++;
}

void add_begin_dll(Dllist *dll, Vertex *vert, const int LNK)
{
	// root     <=>      a <=> b <=> c <=> d <=> root
	// root <=> vert <=> a <=> b <=> c <=> d <=> root
	vert->links[LNK][BWD] = dll->root; 
	vert->links[LNK][FWD] = dll->root->links[LNK][FWD];
	dll->root->links[LNK][FWD]->links[LNK][BWD] = vert;
	dll->root->links[LNK][FWD] = vert;
	dll->length++;
}

void rm_end_dll(Dllist *dll, const int LNK)
{
	// root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> b <=> c <=> d <=> e    <=>    root 
	if ( dll->length > 0 )
	{
		Vertex *temp = dll->root->links[LNK][BWD];
		dll->root->links[LNK][BWD]->links[LNK][BWD]->links[LNK][FWD] = dll->root;
		dll->root->links[LNK][BWD] = dll->root->links[LNK][BWD]->links[LNK][BWD];
		temp->links[LNK][FWD] = NULL;
		temp->links[LNK][BWD] = NULL;
		dll->length--;
	}
	else
		printf("Cannot remove in empty list\n");
}

void rm_begin_dll(Dllist *dll, const int LNK)
{
	// root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// root    <=>    b <=> c <=> d <=> e <=> f <=> root 
	if ( dll->length > 0 )
	{
		Vertex *temp = dll->root->links[LNK][FWD];
		dll->root->links[LNK][FWD]->links[LNK][FWD]->links[LNK][BWD] = dll->root;
		dll->root->links[LNK][FWD] = dll->root->links[LNK][FWD]->links[LNK][FWD];
		temp->links[LNK][FWD] = NULL;
		temp->links[LNK][BWD] = NULL;
		dll->length--;
	}
	else
		printf("Cannot remove in empty list\n");
}

void insert_after(Dllist *dll, Vertex *prev, Vertex *ins, const int LNK)
{
	// root <=> a <=> prev     <=>     b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> prev <=> ins <=> b <=> c <=> d <=> e <=> f <=> root 
	ins->links[LNK][FWD] = prev->links[LNK][FWD];
	ins->links[LNK][FWD]->links[LNK][BWD] = ins;
	ins->links[LNK][BWD] = prev;
	prev->links[LNK][FWD] = ins;
	dll->length++;
}

void rm_after(Dllist *dll, Vertex *prev, const int LNK)
{
	// root <=> a <=> prev <=> b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> prev    <=>    c <=> d <=> e <=> f <=> root 
	Vertex *rm = prev->links[LNK][FWD];
	rm->links[LNK][FWD]->links[LNK][BWD] = prev;
	rm->links[LNK][BWD]->links[LNK][FWD] = rm->links[LNK][FWD];
	rm->links[LNK][FWD] = NULL;
	rm->links[LNK][BWD] = NULL;
	dll->length--;
}	


void insert_btw(Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, const int LNK)
{
	// root <=> a <=> inf <=> b <=> c <=> d <=> sup <=> e <=> f <=> root 
	// root <=> a <=> inf <=>      ins      <=> sup <=> e <=> f <=> root 
	insert_after(dll, inf, ins, LNK);
	Vertex *current = ins ;
	while (current->links[LNK][FWD] != sup)
	{
		rm_after(dll, current, LNK);
	}
}


void move_after(Vertex *prev, Vertex *move, const int LNK)
{
	// root <=> a <=> prev <=> b <=> c <=> d <=> move <=> e <=> f <=> root 
	// root <=> a <=> prev <=> move <=> b <=> c <=> d <=> e <=> f <=> root 
	Vertex *t = move->links[LNK][BWD]->links[LNK][FWD];

	move->links[LNK][BWD]->links[LNK][FWD] = t->links[LNK][FWD];
	move->links[LNK][FWD]->links[LNK][BWD] = move->links[LNK][BWD];
	 
	t->links[LNK][BWD] = prev;
	t->links[LNK][FWD] = prev->links[LNK][FWD];
	prev->links[LNK][FWD]->links[LNK][BWD] = t;
	prev->links[LNK][FWD] = t;
}

void switch_cells(Vertex *p, Vertex *q, const int LNK)
{
	// root <=> a <=> P <=> b <=> c <=> d <=> Q <=> e <=> f <=> root 
	// root <=> a <=> Q <=> b <=> c <=> d <=> P <=> e <=> f <=> root 
	if (p->links[LNK][FWD]==q )
	{
		p->links[LNK][BWD]->links[LNK][FWD] = q;
		q->links[LNK][FWD]->links[LNK][BWD] = p;
		p->links[LNK][FWD] = q->links[LNK][FWD];
		q->links[LNK][BWD] = p->links[LNK][BWD];
		p->links[LNK][BWD] = q;
		q->links[LNK][FWD] = p;
	}
	else if ( q->links[LNK][FWD]==p )
	{
		switch_cells(q, p, LNK);
	}
	else
	{
		Vertex *tpbwd = p->links[LNK][BWD];
		Vertex *tpfwd = p->links[LNK][FWD];
		Vertex *tqbwd = q->links[LNK][BWD];
		Vertex *tqfwd = q->links[LNK][FWD];

		p->links[LNK][FWD]->links[LNK][BWD] = q;
		q->links[LNK][FWD]->links[LNK][BWD] = p;
		p->links[LNK][BWD]->links[LNK][FWD] = q;
		q->links[LNK][BWD]->links[LNK][FWD] = p;
		
		p->links[LNK][BWD] = tqbwd;
		q->links[LNK][BWD] = tpbwd;
		p->links[LNK][FWD] = tqfwd;
		q->links[LNK][FWD] = tpfwd;
	}
}

void copy_order(Dllist *dll, const int SRC, const int DEST)
{
	// SRC  : root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// DEST : root <=> c <=> f <=> d <=> a <=> b <=> e <=> root 
	// copy_order : 
	// DEST : root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	Vertex *temp = dll->root;
	for (int i = 0; i <= dll->length; i++)
	{
		temp->links[DEST][FWD] = temp->links[SRC][FWD];
		temp->links[DEST][BWD] = temp->links[SRC][BWD];
		temp = temp->links[SRC][FWD];
	}
	dll->length = dll->length;
}

void add_dll_end_dll(Dllist *dll, Vertex *inf, Vertex *sup, const int len, const int LNK)
{
	sew( dll->root->links[LNK][BWD], inf, LNK );
	sew( sup, dll->root, LNK );
	dll->length += len;
}

void sew( Vertex *left, Vertex *right, const int LNK )
{
	left->links[LNK][FWD] = right;
	right->links[LNK][BWD] = left;
}


/*----------------------------------------------------------------------------------*/
//                                  Vertex functions
/*----------------------------------------------------------------------------------*/

void init_vertex( Vertex *vert, double x, double y, double z )
{
	vert->coords[0] = x;
	vert->coords[1] = y;
	vert->coords[2] = z;

	for(int i=0; i<NBL; i++)
	{
		vert->links[i][BWD] = NULL;	
		vert->links[i][FWD] = NULL;
	}

	vert->zdist = 0;
}

Vertex* create_vertex( double x, double y, double z )
{
	Vertex *new_vert = (Vertex *) malloc(sizeof(Vertex));

	if (new_vert != NULL)
		init_vertex( new_vert, x, y, z );
	else
		printf("Error in create_vertex function\n");

	return new_vert;
}


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
	simp->datation = 0;
	simp->next_stk = NULL;
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

int inside_simplex( Simplex *simp, Vertex *vert )
{
	int ori = 0;
	for (int i = 0; i < 3; i++)
	{
		ori += orientation(simp->sommet[i], simp->sommet[(i+1)%3], vert);
	}
	return ( fabs(ori) == 3 ) ;
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

		// si j==3, on le pousse de 10-13 !

		current->zdist = compute_zdist( tab[j], current );
		next = current->links[STD][FWD];
		if ( tab[j]->candidates->length > 0 && is_superior_vertex( current, tab[j]->candidates->root->links[STD][FWD]) )
			add_begin_dll( tab[j]->candidates, current, STD );
		else
			add_end_dll( tab[j]->candidates, current, STD );
		current = next;
	}
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

	compute_plan(simp);

	stack( grid, new_simp1 );
	stack( grid, new_simp2 );
	stack( grid, simp );
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

int get_vertex_index(Vertex *vert, Simplex *simp)
{
	int i;
	for (i = 0; i < 3; i++)
		if (vert == simp->sommet[i]) break;

	return i;
}

void delauney( Grid *grid, Simplex *simp )
{
	// print_fdp(grid->fdp);
	// printf("\nSimplex to split :\n");
	// print_simplex(simp);

	// Vertex to insert (first candidat)
	Vertex *vert = simp->candidates->root->links[STD][FWD];
	
	// printf("\nChosen vertex :\n");
	// print_vertex(vert);

	vert->zdist = 0;

	rm_begin_dll(simp->candidates, STD);

	// printf("\ncandidates simplex to split :\n");
	// print_dll(simp->candidates, STD);

	// printf("\ncandidates to redistribute bef:\n");
	// print_dll(grid->candidates_to_redistribute, STD);
	// save dllist of candidates for later
	add_dll_end_dll(grid->candidates_to_redistribute, simp->candidates->root->links[STD][FWD], simp->candidates->root->links[STD][BWD], simp->candidates->length, STD );

	// printf("\ncandidates to redistribute aft:\n");
	// print_dll(grid->candidates_to_redistribute, STD);

	// reinit dll
	init_dll(simp->candidates, simp->candidates->root);

	// printf("\ncandidates simplex to split aft init :\n");
	// print_dll(simp->candidates, STD);

	split_in_3(grid, simp, vert);

	Simplex *my_simp = unstack(grid);
	Simplex *new_simp2 = unstack(grid);
	Simplex *new_simp1 = unstack(grid);

	// printf("\nSimplex ori :\n");
	// print_simplex(simp);

	// printf("\nSimplex new 1 :\n");
	// print_simplex(new_simp1);

	// printf("\nSimplex new 2 :\n");
	// print_simplex(new_simp2);


	add_end_array(grid->new, &grid->new_current_size, &grid->new_max_size, my_simp );
	add_end_array(grid->new, &grid->new_current_size, &grid->new_max_size, new_simp1);
	add_end_array(grid->new, &grid->new_current_size, &grid->new_max_size, new_simp2);

	// for (int i = 0; i < grid->new_current_size; i++)
	// {
	// 	print_simplex(grid->new[i]);
	// }

	// redistribute candidates
	redistribute_candidates(grid->candidates_to_redistribute, grid->new, grid->new_current_size, STD );

	// printf("\nSimplex ori :\n");
	// print_simplex(simp);

	// printf("\nSimplex new 1 :\n");
	// print_simplex(new_simp1);

	// printf("\nSimplex new 2 :\n");
	// print_simplex(new_simp2);

	// update fdp and add new triangle to it
	down_heap(grid->fdp, 2, 1);

	insert_in_fdp(grid->fdp, new_simp1);
	insert_in_fdp(grid->fdp, new_simp2);

	// reinit
	init_dll(grid->candidates_to_redistribute, grid->candidates_to_redistribute->root);
	// int n; // = grid->stack_size;
	// for (int i = 0; i < n; i++)
	// {
	// 	unstack(grid);
	// }
	int n = grid->new_current_size;
	for (int i = 0; i < n; i++)
	{
		grid->new[i] = NULL;
		grid->new_current_size--;
	}
	// printf("nnew = %d\n", grid->new_current_size);
	// printf("nstack = %d\n", grid->stack_size);
	// printf("grid->fdp->nb = %d\n", grid->fdp->nb);
	// print_fdp(grid->fdp);
}

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

int is_empty( Simplex *simp )
{
	return 1 - sgn(simp->candidates->length);
}

int is_superior_vertex( Vertex *p1, Vertex *p2 )
{
	return (fabs(p1->zdist) > fabs(p2->zdist));
}

int is_superior_simplex( Simplex *s1, Simplex *s2 )
{
	return is_superior_vertex(s1->candidates->root->links[STD][FWD], s2->candidates->root->links[STD][FWD] );
}

int is_superior_fdp( FDP *fdp, const int a, const int b )
{
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
	fdp->table[fdp->nb + 1] = simp;
	fdp->nb++;
	up_heap(fdp, fdp->nb, fdp->nb / 2);
}

Simplex* extract_max( FDP *fdp )
{
	if (fdp->nb > 0) return fdp->table[1];
	else return NULL;
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

/*----------------------------------------------------------------------------------*/
//                                  Grid
/*----------------------------------------------------------------------------------*/

void init_grid( Grid *grid, Dllist *dll, FDP *fdp, Simplex **tab, const int init_size_new_tab, const int nb_pts )
{
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

	grid->fdp = fdp;

	grid->candidates_to_redistribute = dll;

	grid->top_of_stack = NULL;
	grid->stack_size = 0;

	grid->new = tab;
	grid->new_current_size = 0;
	grid->new_max_size = init_size_new_tab;

	grid->gof = 0;

	insert_in_fdp(grid->fdp, simp[0]);
	insert_in_fdp(grid->fdp, simp[1]);
}

Grid* create_grid( const int nb_pts, const int size_fdp, const int init_size_new_tab )
{
	Grid *new_grid = (Grid *) malloc(sizeof(Grid));
	Dllist *new_dll = create_dll();
	FDP *new_fdp = create_fdp( size_fdp );
	Simplex **new_tab = (Simplex **)malloc(init_size_new_tab*sizeof(Simplex *));

	if (new_grid != NULL && new_dll !=NULL && new_fdp !=NULL && new_tab !=NULL)
		init_grid( new_grid, new_dll, new_fdp, new_tab, init_size_new_tab, nb_pts );
	else
		printf("Error in create_grid function\n");

	return new_grid;
}

/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/


Dllist* create_rd_data_struct( )
{
	Dllist *dll = create_dll();

	for(int i=0; i<NB_VERTEX; i++)
	{
		Vertex *new_vert = create_vertex( randf(), randf(), randf() );
		add_end_dll( dll, new_vert, STD );
	}
	return dll;
}

// TODO
// create_data_struct_from_tab_of_coords()

void remove_data_struct( Dllist *dll )
{
	if (dll != NULL)
	{
		Vertex *del = dll->root->links[STD][FWD];
		Vertex *temp = NULL;

		while (del != dll->root)
		{
			temp = del->links[STD][FWD];

			free(del);
			del->links[STD][FWD] = NULL;
			del->links[STD][BWD] = NULL;

			del = temp;
			dll->length--;
		}
		free(dll->root);
		free(dll);	
		dll = NULL;
	}	
}
