#ifndef __FDP_H__
#define __FDP_H__
/*! \file fdp.h
 * \author M. Sainlot
 * \date 2014
 */

#include "simplex.h"

/**
 * @brief The priority queue structure
 * @detail Order rule : zdist of vertex.
 * 
 * @param table : the array in which are stored the non empty simplex  
 */
typedef struct FDP
{
	Simplex **table;
	int nb;
	int nb_comp;
} FDP ;


/*----------------------------------------------------------------------------------*/
//                                  Priority Queue functions prototypes
/*----------------------------------------------------------------------------------*/

/**
 * @brief Initialize all attributes of fdp
 */
void init_fdp( FDP *fdp, Simplex **tab, int size ) ;

/**
 * @brief Allocate a priority queue
 * 
 * @param size : The size of the priority queue's array
 * @return The fdp created
 */
FDP* create_fdp( int size ) ;

/**
 * @brief Switch to element of the fdp given two index
 * @details Used after is_superior has returned True
 * 
 * @param fdp The priority queue
 * @param a The index of the first element
 * @param b The index of the second element
 */
void switch_cells_fdp( FDP *fdp, const int a, const int b ) ;

/**
 * @brief How many sons have a father (0, 1, 2) ?
 * 
 * @param i The index of the father
 * @param n The number of element in the fdp
 * 
 * @return 0, 1 or 2 (according to the number of son(s))
 */
int get_number_of_sons( const int i, const int n ) ;

/**
 * @brief Test whether an element is superior to another in the fdp (given an order rule)
 * @details Call is_superior_simplex()
 * 
 * @param fdp The priority queue
 * @param a The index of the first element 
 * @param b The index of the second element
 * @return element[a] > element[b]
 */
int is_superior( FDP *fdp, const int a, const int b ) ;

/**
 * @brief Up heap fonction
 * @details Switch elements if a son is greater than his father (given an order rule)
 * 
 * @param fdp The priority queue
 * @param son index of the son
 * @param father index of the father
 */
void up_heap( FDP *fdp, int son, int father ) ;

/**
 * @brief Down heap function
 * @details Switch elements if a father is smaller than his son (given an order rule)
 * 
 * @param fdp The priority queue
 * @param son index of the son
 * @param father index of the father
 */
void down_heap( FDP *fdp, int son, int father) ;  

/**
 * @brief Insert a new simplex in the priority queue
 * @details Insert it at the end and call up_heap() 
 * 
 * @param fdp The priority queue
 * @param simp The nez simplex to insert
 */
void insert_in_fdp( FDP *fdp, Simplex *simp ) ;

/**
 * @brief Remove a simplex from fdp
 * @details Only used when the simplex is empty
 * 
 * @param fdp The priority queue
 * @param simp The simplex to remove
 */
void rm_of_fdp( FDP *fdp, Simplex *simp ) ;

#endif