#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__
/*! \file primitives.h
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include "ressources.h"


/*----------------------------------------------------------------------------------*/
//                                  Structures
/*----------------------------------------------------------------------------------*/

struct vertex
{
	int coords[DIM];
	struct vertex *links[NBL][2];
};
typedef struct vertex Vertex;


struct dllist
{
	int length[NBL];
	int up2date[NBL];
	Vertex *root;
};
typedef struct dllist Dllist;


struct simplex
{
	Vertex *sommet[3];
	struct simplex *voisin[3];
	Dllist *pts;
};
typedef struct simplex Simplex;

struct fdprior
{
	Simplex **table;
	// int *table;
	int nb;
};
typedef struct fdprior FDP;

/*----------------------------------------------------------------------------------*/
//                                  DLL functions prototypes
/*----------------------------------------------------------------------------------*/

void init_dll(Dllist *dll);

Dllist* create_dll(void);

void add_end_dll(Dllist *dll, Vertex *vert, int const LNK);

void add_begin_dll(Dllist *dll, Vertex *vert, int const LNK);

void insert_after(Dllist *dll, Vertex *prev, Vertex *ins, int const LNK);

void rm_after(Dllist *dll, Vertex *prev, int const LNK);

void insert_btw(Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, int const LNK);

void rm_end_dll(Dllist *dll, int const LNK);

void rm_begin_dll(Dllist *dll, int const LNK);

void copy_order(Dllist *dll, int const SRC, int const DEST);

void move_after(Vertex *p, Vertex *q, int const LNK);

void swich_cells(Vertex *p, Vertex *q, int const LNK);


/*----------------------------------------------------------------------------------*/
//                                  Vertex functions prototypes
/*----------------------------------------------------------------------------------*/

void init_vert( Vertex *vert, int coords[DIM] );

Vertex* create_vert( int coords[DIM] );


/*----------------------------------------------------------------------------------*/
//                                  Simplex functions prototypes
/*----------------------------------------------------------------------------------*/

void init_simplex(Simplex *simp, Vertex *vert[3]);

Simplex* create_simplex(Vertex *vert[3]);


/*----------------------------------------------------------------------------------*/
//                                  Priority Queue functions prototypes
/*----------------------------------------------------------------------------------*/

void init_fdp(FDP *fdp);

FDP* create_fdp();

void switch_cells_fdp(FDP *fdp, int const a, int const b);

int get_number_of_sons(int const i, int const n);

int is_superior(FDP *fdp, int const a, int const b);

void up_heap(FDP *fdp, int son, int father);

void down_heap(FDP *fdp, int son, int father); 

void insert_in_fdp(FDP *fdp, Simplex *simp);

Simplex* extract_max(FDP *fdp);

void heap_sort(FDP *fdp);

/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

void inittab(Dllist *dll);

void init_links(Dllist *dll, int const LNK);

Dllist* create_rd_data_struct();

void remove_data_struct(Dllist *dll);

#endif