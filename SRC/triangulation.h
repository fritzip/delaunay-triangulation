#ifndef __GRID_H__
#define __GRID_H__
/*! \file primitives.h
 * \author M. Sainlot
 * \date 2014
 */

#include "dll.h"
#include "simplex.h"
#include "vertex.h"
#include "fdp.h"
#include "pgm.h"

/**
 * @brief The grid structure
 * @details [long description]
 * 
 * @param fdp : priority queue of simplex to know which one to treat next
 * @param candidates_to_redistrubute : a doubly linked list which stores during one delaunay round
 * (ie. insertion of one point and computation of any changes required) all the candidates which will be redistribute
 * between all the recently created and/or modified simplices.
 * @param top_of_stack : A pointer to the first simplex to unstack (ie. the last added). Each simplex have a pointer to the 
 * next one (next_in_stack)
 * @param stack_size : Number of simplex currently in stack.
 * @param new : A table of fix but expandable size (thanks to realloc if needed) which stores at every delaunay iteration 
 * the recently created and/or modified simpices in order to iterate through and redistribute candidates in candidates_to_redistrubute
 * @param new_current_size : The current size of the table described above (new). In other terms the number of element currently in the table.
 * @param new_max_size : The maximum number of simplex pointers that you can currently store in new table. 
 * If new_current_size > new_max_size then new is reallocated.
 * @param gof : Goodness of fit. An output condition between others.
 */
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
//                                  Triangulation prototypes
/*----------------------------------------------------------------------------------*/

/**
 * @brief Create the grid and call init_grid()
 * @details Create four vertices (0,0) (0,1) (1,0) (1,1) and two simplices. 
 * Create all the vertices and add them to the corresponding simplex. Initialize all other variable of a grid
 * 
 * @param nb_pts Number of vertex (set by user on command line default in ressources.h NB_VERTEX)
 * @param size_fdp Size of the priority queue, initialized to the maximum number of simplices you can possibly have.
 * @param init_size_new_tab OFFSET (set by user in ressources.h) realloc if superior size needed.
 * @param pic  The heightmap picture which define the z coordinate of every vertex. If NULL, z is random.
 * @return The new grid created and initialized.
 */
Grid* create_grid( const int nb_pts, const int size_fdp, const int init_size_new_tab, const PGMData *pic, const double GOF );

void redistribute_candidates( Dllist *dll, Simplex **tab, const int nb_simp, const int LNK );

void add_end_array( Simplex **tab, int *c_size, int *m_size, Simplex *simp );

void stack( Grid *grid, Simplex *simp ) ;

Simplex* unstack( Grid *grid ) ;

void split_in_3(Grid *grid, Simplex *simp, Vertex *vert) ;

void flip(Grid *grid, Simplex *current, int ind_vert, Simplex *opp, int ind_opp);

void delauney( Grid *grid ) ;


#endif