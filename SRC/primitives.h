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
	double zdist;
} Vertex ;

typedef struct dllist
{
	int length;
	Vertex *root;
} Dllist ;

typedef struct simplex
{
	Vertex *sommet[3];
	struct simplex *voisin[3];
	struct simplex *next_pile;
	Dllist *candidats;
	unsigned long int datation;
	double na;
	double nb;
	double nc;
} Simplex ;

typedef struct fdprior
{
	Simplex **table;
	int nb;
} FDP ;

typedef struct grid
{
	Dllist *candidats_to_redistribute;
	FDP *fdp;
	double gof;
} Grid ;

/*----------------------------------------------------------------------------------*/
//                                  DLL functions prototypes
/*----------------------------------------------------------------------------------*/

void init_dll( Dllist *dll, Vertex* root ) ;

Dllist* create_dll( ) ;

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

void init_vertex( Vertex *vert, double x, double y, double z ) ;

Vertex* create_vertex( double x, double y, double z ) ;


/*----------------------------------------------------------------------------------*/
//                                  Simplex functions prototypes
/*----------------------------------------------------------------------------------*/

void init_simplex( Simplex *simp, Vertex *v0, Vertex *v1, Vertex *v2, Dllist *candidats ) ;

Simplex* create_simplex( Vertex *v0, Vertex *v1, Vertex *v2 ) ;

int inside_simplex( Simplex *simp, Vertex *vert) ;

void split_in_3( FDP *fdp ) ;


/*----------------------------------------------------------------------------------*/
//                                  Priority Queue functions prototypes
/*----------------------------------------------------------------------------------*/

void init_fdp( FDP *fdp, Simplex **tab, int size ) ;

FDP* create_fdp( int size ) ;

void switch_cells_fdp( FDP *fdp, int const a, int const b ) ;

int get_number_of_sons( int const i, int const n ) ;

int is_empty( Simplex *simp ) ;

int is_superior_vertex( Vertex *p1, Vertex *p2 ) ;

int is_superior_simplex( Simplex *s1, Simplex *s2 ) ;

int is_superior( FDP *fdp, int const a, int const b ) ;

void up_heap( FDP *fdp, int son, int father ) ;

void down_heap( FDP *fdp, int son, int father) ;  

void insert_in_fdp( FDP *fdp, Simplex *simp ) ;

Simplex* extract_max( FDP *fdp ) ;

void heap_sort( FDP *fdp ) ;

/*----------------------------------------------------------------------------------*/
//                                  Grid prototypes
/*----------------------------------------------------------------------------------*/

void init_grid( Grid *grid, Dllist *dll, int nb_pts, int size ) ;

Grid* create_grid( int nb_pts, int size ) ;


/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

Dllist* create_rd_data_struct( ) ;

void remove_data_struct( Dllist *dll ) ;

#endif