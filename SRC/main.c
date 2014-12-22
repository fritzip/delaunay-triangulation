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

#include "main.h"
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

int twoD = 0; 
int GRAD = 0;

float rho = 2.0f;
float xrot = -40.0f;
float zrot = 0.0f;

int xOrigin = -1, yOrigin = -1;


Grid* mygrid;

typedef struct color
{
	double r,g,b;
} Color ;

int nbval = 100;
Color gradient_color[100] = {};
/*----------------------------------------------------------------------------------*/
//                                  Graphic functions
/*----------------------------------------------------------------------------------*/

Color getcol(double z)
{
	// printf("grad = %d\n",(int)floor(z*nbval));
	// if (z < 0.01)
	// {
	// 	Color col = {55.0, 40.0, 115.0};
	// 	return col;
	// }
	if (GRAD)
		return gradient_color[(int)floor(z*nbval)];
	else
	{
		Color col = {0.0, 255.0, 0.0};
		return col;
	}
}

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
		twoD = 1 - twoD;
	if (key == 'c')
		GRAD = 1 - GRAD;
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
		case GLUT_KEY_UP : xrot -= 2; break; //y+=0.01f; break;  // xOrigin = xx; break;
		case GLUT_KEY_DOWN : xrot += 2; break; //y-=0.01f; break;
		case GLUT_KEY_LEFT : zrot += 2; break;
		case GLUT_KEY_RIGHT : zrot -= 2; break;
	}

		// case GLUT_KEY_UP : glRotated(0.05, 0.0f, 1.0f, 0.0f); break;
		// case GLUT_KEY_DOWN : glRotated(-0.05, 0.0f, 1.0f, 0.0f); break; //y-=0.01f; break;
		// case GLUT_KEY_LEFT : glRotated(0.05, 0.0f, 0.0f, 1.0f); break;
		// case GLUT_KEY_RIGHT : glRotated(-0.05, 0.0f, 0.0f, 1.0f); break;	
} 

void releaseKey(int key, int x, int y) 
{  
	// switch (key) {
	// 	 case GLUT_KEY_UP :
	// 	 case GLUT_KEY_DOWN : deltaMoveZ = 0; break;
	// 	 // case GLUT_KEY_LEFT : deltaAngle = 0; break;
	// }
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

// void GlutShade(GLfloat r,GLfloat v,GLfloat b)
// {
// 	// Couleur sans lumieres
// 	glColor3f(0.8,0.9,0.6);

// 	// Couleur avec lumieres
// 	GLfloat color[4];

// 	// La couleur diffuse sera egale a 25% de la couleur
// 	color[0]=0.75f*r;
// 	color[1]=0.75f*v;
// 	color[2]=0.75f*b;
// 	color[3]=1.0;

// 	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

// 	// La couleur ambiante sera egale a 25% de la couleur
// 	color[0]=0.25f*r;
// 	color[1]=0.25f*v;
// 	color[2]=0.25f*b;
// 	color[3]=1.0;

// 	glMaterialfv(GL_FRONT, GL_AMBIENT, color); // GL_AMBIENT_AND_DIFFUSE

// 	color[0]=1.0f;
// 	color[1]=0.0f;
// 	color[2]=0.0f;
// 	color[3]=1.0;

// 	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, color);
// }
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
		// printf("%f \n", r2);
	}
}

void display (void)
{
	compute_pos();

	glColor3f(0.0, 0.0, 0.0);

	// Clear Color and Depth Buffers
	// glClear(GL_COLOR_BUFFER_BIT);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// GlutShade(0.0, 1.0, 0.0);
	
	// Reset transformations
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -rho);

	glPushMatrix();
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(zrot, 0.0, 0.0, 1.0); 

	glTranslatef(-0.5, -0.5, -0.5);

	if (twoD)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		// glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEgit IGHT, 0.0, -1.0, 10.0);
		gluOrtho2D(minX-margin, maxX+margin, minY-margin, maxY+margin);

		glMatrixMode(GL_MODELVIEW);
		//glPushMatrix();        ----Not sure if I need this
		glLoadIdentity();
		glDisable(GL_CULL_FACE);
	}

	for (int i = 1; i <= mygrid->fdp->nb; i++)
	{
		glBegin(display_mode);
		for(int j = 0; j < 3; j++)
		{
			double z = mygrid->fdp->table[i]->sommet[j]->coords[2];
			Color col = getcol(z);
			glColor4f(col.r/255, col.g/255, col.b/255, 1);
			// printf("r = %f, g = %f, b = %f\n", col.r, col.g, col.b);
			glVertex3f(mygrid->fdp->table[i]->sommet[j]->coords[0], mygrid->fdp->table[i]->sommet[j]->coords[1], z);
		}
		glEnd();
	}	

	if (twoD)
	{
		// Making sure we can render 3d again
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		//glPopMatrix();        ----and this?
	}
	glPopMatrix();


	// // affiche_grid(mygrid, 0.0, 1.0, 0.0);

	// glFlush();
	glutSwapBuffers();   // swapping image buffer for double buffering
	// glutPostRedisplay();
}

/*----------------------------------------------------------------------------------*/
//                                  Main
/*----------------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	srand (time(NULL));

	int NB_VERTEX = 1000;
	char *input_file = NULL;

	int opt;
	while ((opt = getopt (argc, argv, "c:n:i:")) != -1)
		switch (opt)
		{
			case 'n':
				sscanf(optarg, "%d", &NB_VERTEX);
				break;
			case 'c':
				sscanf(optarg, "%d", &display_mode);
				break;
			case 'i':
				input_file = optarg;
				break;
			case '?':
				if (optopt == 'c' || optopt == 'n' || optopt == 'i')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\%x'.\n", optopt);
				printf("Usage: %s -c DISPLAY_MODE{0,1,2,3} -n NB_VERTEX -i INPUT_FILE \n", argv[0]);
				return 1;
			default:
				abort();
		}

	int NB_SIMPLEX = ( 2*(NB_VERTEX+4 - 1) - 4 );

	printf ("n = %d, c = %d, i=%s\n", NB_VERTEX, display_mode, input_file);

	PGMData mydata = {};

	if (input_file != NULL)
	{
		char command[300] = {0};

		sprintf(command, "x=%s; convert $x ${x%%.*}.pgm", input_file);
		// printf("%s\n", command);
		system(command);

		char *pFile = strrchr(input_file, '/');
		pFile = pFile == NULL ? input_file : pFile+1;
		// change extension
		char *pExt = strrchr(pFile, '.');
		if (pExt != NULL)
			strcpy(pExt, ".pgm");
		else
			strcat(pFile, ".pgm");


		printf("input = %s\n", input_file);
		readPGM(input_file, &mydata);
	}

	if (!(display_mode >= 0 && display_mode <= 4)) display_mode = 3;
	printf("Executing %s with line option %d = %s.\n", argv[0], display_mode, lineOption[display_mode]);

	mygrid = create_grid( NB_VERTEX, NB_SIMPLEX + 1, OFFSET, &mydata );
	while (mygrid->fdp->table[1]->candidates->length != 0)
	{
		delauney( mygrid );
	}


	// Display
	glutInit(&argc, argv);  
	// glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
	// glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);



	glutInitWindowPosition(5,5);  
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);  

	glutCreateWindow("Delaunay grid");  
	
	gluOrtho2D(minX-margin, maxX+margin, minY-margin, maxY+margin);

	glutIdleFunc(display);
	glutDisplayFunc(display); 
	glutReshapeFunc(changeSize); 

	glEnable (GL_DEPTH_TEST);
    // glEnable (GL_LIGHTING);
    // glEnable (GL_LIGHT0);

	compute_color_gradient(gradient_color, nbval, 39.0, 131.0, 29.0, 215.0, 226.0, 214.0);

	// for (int i = 0; i < nbval; ++i)
	// {
	// 	printf("%f \n", gradient_color[i].r);
	// }
	glutSetKeyRepeat( GLUT_KEY_REPEAT_ON );
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// glutFullScreen();  

	glutMainLoop();  
		
	return EXIT_SUCCESS; 
}



























void affiche_dll(Dllist *dll, int LNK, int GL_DRAW_STYLE, double r, double g, double b)
{
	glBegin(GL_DRAW_STYLE);
	glColor3f(r, g, b);

	Vertex *current = dll->root->links[LNK][FWD];

	for(int i = 0; i < dll->length; i++)
	{
		glVertex2f(current->coords[0], current->coords[1]);
		current = current->links[LNK][FWD];
	}

	glEnd();
}

void affiche_simplex(Simplex *simp, double r, double g, double b)
{
	// glBegin(lineOption[displayChoice]);
	glColor3f(r, g, b);

	for(int i = 0; i < 3; i++)
	{
		glVertex3f(simp->sommet[i]->coords[0], simp->sommet[i]->coords[1], simp->sommet[i]->coords[2]);
	}

	// glEnd();

	// scanf("%d", &go_on);
	// glutSwapBuffers();   // swapping image buffer for double buffering
	// glutPostRedisplay();
	// glFlush();


	// printf("len = %d\n", simp->candidats->length[STD]);
	// glBegin(GL_POINTS);

	// Vertex *current = simp->candidats->root->links[STD][FWD];
	// for (int i = 0; i < simp->candidats->length[STD]; i++)
	// {
	// 	glVertex3f(current->coords[0], current->coords[1], current->coords[2]);
	// 	current = current->links[STD][FWD];
	// }

	// glEnd();
}

void affiche_grid(Grid *grid, double r, double g, double b)
{
	// print_fdp(grid->fdp);
	// printf("size = %d\n", grid->fdp->nb);
	for (int i = 1; i <= grid->fdp->nb; i++)
	{
		affiche_simplex( grid->fdp->table[i], r, g, b );
	}	
}