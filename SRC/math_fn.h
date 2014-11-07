#ifndef __MATH_H__
#define __MATH_H__
/*! \file math_fn.h
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include "primitives.h"
/*----------------------------------------------------------------------------------*/
//                                  Math functions prototypes
/*----------------------------------------------------------------------------------*/

int sgn(int const x);

int randn(int const min, int const max);

int determinant2d(int const a, int const b, int const c, int const d);

int determinant3d(Vertex const *p1, Vertex const *p2, Vertex const *p3);

int orientation(Vertex const *p1, Vertex const *p2, Vertex const *p3);

#endif