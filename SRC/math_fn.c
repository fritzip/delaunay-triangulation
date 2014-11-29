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

int sgn(double const x)
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

double determinant2d(double const a, double const b, double const c, double const d)
{
	return a*d-c*b;
}

double determinant3d(Vertex const *p1, Vertex const *p2, Vertex const *p3)
{
	return determinant2d (p1->coords[0], p2->coords[0], p1->coords[1], p2->coords[1])
		-  determinant2d (p1->coords[0], p3->coords[0], p1->coords[1], p3->coords[1])
		+  determinant2d (p2->coords[0], p3->coords[0], p2->coords[1], p3->coords[1]);
}

int orientation(Vertex const *p1, Vertex const *p2, Vertex const *p3)
{
	// 1 : sens horaire , -1 : sens trigo
	// (x2-x1)(y3-y1)-(y2-y1)(x3-x1)
	// return -sgn(determinant3d(p1, p2, p3));
	return - sgn( (p2->coords[0] - p1->coords[0]) * (p3->coords[1] - p1->coords[1])
			    - (p2->coords[1] - p1->coords[1]) * (p3->coords[0] - p1->coords[0]) );
}

void compute_plan( Simplex *simp )
{
	simp->na =  ( simp->sommet[1]->coords[1] - simp->sommet[0]->coords[1] ) 
		   *    ( simp->sommet[2]->coords[2] - simp->sommet[0]->coords[2] )
		   -    ( simp->sommet[1]->coords[2] - simp->sommet[0]->coords[2] ) 
		   *    ( simp->sommet[2]->coords[1] - simp->sommet[0]->coords[1] );

	simp->nb =  ( simp->sommet[1]->coords[2] - simp->sommet[0]->coords[2] ) 
		   *    ( simp->sommet[2]->coords[0] - simp->sommet[0]->coords[0] )
		   -    ( simp->sommet[1]->coords[0] - simp->sommet[0]->coords[0] ) 
		   *    ( simp->sommet[2]->coords[2] - simp->sommet[0]->coords[2] );

	simp->nc =  ( simp->sommet[1]->coords[0] - simp->sommet[0]->coords[0] ) 
		   *    ( simp->sommet[2]->coords[1] - simp->sommet[0]->coords[1] )
		   -    ( simp->sommet[1]->coords[1] - simp->sommet[0]->coords[1] ) 
		   *    ( simp->sommet[2]->coords[0] - simp->sommet[0]->coords[0] );
}

double compute_zdist( Simplex const *simp, Vertex const *vert ) //double x, double y )
{
	// mettre equation dans simplex
	// return vert->coords[2] - simp->sommet[0]->coords[2]
	// 	   - (((( simp->sommet[1]->coords[1] - simp->sommet[0]->coords[1] ) 
	// 	   *    ( simp->sommet[2]->coords[2] - simp->sommet[0]->coords[2] )
	// 	   -    ( simp->sommet[1]->coords[2] - simp->sommet[0]->coords[2] ) 
	// 	   *    ( simp->sommet[2]->coords[1] - simp->sommet[0]->coords[1] ))
	// 	   *    (            vert->coords[0] - simp->sommet[0]->coords[0] ))

	// 	   +  ((( simp->sommet[1]->coords[2] - simp->sommet[0]->coords[2] ) 
	// 	   *    ( simp->sommet[2]->coords[0] - simp->sommet[0]->coords[0] )
	// 	   -    ( simp->sommet[1]->coords[0] - simp->sommet[0]->coords[0] ) 
	// 	   *    ( simp->sommet[2]->coords[2] - simp->sommet[0]->coords[2] ))
	// 	   *    (            vert->coords[1] - simp->sommet[0]->coords[1] )))
		 
	// 	   /   (( simp->sommet[1]->coords[0] - simp->sommet[0]->coords[0] ) 
	// 	   *    ( simp->sommet[2]->coords[1] - simp->sommet[0]->coords[1] )
	// 	   -    ( simp->sommet[1]->coords[1] - simp->sommet[0]->coords[1] ) 
	// 	   *    ( simp->sommet[2]->coords[0] - simp->sommet[0]->coords[0] ));

	return vert->coords[2] - simp->sommet[0]->coords[2]
		   + ((( simp->na * ( vert->coords[0] - simp->sommet[0]->coords[0] ))
		     + ( simp->nb * ( vert->coords[1] - simp->sommet[0]->coords[1] )) )
		     /   simp->nc );
}