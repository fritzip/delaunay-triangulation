#include <stdio.h>
#include <stdlib.h>

#include "math_fn.h"

/*----------------------------------------------------------------------------------*/
//                                  Geometry functions
/*----------------------------------------------------------------------------------*/

int sgn(int x)
{
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

int randn(int min, int max)
{
	return min+rand()%(max-min+1);
}

int determinant2d(int a, int b, int c, int d)
{
	return a*d-c*b;
}

int determinant3d(Vertex* p1, Vertex* p2, Vertex* p3)
{
	return determinant2d (p1->coords[0], p2->coords[0], p1->coords[1], p2->coords[1])
			-determinant2d (p1->coords[0], p3->coords[0], p1->coords[1], p3->coords[1])
			+determinant2d (p2->coords[0], p3->coords[0], p2->coords[1], p3->coords[1]);
}

int orientation(Vertex* p1, Vertex* p2, Vertex* p3)
{
	// 1 : sens horaire , -1 : sens trigo
	return -sgn(determinant3d(p1, p2, p3));
}
