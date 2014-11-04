#ifndef __MATH_H__
#define __MATH_H__

#include "linked_list.h"
/*----------------------------------------------------------------------------------*/
//                                  Geometry functions prototypes
/*----------------------------------------------------------------------------------*/

int sgn(int x);

int randn(int min, int max);

int determinant2d(int a, int b, int c, int d);

int determinant3d(Vertex* p1, Vertex* p2, Vertex* p3);

int orientation(Vertex* p1, Vertex* p2, Vertex* p3);

#endif