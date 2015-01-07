/*! \file main.c
 * \author M. Sainlot
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <GL/glut.h> 

#include "triangulation.h"
#include "print_fn.h"
#include "ressources.h"
#include "math_fn.h"
#include "pgm.h"

/*! variable externe permettant de lire les parametres sur le ligne de commande.*/
extern char *optarg;

/*! bascule pour autoriser ou interdire (0) les messages d'erreurs envoyes par getopt. */
extern int opterr;

/* Display option */
GLenum gl_display_mode[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_POLYGON};
char *lineOption[] = { "GL_POINTS", "GL_LINES", "GL_LINE_STRIP", "GL_LINE_LOOP", "GL_POLYGON"};

int SCREEN_WIDTH = 1000, SCREEN_HEIGHT = 1000;

int TWO_D = 0; 
int GRAD = 1;
int NORMAL = 1;
int DISPLAY_MODE = 4;
int TEXT = 1;

float rho = 2.0f;
float xrot = -40.0f;
float zrot = 0.0f;

int xOrigin = -1, yOrigin = -1;

/* Delaunay Grid */
Grid* mygrid;

/*----------------------------------------------------------------------------------*/
//                                  Color functions
/*----------------------------------------------------------------------------------*/
typedef struct color
{
	double r,g,b;
} Color ;

/* Color variables */
#define nb_gradient_val 100
Color gradient_color[nb_gradient_val] = {};

Color getcol(double z, double zmax)
{
	if (GRAD)
	{
		if (z < 0.01)
		{
			Color col = {55.0, 40.0, 115.0};
			return col;
		}
		else
			return gradient_color[(int)floor(z*nb_gradient_val/(zmax+0.00001))];
	}
	else
	{
		Color col = {0.0, 255.0, 0.0};
		return col;
	}
}

void compute_color_gradient(Color tab[], int nbval, double r1, double g1, double b1, double r2, double g2, double b2)
{
	// pour chaque canal, calcul du différenciel entre chaque teinte (nbVal est le nombre de teintes du dégradé)
	double dr = ((r1 - r2) / nbval);
	double dg = ((g1 - g2) / nbval);
	double db = ((b1 - b2) / nbval);
 
	// on boucle pour remplir un tableau contenant toutes les valeurs des teintes
	for (int i = 0; i < nbval; ++i)
	{
		tab[i].r = r1;
		tab[i].g = g1;
		tab[i].b = b1; 
		r1 -= dr;
		g1 -= dg;
		b1 -= db;
	}
}

/*----------------------------------------------------------------------------------*/
//                                  Graphic functions
/*----------------------------------------------------------------------------------*/


void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(60.0f, ratio, 0.01f, 10000.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	SCREEN_HEIGHT = h;
	SCREEN_WIDTH = w;
}


void processNormalKeys(unsigned char key, int xx, int yy) 
{   
	if (key == 27)
		exit(0);
	if (key == ' ')
	{
		TWO_D = 1 - TWO_D;
		if (TWO_D)
		{
			NORMAL = 0;
			DISPLAY_MODE = 3;			
		}
		else 
		{
			NORMAL = 1;
			DISPLAY_MODE = 4;
		}
	}
	if (key == 'c')
		GRAD = 1 - GRAD;
	if (key == 'n')
		NORMAL = 1 - NORMAL;
	if (key == 't')
		TEXT = 1 - TEXT;
	switch (key)
	{
		case '0': DISPLAY_MODE = 0 ; break;
		case '1': DISPLAY_MODE = 1 ; break;
		case '2': DISPLAY_MODE = 2 ; break;
		case '3': DISPLAY_MODE = 3 ; break;
		case '4': DISPLAY_MODE = 4 ; break;
	}
} 

void pressKey(int key, int xx, int yy)
{
	switch (key) 
	{
		case GLUT_KEY_UP : xrot -= 2; break; 
		case GLUT_KEY_DOWN : xrot += 2; break; 
		case GLUT_KEY_LEFT : zrot += 2; break;
		case GLUT_KEY_RIGHT : zrot -= 2; break;
	}
} 


void mouseMove(int x, int y) 
{
	// this will only be true when the left button is down
	if (xOrigin >= 0) 
	{
		zrot += 0.5f * (x - xOrigin);
		xrot += 0.5f * (y - yOrigin);

		xOrigin = x;
		yOrigin = y;
	}
}

void mouseButton(int button, int state, int x, int y)
{
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) 
	{
		// when the button is released
		if (state == GLUT_UP) 
		{
			xOrigin = -1;
			yOrigin = -1;
		}
		else
		{// state = GLUT_DOWN
			xOrigin = x;
			yOrigin = y;
		}
	}
	if (button == 3)
	{
		rho -= 0.1;
	}
	if (button == 4)
	{
		rho += 0.1;
	}
}


void compute_pos() 
{
	if (xrot > 0) xrot = 0;
	if (xrot < -90) xrot = -90; 
	if (zrot > 360) zrot -= 360;
	if (zrot < 0) zrot += 360;

	// printf("xrot = %f, zrot = %f, rho = %f\n", xrot, zrot, rho);
	// printf("phi = %f, theta = %f, rho = %f\n", phi, theta, rho);
	// printf("camx = %f, camy = %f, camz = %f\n", camx, camy, camz);
}

void vBitmapOutput(double x, double y, char *string, void *font)
{
	int len,i; // len donne la longueur de la chaîne de caractères

	glRasterPos2f(x,y); // Positionne le premier caractère de la chaîne
	len = (int) strlen(string); // Calcule la longueur de la chaîne
	for (i = 0; i < len; i++) glutBitmapCharacter(font,string[i]); // Affiche chaque caractère de la chaîne
}


void display_text()
{
	if (TEXT)
	{
		glColor3d(0, 0, 0);
		glBegin(GL_QUADS);
	        glVertex3f(0.01,0.99,0.5);
	        glVertex3f(0.01,0.86,0.5);
	        glVertex3f(0.3,0.86,0.5);
	        glVertex3f(0.3,0.99,0.5);
	    glEnd();

		glColor3d(1,1,1);


		vBitmapOutput(0.02,0.97,"[ SPACE ]",GLUT_BITMAP_HELVETICA_12);
		vBitmapOutput(0.08,0.97,"VISUALISATION",GLUT_BITMAP_HELVETICA_12);
		if (TWO_D)
			vBitmapOutput(0.2,0.97," : 2D",GLUT_BITMAP_HELVETICA_12);
		else
			vBitmapOutput(0.2,0.97," : 3D",GLUT_BITMAP_HELVETICA_12);


		vBitmapOutput(0.02,0.95,"[ C ]",GLUT_BITMAP_HELVETICA_12);
		vBitmapOutput(0.08,0.95,"GRADIENT COLOR",GLUT_BITMAP_HELVETICA_12);
		if (GRAD)
			vBitmapOutput(0.2,0.95," : ON",GLUT_BITMAP_HELVETICA_12);
		else
			vBitmapOutput(0.2,0.95," : OFF",GLUT_BITMAP_HELVETICA_12);


		vBitmapOutput(0.02,0.93,"[ N ]",GLUT_BITMAP_HELVETICA_12);
		vBitmapOutput(0.08,0.93,"NORMALS",GLUT_BITMAP_HELVETICA_12);
		if (NORMAL)
			vBitmapOutput(0.2,0.93," : ON",GLUT_BITMAP_HELVETICA_12);
		else
			vBitmapOutput(0.2,0.93," : OFF",GLUT_BITMAP_HELVETICA_12);


		if (DISPLAY_MODE == 0)
		{
			vBitmapOutput(0.02,0.91,"[ 0 ]",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.08,0.91,"DISPLAY MODE",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.2,0.91," : POINTS ",GLUT_BITMAP_HELVETICA_12);
		}
		else if (DISPLAY_MODE == 1)
		{
			vBitmapOutput(0.02,0.91,"[ 1 ]",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.08,0.91,"DISPLAY MODE",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.2,0.91," : LINES",GLUT_BITMAP_HELVETICA_12);
		}
		else if (DISPLAY_MODE == 2)
		{
			vBitmapOutput(0.02,0.91,"[ 2 ]",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.08,0.91,"DISPLAY MODE",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.2,0.91," : LINE_STRIP",GLUT_BITMAP_HELVETICA_12);
		}
		else if (DISPLAY_MODE == 3)
		{
			vBitmapOutput(0.02,0.91,"[ 3 ]",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.08,0.91,"DISPLAY MODE",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.2,0.91," : LINE_LOOP",GLUT_BITMAP_HELVETICA_12);
		}
		else if (DISPLAY_MODE == 4)
		{
			vBitmapOutput(0.02,0.91,"[ 4 ]",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.08,0.91,"DISPLAY MODE",GLUT_BITMAP_HELVETICA_12);
			vBitmapOutput(0.2,0.91," : POLYGON",GLUT_BITMAP_HELVETICA_12);
		}

		vBitmapOutput(0.02,0.89,"[ T ]",GLUT_BITMAP_HELVETICA_12);
		vBitmapOutput(0.08,0.89,"TOOGLE INFO",GLUT_BITMAP_HELVETICA_12);

		vBitmapOutput(0.02,0.87,"[ ESC ]",GLUT_BITMAP_HELVETICA_12);
		vBitmapOutput(0.08,0.87,"CLOSE",GLUT_BITMAP_HELVETICA_12);
		
	}
}

void apply_transform()
{
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -rho);

	glPushMatrix();
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(zrot, 0.0, 0.0, 1.0); 

	glTranslatef(-0.5, -0.5, -0.2);
}

void switch_to_ortho()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(minX-margin, maxX+margin, minY-margin,  maxY+margin, -1, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void switch_to_perspective()
{
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void display (void)
{
	compute_pos();
	double x, y, z;

	// Clear Color and Depth Buffers
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	apply_transform();

	if (TWO_D)
		switch_to_ortho();
	
	for (int i = 0; i < mygrid->nb_simp; i++)
	{
		glBegin(gl_display_mode[DISPLAY_MODE]);
		Simplex *simp = mygrid->table_of_simp[i];
		
		// Normal or not 
		if (NORMAL) glNormal3f(simp->na, simp->nb, simp->nc);
		else glNormal3f(1,1,1);

		x = simp->sommet[0]->coords[0];
		y = simp->sommet[0]->coords[1];

		for(int j = 0; j < 3; j++)
		{
			x = simp->sommet[j]->coords[0];
			y = simp->sommet[j]->coords[1];
			z = simp->sommet[j]->coords[2];
			
			Color col = getcol( z, mygrid->zmax );
			glColor4f(col.r/255, col.g/255, col.b/255, 1);

			glVertex3f(x, y, z);
		
		}			
		glEnd();
	}	


	if (TWO_D)
	{
		display_text();
		switch_to_perspective();
	}
	else
	{
		switch_to_ortho();
		display_text();
		switch_to_perspective();
	}

	glutSwapBuffers();
}

/*----------------------------------------------------------------------------------*/
//                                  Main
/*----------------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	srand (time(NULL));

	int NB_VERTEX = 0;
	int NB_SIMPLEX = 0;
	int GOF = 0;
	int ZUNIF = 0;
	char *INPUT_FILE = NULL;
	char OUTPUT_CONDITION;

	//***********************//
	//      Arg parser
	//***********************//
	int opt;
	while ((opt = getopt (argc, argv, "c:n:i:f:s:z")) != -1)
		switch (opt)
		{
			case 'n':
				sscanf(optarg, "%d", &NB_VERTEX);
				break;			
			case 's':
				sscanf(optarg, "%d", &GOF);
				OUTPUT_CONDITION = 's';
				break;			
			case 'f':
				sscanf(optarg, "%d", &NB_SIMPLEX);
				OUTPUT_CONDITION = 'f';
				break;
			case 'c':
				sscanf(optarg, "%d", &DISPLAY_MODE);
				break;
			case 'i':
				INPUT_FILE = optarg;
				break;
			case 'z':
				ZUNIF = 1;
				break;	
			case '?':
				if (optopt == 'c' || optopt == 'n' || optopt == 'i' || optopt == 's' || optopt == 'f')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\%x'.\n", optopt);
				printf("Usage: %s [-i INPUT_FILE | -z] [-n NB_PTS] [-f NB_SIMPX | -s GOF(%%)] [-c DISPLAY_MODE]\ncf. README.md for further information\n", argv[0]);
				return 1;
			default:
				abort();
		}

	//*****************************//
	// Check consistency of :
	//*****************************//
		// Output conditions
	if (NB_SIMPLEX > 0 && GOF > 0)
	{
		printf("Maximum 1 output condition (2 given -f %d -s %d)\n", NB_SIMPLEX, GOF);
		printf("Usage: %s [-i INPUT_FILE | -z] [-n NB_PTS] [-f NB_SIMPX | -s GOF(%%)] [-c DISPLAY_MODE]\ncf. README.md for further information\n", argv[0]);
		return 1;
	}

		// Number of points
	if (NB_VERTEX <= 0)
		NB_VERTEX = 10000;

		// Number of triangles
	if (NB_SIMPLEX <= 0 || NB_SIMPLEX > 2*(NB_VERTEX+4 - 1) - 4 )
		NB_SIMPLEX = 2*(NB_VERTEX+4 - 1) - 4;

		// Input heightmap (convert if necessary)
	PGMData mydata = {};

	if (INPUT_FILE != NULL)
	{
		char *pFile = strrchr(INPUT_FILE, '/');
		pFile = pFile == NULL ? INPUT_FILE : pFile+1;
		// change extension
		char *pExt = strrchr(pFile, '.');

		if (strcmp(pExt,".pgm") != 0)
		{
			printf("Convertion \n");
			char command[300] = {0};

			sprintf(command, "x=%s; convert $x ${x%%.*}.pgm", INPUT_FILE);
			system(command);
		}

		if (pExt != NULL)
			strcpy(pExt, ".pgm");
		else
			strcat(pFile, ".pgm");

		read_PGM(INPUT_FILE, &mydata);
	}

		// Display mode
	if (DISPLAY_MODE == 5)
	{
		printf("No display\n");
	}
	else if (!(DISPLAY_MODE >= 0 && DISPLAY_MODE <= 5)) 
	{
		DISPLAY_MODE = 3;
		printf("Executing %s with line option %d = %s.\n", argv[0], DISPLAY_MODE, lineOption[DISPLAY_MODE]);
	}
	else
	{
		printf("Executing %s with line option %d = %s.\n", argv[0], DISPLAY_MODE, lineOption[DISPLAY_MODE]);
	}




	//***********************//
	//	Delaunay Algorithm
	//***********************//
	clock_t begin, end;
	double time_spent;

	mygrid = create_grid( NB_VERTEX, NB_SIMPLEX + 1, &mydata, ZUNIF );
	compute_color_gradient(gradient_color, nb_gradient_val, 39.0, 131.0, 29.0, 215.0, 226.0, 214.0);

	begin = clock();

	if (OUTPUT_CONDITION == 'f')
		while ( mygrid->nb_simp < NB_SIMPLEX )
			delauney( mygrid );
	else if (OUTPUT_CONDITION == 's')
		while ( (mygrid->fdp->nb != 0) && (fabs(mygrid->fdp->table[1]->candidates->root->links[STD][FWD]->zdist) >= mygrid->zmax*(100-GOF)/100.0 ) )
			delauney( mygrid );
	else		
		while (mygrid->fdp->nb != 0)
			delauney( mygrid );
	
	end = clock();

	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Execution time : %f sec\n", time_spent);
	printf("Points inserted : %d\nTriangles generated : %d\n", mygrid->nb_vertex_inserted, mygrid->nb_simp);
	printf("Delaunay stack maximum size : %d\n", mygrid->stack_max_size[DEL]);
	printf("New stack maximum size : %d\n", mygrid->stack_max_size[NEW]);
	printf("Mean number of comparaisons / log(m) : %f\n", mygrid->fdp->nb_comp/(mygrid->nb_vertex_inserted*log(mygrid->nb_vertex_inserted)));


	//***********************//
	// 		Display
	//***********************//

	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);

	glutInitWindowPosition(5,5);  
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);  

	glutCreateWindow("Delaunay grid");  
	
	gluOrtho2D(minX-margin, maxX+margin, minY-margin, maxY+margin);

	glutIdleFunc(display);
	glutDisplayFunc(display); 
	glutReshapeFunc(changeSize); 

	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LINE_SMOOTH);

	glutSetKeyRepeat( GLUT_KEY_REPEAT_ON );
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	if (DISPLAY_MODE != 5)
	{
		glutMainLoop();  
	}
		
	return EXIT_SUCCESS; 
}