/*! \file dll.c
 * \author M. Sainlot
 * \date 2014
 */


#include <stdio.h>
#include <stdlib.h>

#include "dll.h"

/*----------------------------------------------------------------------------------*/
//                                  DLL Functions
/*----------------------------------------------------------------------------------*/

void init_dll(Dllist *dll, Vertex* root)
{
	dll->root = root;

	for(int i=0; i<NBL; i++)
	{
		dll->root->links[i][BWD] = dll->root;	
		dll->root->links[i][FWD] = dll->root;
		dll->length = 0;
	}
}

Dllist* create_dll( )
{
	Dllist *new_dll = (Dllist *) malloc(sizeof(Dllist));
	Vertex *new_vert = create_vertex(0.0, 0.0, 0.0);

	if (new_dll != NULL && new_vert != NULL)
		init_dll(new_dll, new_vert);
	else
		printf("Error in create_dll function\n");

	return new_dll;
}

void add_end_dll(Dllist *dll, Vertex *vert, const int LNK)
{
	// root <=> a <=> b <=> c <=> d      <=>     root
	// root <=> a <=> b <=> c <=> d <=> vert <=> root
	vert->links[LNK][BWD] = dll->root->links[LNK][BWD];
	vert->links[LNK][FWD] = dll->root;
	dll->root->links[LNK][BWD]->links[LNK][FWD] = vert;
	dll->root->links[LNK][BWD] = vert;
	dll->length++;
}

void add_begin_dll(Dllist *dll, Vertex *vert, const int LNK)
{
	// root     <=>      a <=> b <=> c <=> d <=> root
	// root <=> vert <=> a <=> b <=> c <=> d <=> root
	vert->links[LNK][BWD] = dll->root; 
	vert->links[LNK][FWD] = dll->root->links[LNK][FWD];
	dll->root->links[LNK][FWD]->links[LNK][BWD] = vert;
	dll->root->links[LNK][FWD] = vert;
	dll->length++;
}

void rm_end_dll(Dllist *dll, const int LNK)
{
	// root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> b <=> c <=> d <=> e    <=>    root 
	if ( dll->length > 0 )
	{
		Vertex *temp = dll->root->links[LNK][BWD];
		dll->root->links[LNK][BWD]->links[LNK][BWD]->links[LNK][FWD] = dll->root;
		dll->root->links[LNK][BWD] = dll->root->links[LNK][BWD]->links[LNK][BWD];
		temp->links[LNK][FWD] = NULL;
		temp->links[LNK][BWD] = NULL;
		dll->length--;
	}
	else
		printf("Cannot remove in empty list\n");
}

void rm_begin_dll(Dllist *dll, const int LNK)
{
	// root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// root    <=>    b <=> c <=> d <=> e <=> f <=> root 
	if ( dll->length > 0 )
	{
		Vertex *temp = dll->root->links[LNK][FWD];
		dll->root->links[LNK][FWD]->links[LNK][FWD]->links[LNK][BWD] = dll->root;
		dll->root->links[LNK][FWD] = dll->root->links[LNK][FWD]->links[LNK][FWD];
		temp->links[LNK][FWD] = NULL;
		temp->links[LNK][BWD] = NULL;
		dll->length--;
	}
	else
		printf("Cannot remove in empty list\n");
}

void insert_after(Dllist *dll, Vertex *prev, Vertex *ins, const int LNK)
{
	// root <=> a <=> prev     <=>     b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> prev <=> ins <=> b <=> c <=> d <=> e <=> f <=> root 
	ins->links[LNK][FWD] = prev->links[LNK][FWD];
	ins->links[LNK][FWD]->links[LNK][BWD] = ins;
	ins->links[LNK][BWD] = prev;
	prev->links[LNK][FWD] = ins;
	dll->length++;
}

void rm_after(Dllist *dll, Vertex *prev, const int LNK)
{
	// root <=> a <=> prev <=> b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> prev    <=>    c <=> d <=> e <=> f <=> root 
	Vertex *rm = prev->links[LNK][FWD];
	rm->links[LNK][FWD]->links[LNK][BWD] = prev;
	rm->links[LNK][BWD]->links[LNK][FWD] = rm->links[LNK][FWD];
	rm->links[LNK][FWD] = NULL;
	rm->links[LNK][BWD] = NULL;
	dll->length--;
}	


void insert_btw(Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, const int LNK)
{
	// root <=> a <=> inf <=> b <=> c <=> d <=> sup <=> e <=> f <=> root 
	// root <=> a <=> inf <=>      ins      <=> sup <=> e <=> f <=> root 
	insert_after(dll, inf, ins, LNK);
	Vertex *current = ins ;
	while (current->links[LNK][FWD] != sup)
	{
		rm_after(dll, current, LNK);
	}
}


void move_after(Vertex *prev, Vertex *move, const int LNK)
{
	// root <=> a <=> prev <=> b <=> c <=> d <=> move <=> e <=> f <=> root 
	// root <=> a <=> prev <=> move <=> b <=> c <=> d <=> e <=> f <=> root 
	Vertex *t = move->links[LNK][BWD]->links[LNK][FWD];

	move->links[LNK][BWD]->links[LNK][FWD] = t->links[LNK][FWD];
	move->links[LNK][FWD]->links[LNK][BWD] = move->links[LNK][BWD];
	 
	t->links[LNK][BWD] = prev;
	t->links[LNK][FWD] = prev->links[LNK][FWD];
	prev->links[LNK][FWD]->links[LNK][BWD] = t;
	prev->links[LNK][FWD] = t;
}

void switch_cells(Vertex *p, Vertex *q, const int LNK)
{
	// root <=> a <=> P <=> b <=> c <=> d <=> Q <=> e <=> f <=> root 
	// root <=> a <=> Q <=> b <=> c <=> d <=> P <=> e <=> f <=> root 
	if (p->links[LNK][FWD]==q )
	{
		p->links[LNK][BWD]->links[LNK][FWD] = q;
		q->links[LNK][FWD]->links[LNK][BWD] = p;
		p->links[LNK][FWD] = q->links[LNK][FWD];
		q->links[LNK][BWD] = p->links[LNK][BWD];
		p->links[LNK][BWD] = q;
		q->links[LNK][FWD] = p;
	}
	else if ( q->links[LNK][FWD]==p )
	{
		switch_cells(q, p, LNK);
	}
	else
	{
		Vertex *tpbwd = p->links[LNK][BWD];
		Vertex *tpfwd = p->links[LNK][FWD];
		Vertex *tqbwd = q->links[LNK][BWD];
		Vertex *tqfwd = q->links[LNK][FWD];

		p->links[LNK][FWD]->links[LNK][BWD] = q;
		q->links[LNK][FWD]->links[LNK][BWD] = p;
		p->links[LNK][BWD]->links[LNK][FWD] = q;
		q->links[LNK][BWD]->links[LNK][FWD] = p;
		
		p->links[LNK][BWD] = tqbwd;
		q->links[LNK][BWD] = tpbwd;
		p->links[LNK][FWD] = tqfwd;
		q->links[LNK][FWD] = tpfwd;
	}
}

void copy_order(Dllist *dll, const int SRC, const int DEST)
{
	// SRC  : root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// DEST : root <=> c <=> f <=> d <=> a <=> b <=> e <=> root 
	// copy_order : 
	// DEST : root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	Vertex *temp = dll->root;
	for (int i = 0; i <= dll->length; i++)
	{
		temp->links[DEST][FWD] = temp->links[SRC][FWD];
		temp->links[DEST][BWD] = temp->links[SRC][BWD];
		temp = temp->links[SRC][FWD];
	}
	dll->length = dll->length;
}

void add_dll_end_dll(Dllist *dll, Vertex *inf, Vertex *sup, const int len, const int LNK)
{
	if (len > 0)
	{
		sew( dll->root->links[LNK][BWD], inf, LNK );
		sew( sup, dll->root, LNK );
		dll->length += len;
	}
}

void sew( Vertex *left, Vertex *right, const int LNK )
{
	left->links[LNK][FWD] = right;
	right->links[LNK][BWD] = left;
}
