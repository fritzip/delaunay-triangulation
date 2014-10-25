#include <unistd.h>  
#include <math.h>  
#include <assert.h>
#include <GL/glut.h> 


/*! variable externe permettant de lire les parametres sur le ligne de commande.*/
extern char *optarg;

/*! bascule pour autoriser ou interdire (0) les messages d'erreurs envoyes par getopt. */
extern int opterr;

/* Display option */
int displayChoice = 3;
char *lineOption[] = { "GL_POINTS", "GL_LINES", "GL_LINE_STRIP", "GL_LINE_LOOP", "GL_POLYGON"};


/*----------------------------------------------------------------------------------*/
//                                  Globals
/*----------------------------------------------------------------------------------*/

int NB_VERTEX = 30;

const double minX = 0,
			maxX = 1000,
			minY = 0,
			maxY = 1000,
			margin = 10;




int randn(int min, int max)
{
	return min+rand()%(max-min+1);
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
