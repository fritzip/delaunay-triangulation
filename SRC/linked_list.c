#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "geoalgo.h"

/*----------------------------------------------------------------------------------*/
//                                  DLL Functions prototypes
/*----------------------------------------------------------------------------------*/
Vertex* create_vert( int coords[DIM] )
{
	Vertex *new_vert = (Vertex *) malloc(sizeof(Vertex));

	for(int i=0; i<DIM; i++)
	{
		new_vert->coords[i] = coords[i];
	}

	for(int i=0; i<NBL; i++)
	{
		new_vert->links[i][BWD] = NULL;	
		new_vert->links[i][FWD] = NULL;
	}

	return new_vert;
}


Dllist* create_dll(void)
{
	Dllist *new_dll = (Dllist *) malloc(sizeof(Dllist));
	if (new_dll != NULL)
	{
		int coords[DIM] = {0};
		new_dll->root = create_vert(coords);

		for(int i=0; i<NBL; i++)
		{
			new_dll->root->links[i][BWD] = new_dll->root;	
			new_dll->root->links[i][FWD] = new_dll->root;
			new_dll->length[i] = 0;
			new_dll->up2date[i] = 0;
		}
	}
	return new_dll;
}

void add_end_dll(Dllist *dll, Vertex *vert, int const LNK)
{
	// root <=> a <=> b <=> c <=> d      <=>     root
	// root <=> a <=> b <=> c <=> d <=> vert <=> root
	vert->links[LNK][BWD] = dll->root->links[LNK][BWD];
	vert->links[LNK][FWD] = dll->root;
	dll->root->links[LNK][BWD]->links[LNK][FWD] = vert;
	dll->root->links[LNK][BWD] = vert;
	dll->length[LNK]++;
}

void add_begin_dll(Dllist *dll, Vertex *vert, int const LNK)
{
	// root     <=>      a <=> b <=> c <=> d <=> root
	// root <=> vert <=> a <=> b <=> c <=> d <=> root
	vert->links[LNK][BWD] = dll->root; 
	vert->links[LNK][FWD] = dll->root->links[LNK][FWD];
	dll->root->links[LNK][FWD]->links[LNK][BWD] = vert;
	dll->root->links[LNK][FWD] = vert;
	dll->length[LNK]++;
}

void rm_end_dll(Dllist *dll, int const LNK)
{
	// root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> b <=> c <=> d <=> e    <=>    root 
	if ( dll->length[LNK] > 0 )
	{
		Vertex *temp = dll->root->links[LNK][BWD];
		dll->root->links[LNK][BWD]->links[LNK][BWD]->links[LNK][FWD] = dll->root;
		dll->root->links[LNK][BWD] = dll->root->links[LNK][BWD]->links[LNK][BWD];
		temp->links[LNK][FWD] = NULL;
		temp->links[LNK][BWD] = NULL;
		dll->length[LNK]--;
	}
	else
		printf("Cannot remove in empty list\n");
}

void rm_begin_dll(Dllist *dll, int const LNK)
{
	// root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// root    <=>    b <=> c <=> d <=> e <=> f <=> root 
	if ( dll->length[LNK] > 0 )
	{
		Vertex *temp = dll->root->links[LNK][FWD];
		dll->root->links[LNK][FWD]->links[LNK][FWD]->links[LNK][BWD] = dll->root;
		dll->root->links[LNK][FWD] = dll->root->links[LNK][FWD]->links[LNK][FWD];
		temp->links[LNK][FWD] = NULL;
		temp->links[LNK][BWD] = NULL;
		dll->length[LNK]--;
	}
	else
		printf("Cannot remove in empty list\n");
}

void insert_after(Dllist *dll, Vertex *prev, Vertex *ins, int const LNK)
{
	// root <=> a <=> prev     <=>     b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> prev <=> ins <=> b <=> c <=> d <=> e <=> f <=> root 
	ins->links[LNK][FWD] = prev->links[LNK][FWD];
	ins->links[LNK][FWD]->links[LNK][BWD] = ins;
	ins->links[LNK][BWD] = prev;
	prev->links[LNK][FWD] = ins;
	dll->length[LNK]++;
}

void rm_after(Dllist *dll, Vertex *prev, int const LNK)
{
	// root <=> a <=> prev <=> b <=> c <=> d <=> e <=> f <=> root 
	// root <=> a <=> prev    <=>    c <=> d <=> e <=> f <=> root 
	Vertex *rm = prev->links[LNK][FWD];
	rm->links[LNK][FWD]->links[LNK][BWD] = prev;
	rm->links[LNK][BWD]->links[LNK][FWD] = rm->links[LNK][FWD];
	rm->links[LNK][FWD] = NULL;
	rm->links[LNK][BWD] = NULL;
	dll->length[LNK]--;
}	


void insert_btw(Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, int const LNK)
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

void move_after(Vertex *prev, Vertex *move, int const LNK)
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

void switch_cells(Vertex *p, Vertex *q, int const LNK)
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

void copy_order(Dllist *dll, int const SRC, int const DEST)
{
	// SRC  : root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	// DEST : root <=> c <=> f <=> d <=> a <=> b <=> e <=> root 
	// copy_order : 
	// DEST : root <=> a <=> b <=> c <=> d <=> e <=> f <=> root 
	Vertex *temp = dll->root;
	for (int i = 0; i <= dll->length[STD]; i++)
	{
		temp->links[DEST][FWD] = temp->links[SRC][FWD];
		temp->links[DEST][BWD] = temp->links[SRC][BWD];
		temp = temp->links[SRC][FWD];
	}
	dll->length[DEST] = dll->length[SRC];
}



/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

void init_links(Dllist *dll, int const LNK)
{
	switch (LNK)
	{
		case LEX: 
			copy_order(dll, STD, LEX);
			sort(dll->root, dll->length[LEX], dll, LEX);
			break;

		case POL: 
			if (dll->up2date[LEX])
			{
				copy_order(dll, LEX, POL);
				sort(dll->root->links[POL][FWD], dll->length[POL]-1, dll, POL);
			}
			else 
			{
				init_links(dll, LEX);
				init_links(dll, POL);
			}
			break;

		case GRA:
			if (dll->up2date[POL])
				graham(dll); 
			else
			{
				init_links(dll, POL);
				init_links(dll, GRA);
			}
			break;

		case JAR:
			if (dll->up2date[POL])
				jarvis(dll); 
			else
			{
				init_links(dll, POL);
				init_links(dll, JAR);
			}
			break;

		case LXC:
			if (dll->up2date[LEX]) 
			{
				lexconvex(dll);
			}
			else
			{
				init_links(dll, LEX);
				init_links(dll, LXC);
			}
			break;

		case DAC:
			if (dll->up2date[LEX]) 
			{
				inittab(dll);
				divac(dll);
			}
			else
			{
				init_links(dll, LEX);
				init_links(dll, DAC);
			}
			break;

		default: printf("Error in init_links function\n");
			break;
	}
}


Dllist* create_rd_data_struct()
{
	Dllist* dll_lnk = create_dll();

	for(int i=0; i<NB_VERTEX; i++)
	{
		int coords[2] = { randn( minX+margin, maxX-margin ), randn( minY+margin, maxY-margin ) };
		Vertex *new_vert = create_vert( coords );
		add_end_dll( dll_lnk, new_vert, STD );
	}
	dll_lnk->up2date[STD] = 1;
	return dll_lnk;
}

// TODO
// create_data_struct_from_tab_of_coords()

void remove_data_struct(Dllist *dll)
{
	if (dll != NULL)
	{
		Vertex *del = dll->root->links[STD][FWD];
		Vertex *temp = NULL;

		while (del != dll->root)
		{
			temp = del->links[STD][FWD];

			free(del);
			del->links[STD][FWD] = NULL;
			del->links[STD][BWD] = NULL;

			del = temp;
			dll->length[STD]--;
		}
		free(dll->root);
		free(dll);	
		dll = NULL;
	}	
}
