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
	double coords[DIM];
	struct vertex *links[NBL][2];
} Vertex ;

typedef struct dllist
{
	int length[NBL];
	int up2date[NBL];
	Vertex *root;
} Dllist ;

typedef struct simplex
{
	Vertex *sommet[3];
	struct simplex *voisin[3];
	Dllist *pts;
} Simplex ;

typedef struct fdprior
{
	Simplex **table;
	// int *table;
	int nb;
} FDP ;

typedef struct grid
{
	FDP *fdp;
	double gof;
} Grid ;

/*----------------------------------------------------------------------------------*/
//                                  DLL functions prototypes
/*----------------------------------------------------------------------------------*/

void init_dll( Dllist *dll ) ;

Dllist* create_dll( void ) ;

void add_end_dll( Dllist *dll, Vertex *vert, int const LNK ) ;

void add_begin_dll( Dllist *dll, Vertex *vert, int const LNK ) ;

void insert_after( Dllist *dll, Vertex *prev, Vertex *ins, int const LNK ) ;

void rm_after( Dllist *dll, Vertex *prev, int const LNK ) ;

void insert_btw( Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, int const LNK ) ;

void rm_end_dll( Dllist *dll, int const LNK ) ;

void rm_begin_dll( Dllist *dll, int const LNK ) ;

void copy_order( Dllist *dll, int const SRC, int const DEST ) ;

void move_after( Vertex *p, Vertex *q, int const LNK ) ;

void swich_cells( Vertex *p, Vertex *q, int const LNK ) ;


/*----------------------------------------------------------------------------------*/
//                                  Vertex functions prototypes
/*----------------------------------------------------------------------------------*/

void init_vert( Vertex *vert, double x, double y, double z );

Vertex* create_vert( double x, double y, double z );


/*----------------------------------------------------------------------------------*/
//                                  Simplex functions prototypes
/*----------------------------------------------------------------------------------*/

void init_simplex(Simplex *simp, Vertex *v0, Vertex *v1, Vertex *v2 );

Simplex* create_simplex( Vertex *v0, Vertex *v1, Vertex *v2 );

int inside_simplex( Simplex *simp, Vertex *vert);

/*----------------------------------------------------------------------------------*/
//                                  Priority Queue functions prototypes
/*----------------------------------------------------------------------------------*/

void init_fdp( FDP *fdp, int size );

FDP* create_fdp( int size );

void switch_cells_fdp(FDP *fdp, int const a, int const b);

int get_number_of_sons(int const i, int const n);

int is_superior(FDP *fdp, int const a, int const b);

void up_heap(FDP *fdp, int son, int father);

void down_heap(FDP *fdp, int son, int father); 

void insert_in_fdp(FDP *fdp, Simplex *simp);

Simplex* extract_max(FDP *fdp);

void heap_sort(FDP *fdp);

/*----------------------------------------------------------------------------------*/
//                                  Grid prototypes
/*----------------------------------------------------------------------------------*/

void init_grid( Grid *grid, int nb_pts, int size );

Grid* create_grid( int nb_pts, int size );


/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

void inittab(Dllist *dll);

void init_links(Dllist *dll, int const LNK);

Dllist* create_rd_data_struct();

void remove_data_struct(Dllist *dll);

#endif