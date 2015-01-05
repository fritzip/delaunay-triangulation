#ifndef __GRID_H__
#define __GRID_H__
/*! \file triangulation.h
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
 * 
 * @param fdp : priority queue of simplex to know which one to treat next
 * @param candidates_to_redistrubute : a doubly linked list which stores during one delaunay round
 * (ie. insertion of one point and computation of any changes required) all the candidates which will be redistribute
 * between all the recently created and/or modified simplices.
 * @param top_of_stack[NB_STK] : A pointer to the first simplex to unstack (ie. the last added). Each simplex have a pointer to the 
 * next one (next_in_stack)
 * @param stack_size[NB_STK] : Number of simplex currently in stack.
 * @param stack_max_size[NB_STK] : 
 */
typedef struct Grid
{
	FDP *fdp;
	Simplex **table_of_simp;
	int nb_simp;

	Dllist *candidates_to_redistribute;
	
	Simplex *top_of_stack[NB_STK];
	int stack_size[NB_STK];
	int stack_max_size[NB_STK];
	
	int nb_vertex_inserted;
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
 * @param pic  The heightmap picture which define the z coordinate of every vertex. If NULL, z is random.
 * @return The new grid created and initialized.
 */
Grid* create_grid( const int nb_pts, const int size_fdp, const PGMData *pic, const int ZUNIF);


Simplex* add_new_simp( Grid *grid, Vertex *v0, Vertex *v1, Vertex *v2 );

/**
 * @brief Redistributes candidates point to the corresponding simplex (in stack NEW)
 * @details At the end of a Delaunay iteration, all new or modified simplex are in a stack (of link NEW) and the candidates are stored in the dll candidates_to_redistribute.  
 * 
 * @param grid The grid which contain candidates_to_redistribute and the stack of link NEW.
 */
void redistribute_candidates( Grid *grid );

/**
 * @brief Stack a simplex at the top of stack given a link.
 * 
 * @param grid top_of_stack is a parameter of grid, therefore it is necessary to give it as argument.
 * @param simp the simplex to stack (at the top).
 * @param LNK the link of the stack.
 */
void stack( Grid *grid, Simplex *simp, int LNK ) ;

/**
 * @brief Unstack the simplex at the top of stack. 
 * 
 * @param grid top_of_stack is a parameter of grid, therefore it is necessary to give it as argument.
 * @param LNK the link of the stack.
 * @return The former simplex at the top of the stack. (Second become first)
 */
Simplex* unstack( Grid *grid, int LNK ) ;

/**
 * @brief Split a simplex in 3 given a vertex inside this simplex.
 * 
 * @param grid The grid is required for stacking the new simplex and inserting them in fdp.
 * @param simp Simplex to split.
 * @param vert Vertex inside the simplex to split.
 */
void split_in_3(Grid *grid, Simplex *simp, Vertex *vert) ;

/**
 * @brief Flips the diagonal of the convex quad formed by two neighbors simplices. 
 * 
 * @param grid The grid is required for inserting them in fdp
 * @param current The first simplex
 * @param ind_vert The index of the first simplex's vertex not included in the diagonal before flip. 
 * @param opp The second simplex
 * @param ind_opp The index of the second simplex's vertex not included in the diagonal before flip.
 */
void flip(Grid *grid, Simplex *current, int ind_vert, Simplex *opp, int ind_opp);

/**
 * @brief Delaunay algorithm
 * @details Take the first candidate to insert, insert it (split in 3) and check whether it is required or not to flip the diagonal around that point.
 * 
 */
void delauney( Grid *grid ) ;


#endif