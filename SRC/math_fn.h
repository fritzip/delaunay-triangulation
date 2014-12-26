#ifndef __MATH_FN_H__
#define __MATH_FN_H__
/*! \file math_fn.h
 * \author M. Sainlot
 * \date 2014
 */

#include "vertex.h"
#include "simplex.h"
/*----------------------------------------------------------------------------------*/
//                                  Math functions prototypes
/*----------------------------------------------------------------------------------*/

/**
 * @brief Sign of a double
 *
 * @return -1, 0 or 1
 */
int sgn(double const x);

/**
 * @brief Random integer between min and max
 * @details min and max included
 * 
 * @param min 
 * @param max 
 * 
 * @return min \<= randint \<= max
 */
int randn(int const min, int const max);

/**
 * @brief Random double beween 0 and 1
 * @return 0 \< randf \<= 1
 */
double randf();

/**
 * @brief Compute the 2D determinant
 * @details mat here	
 * @param a 
 * @param b 
 * @param c 
 * @param d 
 * @return a*d-c*b
 */
double determinant2d(double const a, double const b, double const c, double const d);

/**
 * @brief Compute the 3D determinant
 * @details mat here
 * 
 * @param p1 first vertex
 * @param p3 second vertex
 * @param p2 third vertex
 * @return signed volume <=> det3D
 */
double determinant3d(Vertex const *p1, Vertex const *p2, Vertex const *p3);

/**
 * @brief Orientation predicate
 * @details Test whether vertex p3 is below, on, or above the p1 - p2 axis in 2D (z ignored)
 * 
 * @param p1 
 * @param p2 
 * @param p3 
 * @return 1 clockwise / -1 counter clockwise / 0 on
 */
int orientation(Vertex const *p1, Vertex const *p2, Vertex const *p3);

/**
 * @brief Compute plan discribed by the 3 vertices of the simplex
 * @details Compute na, nb and nc, the normale coordinates
 * 
 * @param simp 
 */
void compute_plan( Simplex *simp );

/**
 * @brief Vertical distance between the vertex (vert) and the simplex (simp) it is attached to
 * 
 * @param simp 
 * @param vert 
 * 
 * @return zdist
 */
double compute_zdist( Simplex const *simp, Vertex const *vert );

/**
 * @brief Delaunay's condition to flip
 * @details Test if the vertex z is in or out of the circle discribed (and passing) by a, b, c
 * 
 * @param a 
 * @param b 
 * @param c 
 * @param z 
 * @return 1 IN / 0 OUT
 */
int in_circle( Vertex const *a, Vertex const *b, Vertex const *c, Vertex const *z );


#endif