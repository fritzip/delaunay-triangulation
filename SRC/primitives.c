/*! \file primitives.c
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "primitives.h"
#include "geoalgo.h"

/*----------------------------------------------------------------------------------*/
//                                  DLL Functions
/*----------------------------------------------------------------------------------*/

void init_dll(Dllist *dll)
{
	int coords[DIM] = {0};
	dll->root = create_vert(coords);

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

void init_vert( Vertex *vert, int coords[DIM] )
{
	for(int i=0; i<DIM; i++)
	{
		vert->coords[i] = coords[i];
	}

	for(int i=0; i<NBL; i++)
	{
		vert->links[i][BWD] = NULL;	
		vert->links[i][FWD] = NULL;
	}
}

Vertex* create_vert( int coords[DIM] )
{
	Vertex *new_vert = (Vertex *) malloc(sizeof(Vertex));
	if (new_vert != NULL)
	{
		init_vert( new_vert, coords );
	}
	return new_vert;
}


/*----------------------------------------------------------------------------------*/
//                                  Simplex functions
/*----------------------------------------------------------------------------------*/

void init_simplex(Simplex *simp, Vertex *vert[3])
{
	for(int i=0; i<3; i++)
	{
		simp->sommet[i] = vert[i];
		simp->voisin[i] = NULL;
	}
	simp->pts = create_dll();
}

Simplex* create_simplex(Vertex *vert[3])
{
	Simplex *new_simp = (Simplex *) malloc(sizeof(Simplex));
	if (new_simp != NULL)
	{
		init_simplex( new_simp, vert);
	}
	return new_simp;
}


/*----------------------------------------------------------------------------------*/
//                                  Priority Queue
/*----------------------------------------------------------------------------------*/

void init_fdp(FDP *fdp)
{
	int *new_tab = (int*)malloc((NB_SIMPLEX + 1)*sizeof(int));
	if (new_tab != NULL)
	{
		fdp->table = new_tab;
		fdp->nb = 0;
	}	
}

FDP* create_fdp()
{
	FDP *new_fdp = (FDP *) malloc(sizeof(FDP));
	if (new_fdp != NULL)
	{
		init_fdp( new_fdp );
	}
	return new_fdp;
}

void switch_cells_fdp(FDP *fdp, int const a, int const b)
{
	// Simplex *c = fdp->table[a];
	int c = fdp->table[a];
	fdp->table[a] = fdp->table[b];
	fdp->table[b] = c;
}

int get_number_of_sons(int const i, int const n)
{
	int nb_leafs = (n + 1) / 2;
	if ( i > n - nb_leafs) return 0;
	else if (i == n - nb_leafs && !(n % 2)) return 1;
	else return 2;
}

int is_superior(FDP *fdp, int const a, int const b)
{
	// return (fdp->table[a]->pts->root->links[STD][FWD]->coords[0] > fdp->table[b]->pts->root->links[STD][FWD]->coords[0]);
	return (fdp->table[a] > fdp->table[b]);
}

void up_heap(FDP *fdp, int son, int father)
{
	// printf("%d, %d\n", son, father);
	while(father > 0)
	{
		// printf("%d, %d, %d, %d, %d\n", is_superior(fdp, son, father), son, father, fdp->table[son], fdp->table[father]);
		if (is_superior(fdp, son, father))
		{
			switch_cells_fdp(fdp, son, father);
			son = father;
			father = son/2;
		}
		else break;
	}
}

void down_heap(FDP *fdp, int son, int father)
{
	int LOOP = 1;

	while(LOOP)
	{
		switch( get_number_of_sons(father, fdp->nb) )
		{
			case 2: 
				son += is_superior(fdp, son+1, son);
				if ( is_superior(fdp, son, father) ) switch_cells_fdp(fdp, son, father);
				break;
			case 1: 
				if ( is_superior(fdp, son, father) ) switch_cells_fdp(fdp, son, father);
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

int check_tas_binaire(FDP *fdp, int i )
{
	// printf("check_tas_binaire i = %d, %d, %d\n", i, i*2, i*2+1);
	switch( get_number_of_sons(i, fdp->nb) )
	{
		case 2: 
			if (is_superior(fdp, i, i*2) && is_superior(fdp, i, i*2+1))
			{
				// printf("OUI : %d > %d ET %d > %d\n", fdp->table[i], fdp->table[i*2], fdp->table[i], fdp->table[i*2+1]);
				if ( !check_tas_binaire(fdp, i*2) ) return 0;
				if ( !check_tas_binaire(fdp, i*2+1) ) return 0;
			}
			else return 0;
			break;
		case 1: 
			if ( !is_superior(fdp, i, i*2) )
				return 0;
			// printf("FIN BRANCHE OKAY 1\n");
			break;
		case 0: 
			// printf("FIN BRANCHE OKAY 0\n");
			break;
		default: printf("Error in function check_tas_binaire\n");
			break;
	}
	return 1;
}

void insert_in_fdp(FDP *fdp, int simp)
{
	fdp->table[fdp->nb + 1] = simp;
	fdp->nb++;
	up_heap(fdp, fdp->nb, fdp->nb / 2);
}

int extract_max(FDP *fdp)
{
	if (fdp->nb > 0) return fdp->table[1];
	else return 9999;
}

void heap_sort(FDP *fdp)
{
	int son, father;

	for(int i = 2; i <= fdp->nb; i++)
	{
		son = i;
		father = son/2;
		up_heap(fdp, son, father);
	}

	// printf("is tas binaire = %d\n", check_tas_binaire(fdp, 1));

	while(fdp->nb > 1)
	{
		// printf("nb = %d\n", fdp->nb);
		// printf("switch_cells\n");
		switch_cells_fdp(fdp, 1, fdp->nb);

		fdp->nb--;

		// pui = 1;
		// for (int i = 1; i <= 20; i++)
		// {
		// 	printf("%d ", fdp->table[i]);
		// 	if (i == fdp->nb) printf("\n\t=>");
		// 	if ((i+1) == (int)pow(2,pui) && i < fdp->nb)
		// 	{
		// 		printf("\n");
		// 		pui++;
		// 	}
		// }
		// printf("\n");


		down_heap(fdp, 2, 1);


		// pui = 1;
		// for (int i = 1; i <= 20; i++)
		// {
		// 	printf("%d ", fdp->table[i]);
		// 	if (i == fdp->nb) printf("\n\t=>");
		// 	if ((i+1) == (int)pow(2,pui) && i < fdp->nb)
		// 	{
		// 		printf("\n");
		// 		pui++;
		// 	}
		// }
		// printf("\n");


		// printf("ISBINARYTREE = %d\n", check_tas_binaire(fdp, 1));

	}
}

/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

void init_links(Dllist *dll, int const LNK)
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


Dllist* create_rd_data_struct()
{
	Dllist *dll = create_dll();
	int coords[DIM];

	for(int i=0; i<NB_VERTEX; i++)
	{
		for (int j = 0; j < DIM; ++j)
		{
			coords[j] = randn( minX+margin, maxX-margin );
		}
		Vertex *new_vert = create_vert( coords );
		add_end_dll( dll, new_vert, STD );
	}
	dll->up2date[STD] = 1;
	return dll;
}

// TODO
// create_data_struct_from_tab_of_coords()

void remove_data_struct(Dllist *dll)
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
