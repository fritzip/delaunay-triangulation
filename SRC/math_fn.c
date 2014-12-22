/*! \file math_fn.c
 * \author M. Sainlot
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
	return vert->coords[2] - simp->sommet[0]->coords[2]
		   + ((( simp->na * ( vert->coords[0] - simp->sommet[0]->coords[0] ))
		     + ( simp->nb * ( vert->coords[1] - simp->sommet[0]->coords[1] )) )
		     /   simp->nc );
}


int in_circle (Vertex const *a, Vertex const *b, Vertex const *c, Vertex const *z)
{
	double az_x = a->coords[0] - z->coords[0], az_y = a->coords[1] - z->coords[1];
	double bz_x = b->coords[0] - z->coords[0], bz_y = b->coords[1] - z->coords[1];
	double cz_x = c->coords[0] - z->coords[0], cz_y = c->coords[1] - z->coords[1];

	double det = ((a->coords[0] + z->coords[0])*az_x + (a->coords[1] + z->coords[1])*az_y) * (bz_x*cz_y - bz_y*cz_x) +
				 ((b->coords[0] + z->coords[0])*bz_x + (b->coords[1] + z->coords[1])*bz_y) * (cz_x*az_y - cz_y*az_x) +
				 ((c->coords[0] + z->coords[0])*cz_x + (c->coords[1] + z->coords[1])*cz_y) * (az_x*bz_y - az_y*bz_x) ;

	return (det > 0.0) ? 1 : 0;
}
