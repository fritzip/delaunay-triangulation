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

	for(int i=0; i<NBL; i++)
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

void init_vertex_img_based( Vertex *vert, const PGMData *pic )
{
	double x = randf();
	double y = randf();
	double z;

	int sizex = pic->row;
	int sizey = pic->col;
	int xpic = floor(x*sizex);
	int ypic = floor(y*sizey);
	double difx = (x*sizex)-xpic-0.5;
	double dify = (y*sizey)-ypic-0.5;
	int sensx = sgn(difx);
	int sensy = sgn(dify);

	// printf("sizex = %d, sizey = %d\n", sizex, sizey);
	// printf("difx = %f, dify = %f\n", difx, dify);
	// printf("xpic = %d, ypic = %d\n", xpic, ypic);

	if (xpic <= 0 || xpic >= pic->row - 1 || ypic <=0 || ypic >= pic->col - 1)
	{
		// printf("fup\n");
		z = (double) pic->matrix[xpic][ypic]/255;
	}
	else
	{
		int xy = pic->matrix[xpic][ypic];
		int xul = pic->matrix[xpic+sensx][ypic];
		int yul = pic->matrix[xpic][ypic+sensy];
		z = ( (difx*sensx*xul + (1-difx*sensx)*xy) + (dify*sensy*yul + (1-dify*sensy)*xy) ) / (2*255);
	}

	// printf("z = %.4f\n", z);

	vert->coords[0] = x;
	vert->coords[1] = y;	
	vert->coords[2] = z;

	for(int i=0; i<NBL; i++)
	{
		vert->links[i][BWD] = NULL;	
		vert->links[i][FWD] = NULL;
	}

	vert->zdist = 0;
}

Vertex* create_vertex_img_based( const PGMData *pic )
{
	Vertex *new_vert = (Vertex *) malloc(sizeof(Vertex));

	if (new_vert != NULL)
		init_vertex_img_based( new_vert, pic);
	else
		printf("Error in create_vertex_img_based function\n");

	return new_vert;
}

int is_superior_vertex( Vertex *p1, Vertex *p2 )
{
	return (fabs(p1->zdist) > fabs(p2->zdist));
}

