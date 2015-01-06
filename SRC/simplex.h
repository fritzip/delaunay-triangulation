#ifndef __SIMPLEX_H__
#define __SIMPLEX_H__
/*! \file simplex.h
 * \author M. Sainlot
 * \date 2014
 */

#include "dll.h"
#include "vertex.h"

typedef struct Simplex
{
	Vertex *sommet[3];
	struct Simplex *voisin[3];
	struct Simplex *next_in_stack[2];
	Dllist *candidates;
	
	double na;
	double nb;
	double nc;

	int index_in_fdp;
	
} Simplex ;


/*----------------------------------------------------------------------------------*/
//                                  Simplex functions prototypes
/*----------------------------------------------------------------------------------*/

/**
 * @brief Initialize simplex
 * @details Put neighbors to NULL, set the verticies and the doubly linked list of candidates
 * 
 * @param simp The simplex to initialize
 * @param v0 first vertex sommet[0]
 * @param v1 second vertex sommet[1]
 * @param v2 third vertex sommet[2]
 * @param candidates list
 */
void init_simplex( Simplex *simp, Vertex *v0, Vertex *v1, Vertex *v2, Dllist *candidates ) ;

/**
 * @brief Create a new simplex
 * @details Do a malloc and call init_simplex()
 * 
 * @param v0 first vertex sommet[0]
 * @param v1 second vertex sommet[1]
 * @param v2 third vertex sommet[2]
 * @return The new simplex created
 */
Simplex* create_simplex( Vertex *v0, Vertex *v1, Vertex *v2 ) ;

Simplex* create_empty_simplex();

/**
 * @brief Test whether a point (vert) is inside or outside of the simplex (simp)
 * 
 * @param simp 
 * @param vert 
 * 
 * @return 1 INSIDE / 0 OUTSIDE
 */
int inside_simplex( Simplex *simp, Vertex *vert) ;

/**
 * @brief Test whether the candidates doubly linked list is empty or not
 * 
 * @param simp
 * @return 1 EMPTY / 0 NOT EMPTY
 */
int is_empty( Simplex *simp ) ;

/**
 * @brief Compare simplex by zdist
 * @details Call is_superior_vertex
 * 
 * @param s1 
 * @param s2 
 * 
 * @return s1 > s2
 */
int is_superior_simplex( Simplex *s1, Simplex *s2 ) ;

/**
 * @brief Find the index of a vertex (vert) in a simplex (simp)
 * @details Iterate through sommet[3]
 * 
 * @param vert 
 * @param simp 
 * 
 * @return 0, 1, or 2
 */
int get_vertex_index(Vertex *vert, Simplex *simp) ;

/**
 * @brief Find the index of a simplex (neighbor) as a neighbor of another simplex (simp)
 * @details Iterate through voisin[3]
 * 
 * @param neighbor	
 * @param simp 
 * 
 * @return the index i where simp->voisin[i] == neighbor 
 */
int get_neighbor_index( Simplex *neighbor, Simplex *simp) ;

#endif