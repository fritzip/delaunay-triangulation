/*! \file main.c
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <GL/glut.h> 

#include "main.h"
#include "math_fn.h"

// angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's direction
float lx=0.0f,ly=-1.0f;

// XZ position of the camera
float x=0.5f, y=0.4f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;


Grid* mygrid;

/*----------------------------------------------------------------------------------*/
//                                  Graphic functions
/*----------------------------------------------------------------------------------*/


void processNormalKeys(unsigned char key, int xx, int yy) {   

        if (key == 27)
              exit(0);

} 

void pressKey(int key, int xx, int yy) {

       switch (key) {
             case GLUT_KEY_UP : y+=0.05f; break; //deltaMove = 0.5f; break; // xOrigin = xx; break;
             case GLUT_KEY_DOWN : y-=0.05f; break; //deltaMove = -0.5f; break;
       }
} 

void releaseKey(int key, int x, int y) {  

        switch (key) {
             case GLUT_KEY_UP :
             case GLUT_KEY_DOWN : deltaMove = 0;break;
        }
} 


void mouseMove(int x, int y) {  

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

	// update deltaAngle
	deltaAngle = (x - xOrigin) * 0.001f;

	// update camera's direction
	lx = sin(angle + deltaAngle);
	ly = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

  // only start motion if the left button is pressed
  if (button == GLUT_LEFT_BUTTON) {

    // when the button is released
   if (state == GLUT_UP) {
      angle += deltaAngle;
      xOrigin = -1;
    }
    else  {// state = GLUT_DOWN
      xOrigin = x;
    }
  }
}

void affiche_dll(Dllist *dll, int LNK, int GL_DRAW_STYLE, double r, double g, double b)
{
	glBegin(GL_DRAW_STYLE);
	glColor3f(r, g, b);

	Vertex *current = dll->root->links[LNK][FWD];

	for(int i = 0; i < dll->length[LNK]; i++)
	{
		glVertex2f(current->coords[0], current->coords[1]);
		current = current->links[LNK][FWD];
	}

	glEnd();
}

void affiche_simplex(Simplex *simp, double r, double g, double b)
{
	glBegin(GL_LINE_LOOP);
	glColor3f(r, g, b);

	for(int i = 0; i < 3; i++)
	{
		glVertex3f(simp->sommet[i]->coords[0], simp->sommet[i]->coords[1], simp->sommet[i]->coords[2]);
	}

	glEnd();

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
	for (int i = 1; i <= grid->fdp->nb; i++)
		{
			affiche_simplex( grid->fdp->table[i], r, g, b );
		}	
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	y += deltaMove * ly * 0.1f;
}

void display (void)
{
	// if (deltaMove) computePos(deltaMove);

	glColor3f(0.0, 0.0, 0.0);
	// glClear(GL_COLOR_BUFFER_BIT);
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Reset transformations
	// 	glLoadIdentity();
	// 	// Set the camera
	// 	gluLookAt(  x, y, 0.5f,
	// 		x+lx, y+ly, 0.7f, 
	// 		0.0f, 0.0f,  1.0f);
	affiche_grid(mygrid, 0.0, 1.0, 0.0);
	// affiche_simplex(mygrid->fdp->table[2], 0.0, 1.0, 0.0);

	// glFlush();
	glutSwapBuffers();   // swapping image buffer for double buffering
	glutPostRedisplay();
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
			case 'c': 
				if ((sscanf(optarg, "%d", &displayChoice) == 1) && displayChoice >= 0 && displayChoice <= 4)
				break;
			case '?': /* value returned by getopt if invalid option chosen */
			default : 
				printf("Usage: %s -cX, with X = 1, 2, 3 or 4.\n", argv[0]);
				displayChoice = 0;
				break;
		}
	}	


	mygrid = create_grid( NB_VERTEX, NB_SIMPLEX + 1 );

	while (mygrid->fdp->table[1]->candidats->length[STD] != 0)
		split_in_3(mygrid->fdp);

	assert(displayChoice >= 0 && displayChoice <= 4);
	printf("Executing %s with line option %d = %s.\n", argv[0], displayChoice, lineOption[displayChoice]);

	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);  

	glutInitWindowPosition(5,5);  
	glutInitWindowSize(maxX*500,maxY*500);  

	glutCreateWindow("Delaunay grid");  
	
	gluOrtho2D(minX-margin, maxX+margin, minY-margin, maxY+margin);

	glutIdleFunc(display);
	glutDisplayFunc(display);  

	// glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutFullScreen();  

	glEnable(GL_DEPTH_TEST);


	glutMainLoop();  
		
	return EXIT_SUCCESS; 
}