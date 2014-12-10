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
	struct simplex *next_stk;
	Dllist *candidates;
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
	FDP *fdp;

	Dllist *candidates_to_redistribute;
	
	Simplex *top_of_stack;
	int stack_size;
	
	Simplex **new;
	int new_current_size;
	int new_max_size;

	double gof;
} Grid ;

/*----------------------------------------------------------------------------------*/
//                                  DLL functions prototypes
/*----------------------------------------------------------------------------------*/

void init_dll( Dllist *dll, Vertex* root ) ;

Dllist* create_dll( ) ;

void add_end_dll( Dllist *dll, Vertex *vert, const int LNK ) ;

void add_begin_dll( Dllist *dll, Vertex *vert, const int LNK ) ;

void insert_after( Dllist *dll, Vertex *prev, Vertex *ins, const int LNK ) ;

void rm_after( Dllist *dll, Vertex *prev, const int LNK ) ;

void rm_end_dll( Dllist *dll, const int LNK ) ;

void rm_begin_dll( Dllist *dll, const int LNK ) ;

void insert_btw( Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, const int LNK ) ;

void move_after( Vertex *p, Vertex *q, const int LNK ) ;

void swich_cells( Vertex *p, Vertex *q, const int LNK ) ;

void copy_order( Dllist *dll, const int SRC, const int DEST ) ;

void sew( Vertex *left, Vertex *right, const int LNK );

void add_dll_end_dll(Dllist *dll, Vertex *inf, Vertex *sup, const int len, const int LNK);

/*----------------------------------------------------------------------------------*/
//                                  Vertex functions prototypes
/*----------------------------------------------------------------------------------*/

void init_vertex( Vertex *vert, double x, double y, double z ) ;

Vertex* create_vertex( double x, double y, double z ) ;


/*----------------------------------------------------------------------------------*/
//                                  Simplex functions prototypes
/*----------------------------------------------------------------------------------*/

void init_simplex( Simplex *simp, Vertex *v0, Vertex *v1, Vertex *v2, Dllist *candidates ) ;

Simplex* create_simplex( Vertex *v0, Vertex *v1, Vertex *v2 ) ;

int inside_simplex( Simplex *simp, Vertex *vert) ;

void redistribute_candidates( Dllist *dll, Simplex **tab, const int nb_simp, const int LNK );

void stack( Grid *grid, Simplex *simp ) ;

Simplex* unstack( Grid *grid ) ;

void split_in_3(Grid *grid, Simplex *simp, Vertex *vert) ;

void add_end_array( Simplex **tab, int *c_size, int *m_size, Simplex *simp );

void delauney( Grid *grid, Simplex *simp ) ;


/*----------------------------------------------------------------------------------*/
//                                  Priority Queue functions prototypes
/*----------------------------------------------------------------------------------*/

void init_fdp( FDP *fdp, Simplex **tab, int size ) ;

FDP* create_fdp( int size ) ;

void switch_cells_fdp( FDP *fdp, const int a, const int b ) ;

int get_number_of_sons( const int i, const int n ) ;

int is_empty( Simplex *simp ) ;

int is_superior_vertex( Vertex *p1, Vertex *p2 ) ;

int is_superior_simplex( Simplex *s1, Simplex *s2 ) ;

int is_superior( FDP *fdp, const int a, const int b ) ;

void up_heap( FDP *fdp, int son, int father ) ;

void down_heap( FDP *fdp, int son, int father) ;  

void insert_in_fdp( FDP *fdp, Simplex *simp ) ;

Simplex* extract_max( FDP *fdp ) ;

void heap_sort( FDP *fdp ) ;

/*----------------------------------------------------------------------------------*/
//                                  Grid prototypes
/*----------------------------------------------------------------------------------*/

void init_grid( Grid *grid, Dllist *dll, FDP *fdp, Simplex **tab, const int init_size_new_tab, const int nb_pts );

Grid* create_grid( const int nb_pts, const int size_fdp, const int init_size_new_tab );


/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

Dllist* create_rd_data_struct( ) ;

void remove_data_struct( Dllist *dll ) ;

#endif