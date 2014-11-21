/*! \file math_fn.c
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "math_fn.h"

/*----------------------------------------------------------------------------------*/
//                                  Math functions
/*----------------------------------------------------------------------------------*/

int sgn(int const x)
{
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

int randn(int const min, int const max)
{
	return min+rand()%(max-min+1);
}

double randf( )
{
	return ((double) rand() / RAND_MAX);
}

int determinant2d(int const a, int const b, int const c, int const d)
{
	return a*d-c*b;
}

int determinant3d(Vertex const *p1, Vertex const *p2, Vertex const *p3)
{
	return determinant2d (p1->coords[0], p2->coords[0], p1->coords[1], p2->coords[1])
		-  determinant2d (p1->coords[0], p3->coords[0], p1->coords[1], p3->coords[1])
		+  determinant2d (p2->coords[0], p3->coords[0], p2->coords[1], p3->coords[1]);
}

int orientation(Vertex const *p1, Vertex const *p2, Vertex const *p3)
{
	// 1 : sens horaire , -1 : sens trigo
	return -sgn(determinant3d(p1, p2, p3));
}

double distancez( Simplex const *s1, double x, double y )
{
	return s1->sommet[0]->coords[2]
		   - (((( s1->sommet[1]->coords[1] - s1->sommet[0]->coords[1] ) 
		   *    ( s1->sommet[2]->coords[2] - s1->sommet[0]->coords[2] )
		   -    ( s1->sommet[1]->coords[2] - s1->sommet[0]->coords[2] ) 
		   *    ( s1->sommet[2]->coords[1] - s1->sommet[0]->coords[1] ))
		   *    (                        x - s1->sommet[0]->coords[0] ))

		   +  ((( s1->sommet[1]->coords[2] - s1->sommet[0]->coords[2] ) 
		   *    ( s1->sommet[2]->coords[0] - s1->sommet[0]->coords[0] )
		   -    ( s1->sommet[1]->coords[0] - s1->sommet[0]->coords[0] ) 
		   *    ( s1->sommet[2]->coords[2] - s1->sommet[0]->coords[2] ))
		   *    (                        y - s1->sommet[0]->coords[1] )))
		 
		   /  (( s1->sommet[1]->coords[0] - s1->sommet[0]->coords[0] ) 
		   *   ( s1->sommet[2]->coords[1] - s1->sommet[0]->coords[1] )
		   -   ( s1->sommet[1]->coords[1] - s1->sommet[0]->coords[1] ) 
		   *   ( s1->sommet[2]->coords[0] - s1->sommet[0]->coords[0] ));
}