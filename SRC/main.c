/*! \file main.c
 * \author M. Sainlot & G. Schoder
 * \date 2014
 */
// #include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <GL/glut.h> 

#include "main.h"
#include "math_fn.h"

/*! variable externe permettant de lire les parametres sur le ligne de commande.*/
extern char *optarg;

/*! bascule pour autoriser ou interdire (0) les messages d'erreurs envoyes par getopt. */
extern int opterr;

/* Display option */
int displayChoice = 3;
char *lineOption[] = { "GL_POINTS", "GL_LINES", "GL_LINE_STRIP", "GL_LINE_LOOP", "GL_POLYGON"};



// angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's direction
float lx=0.0f, ly=-1.0f, lz=0.2f;

// XZ position of the camera
float x=0.5f, y=0.4f, z=0.5f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMoveZ = 0;
float deltaMoveY = 0;
// int xOrigin = 1;


Grid* mygrid;
int go_on;
/*----------------------------------------------------------------------------------*/
//                                  Graphic functions
/*----------------------------------------------------------------------------------*/
void changeSize(int w, int h) {

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
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);

  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
}


void processNormalKeys(unsigned char key, int xx, int yy) {   

        if (key == 27)
              exit(0);

} 

void pressKey(int key, int xx, int yy) {

       switch (key) {
             case GLUT_KEY_UP : deltaMoveZ = 0.5f; break; //y+=0.01f; break;  // xOrigin = xx; break;
             case GLUT_KEY_DOWN : deltaMoveZ = -0.5f; break; //y-=0.01f; break;
             // case GLUT_KEY_LEFT : deltaAngle = 0.5f; break;
       }
} 

void releaseKey(int key, int x, int y) {  

        switch (key) {
             case GLUT_KEY_UP :
             case GLUT_KEY_DOWN : deltaMoveZ = 0; break;
             // case GLUT_KEY_LEFT : deltaAngle = 0; break;
        }
} 


void mouseMove(int x, int y) {  

	// this will only be true when the left button is down
	// if (xOrigin >= 0) {

	// update deltaAngle
	deltaAngle = x * 0.005f;

	// update camera's direction
	lx = sin(angle + deltaAngle);
	ly = -cos(angle + deltaAngle);
	// lz = ;
	// }
}

// void mouseButton(int button, int state, int x, int y) {

//   // // only start motion if the left button is pressed
//   // if (button == GLUT_LEFT_BUTTON) {

//   //   // when the button is released
//   //  if (state == GLUT_UP) {
//   //     angle += deltaAngle;
//   //     xOrigin = -1;
//   //   }
//   //   else  {// state = GLUT_DOWN
//       xOrigin = x;
//     // }
//   // }
// }

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
	glBegin(GL_LINE_LOOP);
	glColor3f(r, g, b);

	for(int i = 0; i < 3; i++)
	{
		glVertex3f(simp->sommet[i]->coords[0], simp->sommet[i]->coords[1], simp->sommet[i]->coords[2]);
	}

	glEnd();

	// scanf("%d", &go_on);
	// glutSwapBuffers();   // swapping image buffer for double buffering
	// glutPostRedisplay();
	glFlush();


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

void computePos(float deltaMove) 
{
	x += deltaMove * lx * 0.1f;
	y += deltaMove * ly * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void display (void)
{
	// if (deltaMoveZ) computePos(deltaMoveZ);

	glColor3f(0.0, 0.0, 0.0);
	// glClear(GL_COLOR_BUFFER_BIT);
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT);


	
	while (mygrid->fdp->table[1]->candidates->length != 0)
	{
		delauney( mygrid );
	}
		// // Reset transformations
		// glLoadIdentity();
		// // Set the camera
		// gluLookAt(  x, y, z,
		// 	lx, ly, lz, 
		// 	0.0f, 0.0f,  1.0f);

	// affiche_grid(mygrid, 0.0, 1.0, 0.0);

	glFlush();
	// glutSwapBuffers();   // swapping image buffer for double buffering
	// glutPostRedisplay();
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


	mygrid = create_grid( NB_VERTEX, NB_SIMPLEX + 1, OFFSET );

	printf("%d\n", NB_SIMPLEX+1);

	// while (mygrid->fdp->table[1]->candidates->length != 0)
	// {
	// 	delauney( mygrid );
	// }

	// while (mygrid->fdp->table[1]->candidates->length != 0){
		// printf("\n\n\n\n ******* NEW DELAUNEY *******\n\n\n\n");
		// delauney(mygrid, mygrid->fdp->table[1]);
		// printf("main : %d\n", mygrid->fdp->table[1]->candidates->length );
	// }
	// print_simplex(mygrid->fdp->table[1]);
	// print_dll(mygrid->fdp->table[1]->candidates, STD);
	printf("fdpnb = %d\n", mygrid->fdp->nb );

	assert(displayChoice >= 0 && displayChoice <= 4);
	printf("Executing %s with line option %d = %s.\n", argv[0], displayChoice, lineOption[displayChoice]);

	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_SINGLE);  

	glutInitWindowPosition(5,5);  
	glutInitWindowSize(maxX*1000,maxY*1000);  

	glutCreateWindow("Delaunay grid");  
	
	gluOrtho2D(minX-margin, maxX+margin, minY-margin, maxY+margin);

	glutIdleFunc(display);
	glutDisplayFunc(display); 
	// glutReshapeFunc(changeSize); 

	// glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	// glutMouseFunc(mouseButton);
	// glutPassiveMotionFunc(mouseMove);

	// glutFullScreen();  

	// glEnable(GL_DEPTH_TEST);


	glutMainLoop();  
		
	return EXIT_SUCCESS; 
}