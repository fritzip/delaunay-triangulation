#include <stdio.h>
#include <stdlib.h>

#include "geoalgo.h"

/*----------------------------------------------------------------------------------*/
//                                  Ordering functions
/*----------------------------------------------------------------------------------*/

int isSuperior(Vertex* p, Vertex *q, Dllist *dll, int LNK)
{
	switch (LNK)
	{
		case LEX: if ((p->coords[0] > q->coords[0]) || (p->coords[0] == q->coords[0] && p->coords[1] >= q->coords[1])) return 1;
			else return 0;
		case POL: return ( orientation( dll->root->links[LEX][FWD], p, q ) + 1 ) / 2 ; // gestion pts alignés 
		default: printf("Error in isSuperior function\n");
			break;
	}
	return -1;
}

int isInferior(Vertex *p, Vertex *q, Dllist *dll, int LNK)
{
	return 1 - isSuperior(p, q, dll, LNK);
}

void insNextAfter(Vertex *p, Vertex *q, int LNK)
{
	Vertex *t = q->links[LNK][FWD];

	q->links[LNK][FWD] = t->links[LNK][FWD];
	q->links[LNK][FWD]->links[LNK][BWD] = q;
	 
	t->links[LNK][BWD] = p;
	t->links[LNK][FWD] = p->links[LNK][FWD];
	p->links[LNK][FWD]->links[LNK][BWD] = t;
	p->links[LNK][FWD] = t;
}

void switchCells(Vertex *p, Vertex *q, int LNK)
{
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
		switchCells(q, p, LNK);
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

void graham(Dllist *dll)
{
	add_end_dll(dll, dll->root->links[POL][FWD], GRA); // 1st in polar order
	add_end_dll(dll, dll->root->links[POL][FWD]->links[POL][FWD], GRA); // 2nd in polar order
	Vertex *current = dll->root->links[POL][FWD]->links[POL][FWD]->links[POL][FWD]; // 3rd in polar order
	int height = 2;

	for (int i = 3; i <= dll->length[POL]; ++i)
	{
		while ( ( height >= 2 ) && ( orientation(dll->root->links[GRA][BWD]->links[GRA][BWD], dll->root->links[GRA][BWD], current) >= 0 ) )
		{
			rm_end_dll(dll, GRA);
			height--;
		}
		add_end_dll(dll, current, GRA);
		height++;
		current = current->links[POL][FWD];
	}
}

void jarvis(Dllist *dll)
{
	Vertex *p = dll->root->links[LEX][FWD];
	Vertex *q = NULL;
	Vertex *r = NULL;
	add_end_dll(dll, p, JAR);
	do
	{
		q = p->links[POL][FWD];
		r = q->links[POL][FWD];
		while (r != dll->root)
		{
			if ( orientation(p, q, r) >= 0 )
			{
				q = r;
			}			
			r = r->links[POL][FWD];
		}
		p = q;
		add_end_dll(dll, p, JAR);
	} while (p != dll->root->links[POL][BWD]);
}


void inittab(Dllist *dll)
{
	// Initialisation du tableau
	int LNK = LEX;
	Vertex *current = dll->root->links[LNK][FWD];
	for (int i = 0;i < NB_VERTEX;i++)
	{
		tablex[i]=current;
		current = current->links[LNK][FWD];
	}
}


void lexconvex(Dllist *dll)
{
	Vertex *first = dll->root->links[LEX][FWD];
	Vertex *deuze = first->links[LEX][FWD];
	Vertex *current = deuze->links[LEX][FWD];
	Vertex *aplacer = NULL;
	Vertex *maxlex = NULL;
	Vertex *bot = NULL;
	int debugi = 0;
	
	if (1==orientation(first, deuze, current))
	{
		// Si le triangle est CW, on lie les points directement
		first->links[LXC][FWD]=deuze;
		deuze->links[LXC][FWD]=current;
		current->links[LXC][FWD]=first;
		// Dans les deux sens
		first->links[LXC][BWD]=current;
		deuze->links[LXC][BWD]=first;
		current->links[LXC][BWD]=deuze;
	}
	else
	{
		//Sinon, s'il est CCW, on les lie dans le sens inverse
		first->links[LXC][FWD]=current;
		deuze->links[LXC][FWD]=first;
		current->links[LXC][FWD]=deuze;
		// Dans les deux sens
		first->links[LXC][BWD]=deuze;
		deuze->links[LXC][BWD]=current;
		current->links[LXC][BWD]=first;	
	}

	while ((current->links[LEX][FWD] != dll->root))
	{ // Tant qu'on a pas fait le tour des points à placer...
		debugi++;	
		aplacer=current->links[LEX][FWD];	
		maxlex=current;
		while (1==orientation(current, aplacer, current->links[LXC][FWD]))
		{ // On check si l'angle est CW	
			current=current->links[LXC][FWD]; // On avance
			current->links[LXC][BWD]->links[LXC][FWD]=NULL; // Et on détruit le liens avec le précédent
			current->links[LXC][BWD]=NULL; // Dans les deux sens
		}
		bot=current;
		current = maxlex; // On réinitialise current
		while (0>=orientation(current, aplacer, current->links[LXC][BWD]))
		{ // On check si l'angle est CCW	
			current=current->links[LXC][BWD]; // On recule
			current->links[LXC][FWD]->links[LXC][BWD]=NULL; // Et on détruit le liens avec le suivant
			current->links[LXC][FWD]=NULL; // Dans les deux sens
		}
		bot->links[LXC][BWD]=aplacer; // Puis on crée la nouvelle arrete
		aplacer->links[LXC][FWD]=bot;
		current->links[LXC][FWD]=aplacer; // Puis on crée la nouvelle arrete
		aplacer->links[LXC][BWD]=current; // Dans les deux sens	
		current = aplacer; // Et on remet le curseur sur le dernier placé	
	}
	dll->root->links[LXC][FWD] = dll->root->links[LEX][FWD];
	dll->root->links[LXC][BWD] = dll->root->links[LEX][BWD];
	int size=1;
	current=dll->root->links[LXC][FWD];
	first=dll->root->links[LXC][FWD];
	while (current->links[LXC][FWD]!=first)
	{
		size++;
		current=current->links[LXC][FWD];
		//printf(" Current = %d,%d\n", current->coords[0],current->coords[1]);
	}
	dll->length[LXC] = size;
}

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////// FUSION /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


EConv miniConvexe(int idmin, int idmax)
{
	EConv EC;
	if (2==(idmax-idmin)) 
	{
		if (1==orientation(tablex[idmin],tablex[idmin+1],tablex[idmax]))
		{
			tablex[idmin]->links[DAC][BWD]=tablex[idmin+1];
			tablex[idmin+1]->links[DAC][BWD]=tablex[idmax];
			tablex[idmax]->links[DAC][BWD]=tablex[idmin];
			tablex[idmin]->links[DAC][FWD]=tablex[idmax];
			tablex[idmin+1]->links[DAC][FWD]=tablex[idmin];
			tablex[idmax]->links[DAC][FWD]=tablex[idmin+1];
		} 
		else 
		{
			tablex[idmin]->links[DAC][FWD]=tablex[idmin+1];
			tablex[idmin+1]->links[DAC][FWD]=tablex[idmax];
			tablex[idmax]->links[DAC][FWD]=tablex[idmin];
			tablex[idmin]->links[DAC][BWD]=tablex[idmax];
			tablex[idmin+1]->links[DAC][BWD]=tablex[idmin];
			tablex[idmax]->links[DAC][BWD]=tablex[idmin+1];
		}
	}
	if (1==(idmax-idmin))
	{
		tablex[idmax]->links[DAC][BWD]=tablex[idmin];
		tablex[idmax]->links[DAC][FWD]=tablex[idmin];
		tablex[idmin]->links[DAC][FWD]=tablex[idmax];
		tablex[idmin]->links[DAC][BWD]=tablex[idmax];
	}
	if (0==(idmax-idmin))
	{
		tablex[idmax]->links[DAC][BWD]=tablex[idmax];
		tablex[idmax]->links[DAC][FWD]=tablex[idmax];
	}
	/*for (int i=0; i<4; i++){
		tablex[idmin] = tablex[idmin]->links[6][FWD];
	}*/
	EC.vmax=tablex[idmax];
	EC.vmin=tablex[idmin];
	return EC;
} 
  
EConv fusionec(EConv ECg, EConv ECd)
{
	EConv EC;
	Vertex *g=ECg.vmax;
	Vertex *d=ECd.vmin;
	Vertex *tg=ECg.vmax;
	Vertex *td=ECd.vmin;
	// On ajoute l'arete du bas
	while (-1==orientation(g, d, d->links[6][BWD]) || (1==orientation(d, g, g->links[6][FWD])))
	{
		if (-1==orientation(g, d, d->links[6][BWD]))
		{
			d=d->links[6][BWD];         
		}
		if (1==orientation(d, g, g->links[6][FWD]))
		{
			g=g->links[6][FWD];      
		}
	}
	tg = g;
	td = d;
	// On ajoute l'arete du haut
	g=ECg.vmax;
	d=ECd.vmin;
	while (1==orientation(g, d, d->links[6][FWD]) || (-1==orientation(d, g, g->links[6][BWD])))
	{
		if (1==orientation(g, d, d->links[6][FWD]))
		{
			d=d->links[6][FWD];             
		}
		if (-1==orientation(d, g, g->links[6][BWD]))
		{
			g=g->links[6][BWD];
		}
	}
	g->links[6][FWD]=d;
	d->links[6][BWD]=g;
	tg->links[6][BWD]=td;
	td->links[6][FWD]=tg;
	// Et on retourne l'EC fusionnée
	EC.vmax=ECd.vmax;
	EC.vmin=ECg.vmin;
	return EC;
}  
  
EConv divideAndConquer(int idmin, int idmax)
{
	EConv EC;
	EConv ECg;
	EConv ECd;
	if (2>=(idmax-idmin))
	{
		EC = miniConvexe(idmin,idmax);
	}
	else
	{
		ECg=divideAndConquer(idmin, ((idmax-idmin)/2)+idmin);
		ECd=divideAndConquer(((idmax-idmin)/2)+idmin+1, idmax);
		EC = fusionec(ECg,ECd);
	}
	return EC;
}

void divac(Dllist *dll)
{
	divideAndConquer(0, NB_VERTEX-1);
	dll->length[DAC]=dll->length[JAR];
	dll->root->links[DAC][FWD] = dll->root->links[LEX][FWD];
	dll->root->links[DAC][BWD] = dll->root->links[LEX][BWD];
	//dll->length[DAC] = dll->length[JAR];
	int size=1;
	Vertex *current=dll->root->links[LXC][FWD];
	Vertex *first=dll->root->links[LXC][FWD];
	while (current->links[LXC][FWD]!=first)
	{
		size++;
		current=current->links[LXC][FWD];
		//printf(" Current = %d,%d\n", current->coords[0],current->coords[1]);
	}
	dll->length[LXC] = size;
}


Vertex* merge(Vertex* p, int P, Vertex* q, int Q, Dllist *dll, int LNK)
{
	while (1)
	{
		if ( isSuperior( p->links[LNK][FWD], q->links[LNK][FWD], dll, LNK ) )
		{
			insNextAfter(p, q, LNK);

			if ( Q == 1 ) break;
			Q = Q - 1;
		}
		else
		{
			if ( P == 1 )
			{
				while ( Q >= 1 )
				{
					q = q->links[LNK][FWD];
					Q = Q - 1;
				}
				break;
			}
			P = P - 1;
		}
		p = p->links[LNK][FWD];
	}
	return q;
}


Vertex* sort(Vertex *p, int n, Dllist *dll, int LNK)
{
	Vertex *q = NULL;
	int Q = n / 2;
	int P = n - Q;
	if ( P >= 2 )
	{
		q = sort(p, P, dll, LNK);
		if ( Q >= 2 ) sort(q, Q, dll, LNK);
	}
	else
	{
		q = p->links[LNK][FWD];
	}
	q = merge(p, P, q, Q, dll, LNK);
	dll->up2date[LNK] = 1;
	return q;
}
