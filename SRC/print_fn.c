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
	for (int i = 0; i < DIM; i++)
	{
		printf("%4d", vert->coords[i]);
	}
	printf("\n");
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
