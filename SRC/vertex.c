/*! \file vertex.c
 * \author M. Sainlot
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vertex.h"
#include "math_fn.h"

/*----------------------------------------------------------------------------------*/
//                                  Vertex functions
/*----------------------------------------------------------------------------------*/

void init_vertex( Vertex *vert, double x, double y, double z )
{
	vert->coords[0] = x;
	vert->coords[1] = y;
	vert->coords[2] = z;

	for(int i=0; i<NB_LNK; i++)
	{
		vert->links[i][BWD] = NULL;	
		vert->links[i][FWD] = NULL;
	}

	vert->zdist = 0;
}


Vertex* create_vertex( double x, double y, double z )
{
	Vertex *new_vert = (Vertex *) malloc(sizeof(Vertex));

	if (new_vert != NULL)
		init_vertex( new_vert, x, y, z );
	else
		printf("Error in create_vertex function\n");

	return new_vert;
}


int is_superior_vertex( Vertex *p1, Vertex *p2 )
{
	return (fabs(p1->zdist) > fabs(p2->zdist));
}

