#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  
#include <math.h>  
#include <assert.h>
#include <GL/glut.h> 

/*----------------------------------------------------------------------------------*/
//                                  #define
/*----------------------------------------------------------------------------------*/

#define DEBUG 0

#define DIM 2

#define BWD 0 
#define FWD 1

#define STD 0
#define LEX 1
#define POL 2
#define GRA 3
#define JAR 4
#define LXC 5
#define DAC 6
#define NBL 7 // Nb de Liens : A incrémenter manuellement, LAST + 1

int NB_VERTEX = 20;

const double minX = 0,
			maxX = 500,
			minY = 0,
			maxY = 500,
			margin = 10;


/*! variable externe permettant de lire les parametres sur le ligne de commande.*/
extern char *optarg;

/*! bascule pour autoriser ou interdire (0) les messages d'erreurs envoyes par getopt. */
extern int opterr;

/* Display option */
int displayChoice = 3;
char *lineOption[] = { "GL_POINTS", "GL_LINES", "GL_LINE_STRIP", "GL_LINE_LOOP", "GL_POLYGON"};


/*----------------------------------------------------------------------------------*/
//                                  Structures
/*----------------------------------------------------------------------------------*/

typedef struct vertex
{
	int coords[DIM];
	struct vertex *links[NBL][2];
} Vertex;

typedef struct
{
	int length[NBL];
	int up2date[NBL];
	Vertex *root;
} Dllist;


#if(DEBUG == 1)
	void debug()

#endif
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

/*----------------------------------------------------------------------------------*/
//                                  DLL Functions
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


void add_end_dll(Dllist *dll, Vertex *vert, int LNK)
{
	vert->links[LNK][BWD] = dll->root->links[LNK][BWD];
	vert->links[LNK][FWD] = dll->root;
	dll->root->links[LNK][BWD]->links[LNK][FWD] = vert;
	dll->root->links[LNK][BWD] = vert;
	dll->length[LNK]++;
}


void add_begin_dll(Dllist *dll, Vertex *vert, int LNK)
{
	vert->links[LNK][BWD] = dll->root; 
	vert->links[LNK][FWD] = dll->root->links[LNK][FWD];
	dll->root->links[LNK][FWD]->links[LNK][BWD] = vert;
	dll->root->links[LNK][FWD] = vert;
	dll->length[LNK]++;
}

void insert_after(Dllist *dll, Vertex *prev, Vertex *ins, int LNK)
{
	ins->links[LNK][FWD] = prev->links[LNK][FWD];
	ins->links[LNK][FWD]->links[LNK][BWD] = ins;
	ins->links[LNK][BWD] = prev;
	prev->links[LNK][FWD] = ins;
	dll->length[LNK]++;
}

void rm_after(Dllist *dll, Vertex *prev, int LNK)
{
	Vertex *rm = prev->links[LNK][FWD];
	rm->links[LNK][FWD]->links[LNK][BWD] = prev;
	rm->links[LNK][BWD]->links[LNK][FWD] = rm->links[LNK][FWD];
	rm->links[LNK][FWD] = NULL;
	rm->links[LNK][BWD] = NULL;
	dll->length[LNK]--;
}	

void insert_btw(Dllist *dll, Vertex *inf, Vertex *sup, Vertex *ins, int LNK)
{
	insert_after(dll, inf, ins, LNK);
	Vertex *current = ins ;
	while (current->links[LNK][FWD] != sup)
	{
		rm_after(dll, current, LNK);
	}
}

void rm_end_dll(Dllist *dll, int LNK)
{
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

void rm_begin_dll(Dllist *dll, Vertex *vert, int LNK)
{
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

void copy_order(Dllist *dll, int SRC, int DEST)
{
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
//                                  Print functions
/*----------------------------------------------------------------------------------*/

void print_lnk(int LNK)
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


void print_info(Dllist *dll, int LNK)
{
	printf("=================\n");
	print_lnk(LNK);
	printf("  Length : %d \n", dll->length[LNK]);
	printf("=================\n");
}


void print_vertex(Vertex *vert)
{
	for (int i = 0; i < DIM; i++)
	{
		printf("%4d", vert->coords[i]);
	}
	printf("\n");
}


void print_dll(Dllist *dll, int LNK)
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


void print_dll_full(Dllist *dll, int LNK)
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

/*----------------------------------------------------------------------------------*/
//                      Create and remove data struct functions
/*----------------------------------------------------------------------------------*/

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


void init_links(Dllist *dll, int LNK)
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
				// insertionlexico(dll); 
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
				// divideandconquer(dll); 
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

/*----------------------------------------------------------------------------------*/
//                                  Graphic functions
/*----------------------------------------------------------------------------------*/
void winInit (void)
{
	gluOrtho2D(minX, maxX, minY, maxY);
}


void display (void)
{
	glColor3f(0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
		
	switch (displayChoice)
	{
		case 0: glBegin(GL_POINTS);
				break;
		case 1: glBegin(GL_LINES);
				break;
		case 2: glBegin(GL_LINE_STRIP);
				break;
		case 3: glBegin(GL_LINE_LOOP);
				break;
		default: glBegin(GL_POLYGON);
				break;
	}
	glEnd();

	//----------------------------------------------------------//
	Dllist *dll_lnk = create_rd_data_struct();

	init_links(dll_lnk, GRA);
	init_links(dll_lnk, JAR);

	print_dll(dll_lnk, STD);
	print_dll(dll_lnk, LEX);
	print_dll(dll_lnk, POL);
	print_dll(dll_lnk, GRA);
	print_dll(dll_lnk, JAR);

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 0.0, 0.0);

	int LNK = POL;
	Vertex *current = dll_lnk->root->links[LNK][FWD];

	for(int i = 0; i < dll_lnk->length[LNK]; i++)
	{
		glVertex2f(current->coords[0], current->coords[1]);
		current = current->links[LNK][FWD];
	}

	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 1.0);

	LNK = JAR;
	current = dll_lnk->root->links[LNK][FWD];

	for(int i = 0; i < dll_lnk->length[LNK]; i++)
	{
		glVertex2f(current->coords[0], current->coords[1]);
		current = current->links[LNK][FWD];
	}

	// remove_data_struct(dll_lnk);

	glEnd();
	glFlush();
}


/*----------------------------------------------------------------------------------*/
//                                  Main
/*----------------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	srand (time(NULL));
	int option;

	opterr = 0; /* set off std error messages in case wrong option is chosen */
	while ((option = getopt(argc, argv, "c:")) != EOF)
	{
		switch (option)
		{
		case 'c': if ((sscanf(optarg, "%d", &displayChoice) == 1) && displayChoice >= 0 && displayChoice <= 4)
				break;
		case '?': /* value returned by getopt if invalid option chosen */
		default : printf("Usage: %s -cX, with X = 1, 2, 3 or 4.\n", argv[0]);
				displayChoice = 0;
				break;
		}
	}

	assert(displayChoice >= 0 && displayChoice <= 4);
	printf("Executing %s with line option %d = %s.\n", argv[0], displayChoice, lineOption[displayChoice]);
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  

	glutInitWindowPosition(5,5);  
	glutInitWindowSize(maxX,maxY);  

	glutCreateWindow("My first OpenGL window...");  
	
	winInit();

	glutDisplayFunc(display);  

	glutMainLoop();  
		
	return EXIT_SUCCESS; 
}