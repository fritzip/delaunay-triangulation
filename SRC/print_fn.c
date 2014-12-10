/*! \file print_fn.c
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "print_fn.h"

/*----------------------------------------------------------------------------------*/
//                                  Print functions
/*----------------------------------------------------------------------------------*/

void print_lnk(int const LNK)
{
	switch (LNK)
	{
		case STD: printf("  Standard\n");
			break;
		default : printf("  Nothing to print\n");
			break;
	}
}


void print_info(Dllist const *dll, int const LNK)
{
	printf("=====================\n");
	print_lnk(LNK);
	printf("  Length : %d \n", dll->length);
	printf("=====================\n");
}


void print_vertex(Vertex const *vert)
{
	printf("(V) coords = ");
	for (int i = 0; i < DIM; i++)
	{
		printf("%.3f ", vert->coords[i]);
	}
	printf(" ||  zdist = %.3f", vert->zdist);
	printf("\n");
	// printf("(V) zdist = %.2f\n", vert->zdist);
}

void print_simplex(Simplex const *simp)
{	
	printf("=== SIMPLEX ===\n");
	printf("(S) sommets =\n");
	for (int i = 0; i < 3; i++)
	{
		printf("(S)\t");
		print_vertex(simp->sommet[i]);
	}
	printf("(S) voisins = ");
	for (int i = 0; i < 3; i++)
	{
		if (simp->voisin[i] != NULL)	
		{
			printf("%d ", i);
		}
		else
		{
			printf("NULL ");
		}
	}
	printf("\n");
	printf("(%3d) zdist = ", simp->candidates->length);
	Vertex *temp = simp->candidates->root->links[STD][FWD];
	for (int i = 0; i < simp->candidates->length; i++)
	{
		// print_vertex(temp);
		printf("%.3f ", temp->zdist);
		temp = temp->links[STD][FWD];
	}
	printf("\n===============\n");
}

void print_fdp(FDP const *fdp )
{
	printf("===== FDP =====\n");
	Vertex *vert = NULL;
	int n;
	for (int i = 1; i <= fdp->nb; i++)
	{
		n = fdp->table[i]->candidates->length;
		printf("( %4d | %3d) ", i, n );
		vert = fdp->table[i]->candidates->root->links[STD][FWD];
		for (int j = 0; j < n; j++)
		{
			printf("%.3f ", vert->zdist);
			vert = vert->links[STD][FWD];
		}
		printf("\n");
	}
	printf("===============\n");
}

void print_dll(Dllist const *dll, int const LNK)
{
	Vertex *temp = dll->root->links[LNK][FWD];
	int n = 0;

	print_info(dll, LNK);
	
	while ( n++ < dll->length) //temp != NULL)
	{
		print_vertex(temp); 
		temp = temp->links[LNK][FWD];
	}
	printf("\n");
}


void print_dll_full(Dllist const *dll, int const LNK)
{
	Vertex *tempfw = dll->root;
	Vertex *tempbw = dll->root;

	print_info(dll, LNK);
	
	for (int i = 0; i <= dll->length+ 1; ++i)
	{
		print_vertex(tempfw); 
		tempfw = tempfw->links[LNK][FWD];
	}
	printf("\n");
	for (int i = 0; i <= dll->length+ 1; ++i)
	{
		print_vertex(tempbw); 
		tempbw = tempbw->links[LNK][BWD];
	}
	printf("\n");

}
