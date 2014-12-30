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
GLenum display_mode = 3;
char *lineOption[] = { "GL_POINTS", "GL_LINES", "GL_LINE_STRIP", "GL_LINE_LOOP", "GL_POLYGON"};

int SCREEN_WIDTH = 1000, SCREEN_HEIGHT = 1000;

int TWO_D = 0; 
int GRAD = 0;
int NORMAL = 1;

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

Color getcol(double z)
{
	if (GRAD)
	{
		if (z < 0.01)
		{
			Color col = {55.0, 40.0, 115.0};
			return col;
		}
		else
			return gradient_color[(int)floor(z*nb_gradient_val)];
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
			display_mode = GL_LINE_LOOP;			
		}
		else 
		{
			NORMAL = 1;
			display_mode = GL_POLYGON;
		}
	}
	if (key == 'c')
		GRAD = 1 - GRAD;
	if (key == 'n')
		NORMAL = 1 - NORMAL;
	switch (key)
	{
		case '0': display_mode = GL_POINTS ; break;
		case '1': display_mode = GL_LINES ; break;
		case '2': display_mode = GL_LINE_STRIP ; break;
		case '3': display_mode = GL_LINE_LOOP ; break;
		case '4': display_mode = GL_POLYGON ; break;
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
	if (xrot < -180) xrot = -180; 
	if (zrot > 360) zrot -= 360;
	if (zrot < 0) zrot += 360;

	// printf("xrot = %f, zrot = %f, rho = %f\n", xrot, zrot, rho);
	// printf("phi = %f, theta = %f, rho = %f\n", phi, theta, rho);
	// printf("camx = %f, camy = %f, camz = %f\n", camx, camy, camz);
}



void display (void)
{
	compute_pos();

	glColor3f(0.0, 0.0, 0.0);

	// Clear Color and Depth Buffers
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -rho);

	glPushMatrix();
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(zrot, 0.0, 0.0, 1.0); 

	glTranslatef(-0.5, -0.5, -0.5);

	if (TWO_D)
	{ // 2D Display
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(minX-margin, maxX+margin, minY-margin, maxY+margin);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_CULL_FACE);
	}

	for (int i = 1; i <= mygrid->fdp->nb; i++)
	{
		glBegin(display_mode);
		Simplex *simp = mygrid->fdp->table[i];
		// Normal or not 
		if (NORMAL) glNormal3f(simp->na, simp->nb, simp->nc);
		else glNormal3f(1,1,1);

		for(int j = 0; j < 3; j++)
		{
			double z = simp->sommet[j]->coords[2];
			Color col = getcol(z);
			glColor4f(col.r/255, col.g/255, col.b/255, 1);
			glVertex3f(simp->sommet[j]->coords[0], simp->sommet[j]->coords[1], z);
		}
		glEnd();
	}	

	if (TWO_D)
	{ // 2D Display
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	glPopMatrix();

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
	char *INPUT_FILE = NULL;
	char OUTPUT_CONDITION;

	//***********************//
	//      Arg parser
	//***********************//
	int opt;
	while ((opt = getopt (argc, argv, "c:n:i:g:s:")) != -1)
		switch (opt)
		{
			case 'n':
				sscanf(optarg, "%d", &NB_VERTEX);
				break;			
			case 'g':
				sscanf(optarg, "%d", &GOF);
				OUTPUT_CONDITION = 'g';
				break;			
			case 's':
				sscanf(optarg, "%d", &NB_SIMPLEX);
				OUTPUT_CONDITION = 's';
				break;
			case 'c':
				sscanf(optarg, "%d", &display_mode);
				break;
			case 'i':
				INPUT_FILE = optarg;
				break;
			case '?':
				if (optopt == 'c' || optopt == 'n' || optopt == 'i' || optopt == 's' || optopt == 'g')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\%x'.\n", optopt);
				printf("Usage: %s ./delaunay [-i INPUT_FILE] [-n NB_PTS] [-s NB_SIMPX | -g GOF(%%)] [-c DISPLAY_MODE]\n cf. README.md for further information", argv[0]);
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
		printf("Maximum 1 output condition (2 given -s %d -g %d)\n", NB_SIMPLEX, GOF);
		printf("Usage: %s ./delaunay [-i INPUT_FILE] [-n NB_PTS] [-s NB_SIMPX | -g GOF(%%)] [-c DISPLAY_MODE]\n cf. README.md for further information", argv[0]);
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
		char command[300] = {0};

		sprintf(command, "x=%s; convert $x ${x%%.*}.pgm", INPUT_FILE);
		system(command);

		char *pFile = strrchr(INPUT_FILE, '/');
		pFile = pFile == NULL ? INPUT_FILE : pFile+1;
		// change extension
		char *pExt = strrchr(pFile, '.');
		if (pExt != NULL)
			strcpy(pExt, ".pgm");
		else
			strcat(pFile, ".pgm");

		printf("input = %s\n", INPUT_FILE);
		readPGM(INPUT_FILE, &mydata);
	}

		// Display mode
	if (!(display_mode >= 0 && display_mode <= 4)) display_mode = 3;

	printf ("n = %d, c = %d, i=%s\n", NB_VERTEX, display_mode, INPUT_FILE);
	printf("Executing %s with line option %d = %s.\n", argv[0], display_mode, lineOption[display_mode]);

	switch (display_mode)
	{
		case 0: display_mode = GL_POINTS ; break;
		case 1: display_mode = GL_LINES ; break;
		case 2: display_mode = GL_LINE_STRIP ; break;
		case 3: display_mode = GL_LINE_LOOP ; break;
		case 4: display_mode = GL_POLYGON ; break;
	}

	compute_color_gradient(gradient_color, nb_gradient_val, 39.0, 131.0, 29.0, 215.0, 226.0, 214.0);


	//***********************//
	//	Delaunay Algorithm
	//***********************//

	mygrid = create_grid( NB_VERTEX, NB_SIMPLEX + 1, OFFSET, &mydata, GOF/1000.f );

	if (OUTPUT_CONDITION == 's')
		while ( mygrid->fdp->nb < NB_SIMPLEX )
			delauney( mygrid );
	else if (OUTPUT_CONDITION == 'g')
		while ( fabs(mygrid->fdp->table[1]->candidates->root->links[STD][FWD]->zdist) > mygrid->gof )
			delauney( mygrid );
	else		
		while (mygrid->fdp->table[1]->candidates->length != 0)
			delauney( mygrid );
	

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

	glutSetKeyRepeat( GLUT_KEY_REPEAT_ON );
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutMainLoop();  
		
	return EXIT_SUCCESS; 
}