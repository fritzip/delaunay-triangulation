#ifndef __FDP_H__
#define __FDP_H__
/*! \file fdp.h
 * \author M. Sainlot
 * \date 2014
 */

#include "simplex.h"

typedef struct FDP
{
	Simplex **table;
	int nb;
	int nb_comp;
} FDP ;


/*----------------------------------------------------------------------------------*/
//                                  Priority Queue functions prototypes
/*----------------------------------------------------------------------------------*/

void init_fdp( FDP *fdp, Simplex **tab, int size ) ;

FDP* create_fdp( int size ) ;

void switch_cells_fdp( FDP *fdp, const int a, const int b ) ;

int get_number_of_sons( const int i, const int n ) ;

int is_superior( FDP *fdp, const int a, const int b ) ;

void up_heap( FDP *fdp, int son, int father ) ;

void down_heap( FDP *fdp, int son, int father) ;  

void insert_in_fdp( FDP *fdp, Simplex *simp ) ;

void rm_of_fdp( FDP *fdp, Simplex *simp ) ;

void heap_sort( FDP *fdp ) ;

#endif