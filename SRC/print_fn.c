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
		case STD: printf("  Unsorted\n");
			break;
		case LEX: printf("  Lexico\n");
			break;
		case POL: printf("  Polar\n");
			break;
		case GRA: printf("  Graham\n");
			break;
		case JAR: printf("  Jarvis\n");
			break;
		case LXC: printf("  Insertion Lexico\n");
			break;
		case DAC: printf("  Divide & Conquer\n");
			break;
		default : printf("  Nothing to print\n");
			break;
	}
}


void print_info(Dllist const *dll, int const LNK)
{
	printf("=====================\n");
	print_lnk(LNK);
	printf("  Length : %d \n", dll->length[LNK]);
	printf("=====================\n");
}


void print_vertex(Vertex const *vert)
{
	printf("(V) coords = ");
	for (int i = 0; i < DIM; i++)
	{
		printf("%.2f ", vert->coords[i]);
	}
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
	printf("(S) zdist = ");
	Vertex *temp = simp->candidats->root->links[STD][FWD];
	for (int i = 0; i < simp->candidats->length[STD]; i++)
	{
		printf("%.2f ", temp->zdist);
		temp = temp->links[STD][FWD];
	}
	printf("\n===============\n");
}

void print_dll(Dllist const *dll, int const LNK)
{
	Vertex *temp = dll->root->links[LNK][FWD];
	int n = 0;

	print_info(dll, LNK);
	
	while ( n++ < dll->length[LNK] ) //temp != NULL)
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
	
	for (int i = 0; i <= dll->length[LNK] + 1; ++i)
	{
		print_vertex(tempfw); 
		tempfw = tempfw->links[LNK][FWD];
	}
	printf("\n");
	for (int i = 0; i <= dll->length[LNK] + 1; ++i)
	{
		print_vertex(tempbw); 
		tempbw = tempbw->links[LNK][BWD];
	}
	printf("\n");

}
