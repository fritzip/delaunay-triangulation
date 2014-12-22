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

int sgn(double const x);

int randn(int const min, int const max);

double randf();

double determinant2d(double const a, double const b, double const c, double const d);

double determinant3d(Vertex const *p1, Vertex const *p2, Vertex const *p3);

int orientation(Vertex const *p1, Vertex const *p2, Vertex const *p3);

void compute_plan( Simplex *simp );

double compute_zdist( Simplex const *simp, Vertex const *vert );

int in_circle( Vertex const *a, Vertex const *b, Vertex const *c, Vertex const *z );


#endif