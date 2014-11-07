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

int determinant2d(int const a, int const b, int const c, int const d)
{
	return a*d-c*b;
}

int determinant3d(Vertex const *p1, Vertex const *p2, Vertex const *p3)
{
	return determinant2d (p1->coords[0], p2->coords[0], p1->coords[1], p2->coords[1])
			-determinant2d (p1->coords[0], p3->coords[0], p1->coords[1], p3->coords[1])
			+determinant2d (p2->coords[0], p3->coords[0], p2->coords[1], p3->coords[1]);
}

int orientation(Vertex const *p1, Vertex const *p2, Vertex const *p3)
{
	// 1 : sens horaire , -1 : sens trigo
	return -sgn(determinant3d(p1, p2, p3));
}
