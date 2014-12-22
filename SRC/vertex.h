#ifndef __VERTEX_H__
#define __VERTEX_H__
/*! \file vertex.h
 * \author M. Sainlot
 * \date 2014
 */

#include "ressources.h"
#include "pgm.h"

typedef struct vertex
{
	double coords[DIM];
	struct vertex *links[NBL][2];
	double zdist;
} Vertex ;


/*----------------------------------------------------------------------------------*/
//                                  Vertex functions prototypes
/*----------------------------------------------------------------------------------*/

void init_vertex( Vertex *vert, double x, double y, double z ) ;

Vertex* create_vertex( double x, double y, double z ) ;

void init_vertex_img_based( Vertex *vert, const PGMData *pic ) ;

Vertex* create_vertex_img_based( const PGMData *pic ) ;

int is_superior_vertex( Vertex *p1, Vertex *p2 ) ;

#endif