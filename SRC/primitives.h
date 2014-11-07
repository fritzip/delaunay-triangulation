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

typedef struct vertex
{
	int coords[DIM];
	struct vertex *links[NBL][2];
} Vertex;

typedef struct simplex
{
	Vertex *v[3];
	struct simplex *voisins[3];
} Simplex;

typedef struct
{
	int length[NBL];
	int up2date[NBL];
	Vertex *root;
} Dllist;

typedef struct
{
	void *father;
	void *son1;
	void *son2;
} ABR;

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


/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

void inittab(Dllist *dll);

void init_links(Dllist *dll, int const LNK);

Dllist* create_rd_data_struct();

void remove_data_struct(Dllist *dll);

#endif