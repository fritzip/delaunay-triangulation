#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "ressources.h"

/*----------------------------------------------------------------------------------*/
//                                  Structures
/*----------------------------------------------------------------------------------*/

typedef struct vertex
{
	int coords[DIM];
	struct vertex *links[NBL][2];
} Vertex;

typedef struct
{
	int length[NBL];
	int up2date[NBL];
	Vertex *root;
} Dllist;

/*----------------------------------------------------------------------------------*/
//                                  DLL functions prototypes
/*----------------------------------------------------------------------------------*/

Vertex* create_vert( int coords[DIM] );

Dllist* create_dll(void);

void add_end_dll(Dllist *dll, Vertex *vert, int const LNK);

void add_begin_dll(Dllist *dll, Vertex *vert, int const LNK);

void insert_after(Dllist *dll, Vertex *prev, Vertex *ins, int const LNK);

void rm_after(Dllist *dll, Vertex *prev, int const LNK);

void insert_btw(Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, int const LNK);

void rm_end_dll(Dllist *dll, int const LNK);

void rm_begin_dll(Dllist *dll, Vertex *vert, int const LNK);

void copy_order(Dllist *dll, int const SRC, int const DEST);


/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

void inittab(Dllist *dll);

void init_links(Dllist *dll, int const LNK);

Dllist* create_rd_data_struct();

void remove_data_struct(Dllist *dll);

#endif