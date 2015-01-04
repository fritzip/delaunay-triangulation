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
	struct vertex *links[NB_LNK][2];
	double zdist;
} Vertex ;


/*----------------------------------------------------------------------------------*/
//                                  Vertex functions prototypes
/*----------------------------------------------------------------------------------*/
/**
 * @brief Initialize vertex
 * @details Put neighbors to NULL
 * 
 * @param vert The vertex to initialize
 * @param x x coordinate
 * @param y y coordinate
 * @param z z coordinate
 */	
void init_vertex( Vertex *vert, double x, double y, double z ) ;

/**
 * @brief Create a new vertex
 * @details Do a malloc and call init_vertex()
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param z z coordinate
 * @return The new vertex created
 */
Vertex* create_vertex( double x, double y, double z ) ;

/**
 * @brief Initialize a vertex based on an image
 * @details Read the grayscale value of the image
 * 
 * @param vert The vertex to initialize
 * @param pic The input image
 */
void init_vertex_img_based( Vertex *vert, const PGMData *pic ) ;

/**
 * @brief Create a new vertex based on an image
 * @details Do a malloc and call init_vertex_img_based()
 * 
 * @param pic The input image
 * @return The new vertex created
 */
Vertex* create_vertex_img_based( const PGMData *pic ) ;

/**
 * @brief Compare vertices by there zdist
 * @details is p1 > p2 ?
 * 
 * @param p1 first vertex
 * @param p2 second vertex
 * 
 * @return 0 if p1 > p2 else 0
 */
int is_superior_vertex( Vertex *p1, Vertex *p2 ) ;

#endif