/*! \file primitives.c
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "primitives.h"
#include "geoalgo.h"
#include "print_fn.h"

/*----------------------------------------------------------------------------------*/
//                                  DLL Functions
/*----------------------------------------------------------------------------------*/

void init_dll(Dllist *dll)
{
	dll->root = create_vert(0.0, 0.0, 0.0);

	for(int i=0; i<NBL; i++)
	{
		dll->root->links[i][BWD] = dll->root;	
		dll->root->links[i][FWD] = dll->root;
		dll->length[i] = 0;
		dll->up2date[i] = 0;
	}
}

Dllist* create_dll(void)
{
	Dllist *new_dll = (Dllist *) malloc(sizeof(Dllist));
	if (new_dll != NULL)
	{
		init_dll(new_dll);
	}
	return new_dll;
}

void add_end_dll(Dllist *dll, Vertex *vert, int const LNK)
{
	// root <=> a <=> b <=> c <=> d      <=>     root
	// root <=> a <=> b <=> c <=> d <=> vert <=> root
	vert->links[LNK][BWD] = dll->root->links[LNK][BWD];
	vert->links[LNK][FWD] = dll->root;
	dll->root->links[LNK][BWD]->links[LNK][FWD] = vert;
	dll->root->links[LNK][BWD] = vert;
	dll->length[LNK]++;
}

void add_begin_dll(Dllist *dll, Vertex *vert, int const LNK)
{
	// root     <=>      a <=> b <=> c <=> d <=> root
	// root <=> vert <=> a <=> b <=> c <=> d <=> root
	vert->links[LNK][BWD] = dll->root; 
	vert->links[LNK][FWD] = dll->root->links[LNK][FWD];
	dll->root->links[LNK][FWD]->links[LNK][BWD] = vert;
	dll->root->links[LNK][FWD] = vert;
	dll->length[LNK]++;
}

void rm_end_dll(Dllist *dll, int const LNK)
{
	// root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> b <=> c <=> d <=> e    <=>    root 
	if ( dll->length[LNK] > 0 )
	{
		Vertex *temp = dll->root->links[LNK][BWD];
		dll->root->links[LNK][BWD]->links[LNK][BWD]->links[LNK][FWD] = dll->root;
		dll->root->links[LNK][BWD] = dll->root->links[LNK][BWD]->links[LNK][BWD];
		temp->links[LNK][FWD] = NULL;
		temp->links[LNK][BWD] = NULL;
		dll->length[LNK]--;
	}
	else
		printf("Cannot remove in empty list\n");
}

void rm_begin_dll(Dllist *dll, int const LNK)
{
	// root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// root    <=>    b <=> c <=> d <=> e <=> f <=> root 
	if ( dll->length[LNK] > 0 )
	{
		Vertex *temp = dll->root->links[LNK][FWD];
		dll->root->links[LNK][FWD]->links[LNK][FWD]->links[LNK][BWD] = dll->root;
		dll->root->links[LNK][FWD] = dll->root->links[LNK][FWD]->links[LNK][FWD];
		temp->links[LNK][FWD] = NULL;
		temp->links[LNK][BWD] = NULL;
		dll->length[LNK]--;
	}
	else
		printf("Cannot remove in empty list\n");
}

void insert_after(Dllist *dll, Vertex *prev, Vertex *ins, int const LNK)
{
	// root <=> a <=> prev     <=>     b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> prev <=> ins <=> b <=> c <=> d <=> e <=> f <=> root 
	ins->links[LNK][FWD] = prev->links[LNK][FWD];
	ins->links[LNK][FWD]->links[LNK][BWD] = ins;
	ins->links[LNK][BWD] = prev;
	prev->links[LNK][FWD] = ins;
	dll->length[LNK]++;
}

void rm_after(Dllist *dll, Vertex *prev, int const LNK)
{
	// root <=> a <=> prev <=> b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> prev    <=>    c <=> d <=> e <=> f <=> root 
	Vertex *rm = prev->links[LNK][FWD];
	rm->links[LNK][FWD]->links[LNK][BWD] = prev;
	rm->links[LNK][BWD]->links[LNK][FWD] = rm->links[LNK][FWD];
	rm->links[LNK][FWD] = NULL;
	rm->links[LNK][BWD] = NULL;
	dll->length[LNK]--;
}	


void insert_btw(Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, int const LNK)
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

void move_after(Vertex *prev, Vertex *move, int const LNK)
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

void switch_cells(Vertex *p, Vertex *q, int const LNK)
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

void copy_order(Dllist *dll, int const SRC, int const DEST)
{
	// SRC  : root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// DEST : root <=> c <=> f <=> d <=> a <=> b <=> e <=> root 
	// copy_order : 
	// DEST : root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	Vertex *temp = dll->root;
	for (int i = 0; i <= dll->length[STD]; i++)
	{
		temp->links[DEST][FWD] = temp->links[SRC][FWD];
		temp->links[DEST][BWD] = temp->links[SRC][BWD];
		temp = temp->links[SRC][FWD];
	}
	dll->length[DEST] = dll->length[SRC];
}


/*----------------------------------------------------------------------------------*/
//                                  Vertex functions
/*----------------------------------------------------------------------------------*/

void init_vert( Vertex *vert, double x, double y, double z )
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

Vertex* create_vert( double x, double y, double z )
{
	Vertex *new_vert = (Vertex *) malloc(sizeof(Vertex));
	if (new_vert != NULL)
	{
		init_vert( new_vert, x, y, z );
	}
	return new_vert;
}


/*----------------------------------------------------------------------------------*/
//                                  Simplex functions
/*----------------------------------------------------------------------------------*/

void init_simplex( Simplex *simp, Vertex *v0, Vertex *v1, Vertex *v2 )
{
	simp->sommet[0] = v0;
	simp->sommet[1] = v1;
	simp->sommet[2] = v2;

	compute_plan(simp);

	for(int i=0; i<3; i++)
		simp->voisin[i] = NULL;

	simp->candidats = create_dll();
	simp->datation = 0;
}

Simplex* create_simplex( Vertex *v0, Vertex *v1, Vertex *v2 )
{
	Simplex *new_simp = (Simplex *) malloc(sizeof(Simplex));
	if (new_simp != NULL)
	{
		init_simplex( new_simp, v0, v1, v2);
	}
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

void split_in_3( FDP *fdp )
{
	Simplex *simp = fdp->table[1];

	Vertex *vert = simp->candidats->root->links[STD][FWD];
	vert->zdist = 0;
	rm_after(simp->candidats, simp->candidats->root, STD);
	Simplex *new_simp1 = create_simplex(vert, simp->sommet[1], simp->sommet[2]);
	Simplex *new_simp2 = create_simplex(vert, simp->sommet[2], simp->sommet[0]);
	simp->sommet[2] = vert;

	new_simp1->voisin[0] = simp->voisin[0];
	new_simp1->voisin[1] = new_simp2;
	new_simp1->voisin[2] = simp;

	new_simp2->voisin[0] = simp->voisin[1];
	new_simp2->voisin[1] = simp;
	new_simp2->voisin[2] = new_simp1;

	simp->voisin[0] = new_simp1;
	simp->voisin[1] = new_simp2;

	simp->datation = 0;

	compute_plan(simp);

	Simplex *tab[3] = {new_simp1, new_simp2, simp};

	int i, j;
	int n = simp->candidats->length[STD];
	Vertex *current = simp->candidats->root->links[STD][FWD];
	Vertex *next = NULL;

	// redistributes candidates through 3 new triangles and update zdist
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < 3; j++)
			if ( inside_simplex( tab[j], current ) ) break;

		// si j==3, on le pousse de 10-13 !

		current->zdist = compute_zdist( tab[j], current );
		
		if ( j == 2 )
			current = current->links[STD][FWD];
		else
		{
			next = current->links[STD][FWD];
			rm_after(tab[2]->candidats, current->links[STD][BWD], STD);

			if ( tab[j]->candidats->length[STD] > 0 && is_superior_vertex( current, tab[j]->candidats->root->links[STD][FWD]) )
				add_begin_dll( tab[j]->candidats, current, STD );
			else
				add_end_dll( tab[j]->candidats, current, STD );
			current = next;
		}
	}

	// put max of candidates in top of dll
	n = 0;
	current = simp->candidats->root->links[STD][FWD];
	Vertex *max = current;
	while (n < simp->candidats->length[STD])
	{
		if ( is_superior_vertex( current, max ) ) max = current;
		current = current->links[STD][FWD];
		n++;
	}
	switch_cells(max, simp->candidats->root->links[STD][FWD], STD);

	// update fdp and add new triangle to it
	down_heap(fdp, 2, 1);

	insert_in_fdp(fdp, tab[0]);
	insert_in_fdp(fdp, tab[1]);
}

/*----------------------------------------------------------------------------------*/
//                                  Priority Queue
/*----------------------------------------------------------------------------------*/

void init_fdp( FDP *fdp, int size )
{
	Simplex **new_tab = (Simplex **)malloc(size*sizeof(Simplex *));
	if (new_tab != NULL)
	{
		fdp->table = new_tab;
		fdp->nb = 0;
	}	
}

FDP* create_fdp( int size )
{
	FDP *new_fdp = (FDP *) malloc(sizeof(FDP));
	if (new_fdp != NULL)
	{
		init_fdp( new_fdp, size );
	}
	return new_fdp;
}

void switch_cells_fdp( FDP *fdp, int const a, int const b )
{
	Simplex *c = fdp->table[a];
	fdp->table[a] = fdp->table[b];
	fdp->table[b] = c;
}

int get_number_of_sons( int const i, int const n )
{
	int nb_leafs = (n + 1) / 2;
	if ( i > n - nb_leafs) return 0;
	else if (i == n - nb_leafs && !(n % 2)) return 1;
	else return 2;
}

int is_empty( Simplex *simp )
{
	return 1 - sgn(simp->candidats->length[STD]);
}

int is_superior_vertex( Vertex *p1, Vertex *p2 )
{
	return (fabs(p1->zdist) > fabs(p2->zdist));
}

int is_superior_simplex( Simplex *s1, Simplex *s2 )
{
	return is_superior_vertex(s1->candidats->root->links[STD][FWD], s2->candidats->root->links[STD][FWD] );
}

int is_superior_fdp( FDP *fdp, int const a, int const b )
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

void init_grid( Grid *grid, int nb_pts, int size )
{
	// 4 corners vertex
	Vertex *ul = create_vert(0.0, 1.0, 0.0);
	Vertex *ur = create_vert(1.0, 1.0, 0.0);
	Vertex *dr = create_vert(1.0, 0.0, 0.0);
	Vertex *dl = create_vert(0.0, 0.0, 0.0);

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
		new_vert = create_vert(randf(), randf(), randf());
		for (j = 0; j < 2; j++)
		{
			if ( inside_simplex( simp[j], new_vert ) )
				break;
		}

		new_vert->zdist = compute_zdist( simp[j], new_vert );

		if ( simp[j]->candidats->length[STD] > 0 && is_superior_vertex( new_vert, simp[j]->candidats->root->links[STD][FWD]) )
			add_begin_dll( simp[j]->candidats, new_vert, STD );
		else
			add_end_dll( simp[j]->candidats, new_vert, STD );
	}
	
	// print_simplex(simp[0]);
	// print_simplex(simp[1]);

	grid->fdp = create_fdp( size );
	insert_in_fdp(grid->fdp, simp[0]);
	insert_in_fdp(grid->fdp, simp[1]);
}

Grid* create_grid( int nb_pts, int size )
{
	Grid *new_grid = (Grid *) malloc(sizeof(Grid));
	if (new_grid != NULL)
	{
		init_grid( new_grid, nb_pts, size );
	}
	return new_grid;
}

/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

void init_links( Dllist *dll, int const LNK )
{
	switch (LNK)
	{
		case LEX: 
			copy_order(dll, STD, LEX);
			sort(dll->root, dll->length[LEX], dll, LEX);
			break;

		case POL: 
			if (dll->up2date[LEX])
			{
				copy_order(dll, LEX, POL);
				sort(dll->root->links[POL][FWD], dll->length[POL]-1, dll, POL);
			}
			else 
			{
				init_links(dll, LEX);
				init_links(dll, POL);
			}
			break;

		case GRA:
			if (dll->up2date[POL])
				graham(dll); 
			else
			{
				init_links(dll, POL);
				init_links(dll, GRA);
			}
			break;

		case JAR:
			if (dll->up2date[POL])
				jarvis(dll); 
			else
			{
				init_links(dll, POL);
				init_links(dll, JAR);
			}
			break;

		case LXC:
			if (dll->up2date[LEX]) 
			{
				lexconvex(dll);
			}
			else
			{
				init_links(dll, LEX);
				init_links(dll, LXC);
			}
			break;

		case DAC:
			if (dll->up2date[LEX]) 
			{
				inittab(dll);
				divac(dll);
			}
			else
			{
				init_links(dll, LEX);
				init_links(dll, DAC);
			}
			break;

		default: printf("Error in init_links function\n");
			break;
	}
}


Dllist* create_rd_data_struct( )
{
	Dllist *dll = create_dll();

	for(int i=0; i<NB_VERTEX; i++)
	{
		Vertex *new_vert = create_vert( randf(), randf(), randf() );
		add_end_dll( dll, new_vert, STD );
	}
	dll->up2date[STD] = 1;
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
			dll->length[STD]--;
		}
		free(dll->root);
		free(dll);	
		dll = NULL;
	}	
}
