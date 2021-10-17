/********************************************************/
/*                     projet.cpp                                                 */
/********************************************************/
/*                Affiche d'un dragon en 3D                     */
/********************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif
#include <cstdlib>
#include <GL/freeglut.h>

#include <cmath>

float camZoom = 5;
float camSpeed = 1;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche, int x, int y);
void reshape(int x, int y);
void idle();
void mouse(int bouton, int etat, int x, int y);
void mousemotion(int x, int y);
void zoom(int signe);

int main(int argc,char **argv)
{
	/* initialisation de glut et creation
	 de la fenetre */
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("cube");

	/* Initialisation d'OpenGL */
	glClearColor(0.0,0.0,0.0,0.0);
	glColor3f(1.0,1.0,1.0);
	glPointSize(2.0);
	glEnable(GL_DEPTH_TEST);

	/* enregistrement des fonctions de rappel */
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);

	/* Entree dans la boucle principale glut */
	glutMainLoop();
	return 0;
}

void dessinDragon()
{
	glPushMatrix();



    glPopMatrix();
}

void affichage()
{
	/* effacement de l'image avec la couleur de fond */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);

	glLoadIdentity();
	glLookAt(0, 0, 0, 0, 0, 0, 0, 2, camZoom);	// TODO : check si ça marche

	dessinDragon();

    //Repère
    //axe x en rouge
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(1, 0,0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
    	glColor3f(0.0,1.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 1,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    	glColor3f(0.0,0.0,1.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 0,1.0);
    glEnd();

	glFlush();

	//On echange les buffers
	glutSwapBuffers();
}

void clavier(unsigned char touche, int x, int y)
{
	switch (touche)
	{
		case 'p': /* affichage du carre plein */
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glutPostRedisplay();
		  break;
		case 'f': /* affichage en mode fil de fer */
		    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		    glutPostRedisplay();
			break;
		case 's' : /* Affichage en mode sommets seuls */
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			glutPostRedisplay();
			break;
		case 'd':
			glEnable(GL_DEPTH_TEST);
			glutPostRedisplay();
			break;
		case 'D':
			glDisable(GL_DEPTH_TEST);
			glutPostRedisplay();
			break;
		case 'a':
			glPolygonMode(GL_FRONT,GL_FILL);
			glPolygonMode(GL_FRONT,GL_LINE);
			glutPostRedisplay();
			break;
		case 'z':
			zoom(1);
			break;
		case 'Z':
			zoom(-1);
			break;
		case 'q' : /*la touche 'q' permet de quitter le programme */
			exit(0);
	}
}

void reshape(int x, int y)
{
	if (x<y)
		glViewport(0,(y-x)/2,x,x);
	else
		glViewport((x-y)/2,0,y,y);
}

void zoom(int signe)
{
	camZoom += signe * camSpeed;
}

void mouse(int button, int state, int x, int y)
{

}

void mousemotion(int x, int y)
{

}