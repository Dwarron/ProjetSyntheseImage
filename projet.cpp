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
char presse;
int anglex,angley,x,y,xold,yold;
float mouthAngle;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche, int x, int y);
void reshape(int x, int y);
void idle();
void mouse(int bouton, int etat, int x, int y);
void mousemotion(int x, int y);
void zoom(int signe);
void mouthAnim();

int main(int argc,char **argv)
{
	/* initialisation de glut et creation
	 de la fenetre */
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Dragon");

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
	glutIdleFunc(mouthAnim);

	/* Entree dans la boucle principale glut */
	glutMainLoop();
	return 0;
}

//pour la tete on peut garder c'est valeur
//mais crée des valeur pour la bouche et la crete du dragon pour s'y retrouver dans les translate et les scale

float Ltete = 2;
float ltete = 2;
float htete = 2;

void dessinDragon()
{
	glPushMatrix();
        //tete du dragon
        glPushMatrix();
        glColor3f(150,150,0);
        glScalef(Ltete, ltete, htete);
        glutSolidCube(1);
        glPopMatrix();

        //machoire superieur
        glPushMatrix();
        glColor3f(0,0,250);
        glTranslated(Ltete, -ltete/12.0, 0);
        glScalef(Ltete, ltete/4.0, htete/1.20);
        glutSolidCube(1);
        glPopMatrix();

        //machoire inferieur
        glPushMatrix();
            glTranslated(Ltete/2.0, -ltete/3.0, 0);
            glRotatef(mouthAngle, 0, 0, 1);
            glTranslated(-Ltete/2.0, ltete/3.0, 0);

            glPushMatrix();
            glColor3f(0,250,0);
            glTranslated(Ltete, -ltete/3.0, 0);
            glScalef(Ltete, ltete/4.0, htete/1.20);
            glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        //Crete crane gauche
        glPushMatrix();
        glColor3f(255,0,255);
        glTranslated(0, ltete - ltete/3.0, Ltete/4.0);
        glScalef(Ltete/2.0, ltete/3.0, htete/8.0);
        glutSolidCube(1);
        glPopMatrix();

        //Crete crane droite
        glPushMatrix();
        glColor3f(55,0,55);
        glTranslated(0, ltete - ltete/3.0, -Ltete/4.0);
        glScalef(Ltete/2.0, ltete/3.0, htete/8.0);
        glutSolidCube(1);
        glPopMatrix();

        //Crete nez gauche
        glPushMatrix();
        glColor3f(255,0,0);
        glTranslated(Ltete, ltete/6.0 , Ltete/6.0);
        glScalef(Ltete/2.0, ltete/4.0, htete/6.0);
        glutSolidCube(1);
        glPopMatrix();

        //Crete nez droite
        glPushMatrix();
        glColor3f(255,0,0);
        glTranslated(Ltete, ltete/6.0 , -Ltete/6.0);
        glScalef(Ltete/2.0, ltete/4.0, htete/6.0);
        glutSolidCube(1);
        glPopMatrix();

    glPopMatrix();
}

void mouthAnim()
{
    bool stop = false;

    if(mouthAngle >= -30.0 && stop == false)
        mouthAngle -= 0.02;

    else if(mouthAngle < -30 && stop == false)
        stop = true;

    else if(mouthAngle < 0.0 && stop == true)
        mouthAngle += 0.02;

    else
        stop = false;

    glutPostRedisplay();
}

void affichage()
{
	/* effacement de l'image avec la couleur de fond */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);

	//glLoadIdentity();
	//gluLookAt(0, 0, 0, 0, 0, 0, 0, 2, camZoom);	// TODO : check si ça marche

    glLoadIdentity();
    glOrtho(4,-4,-4,4,-4,4);
    glRotatef(angley,1.0,0.0,0.0);
    glRotatef(anglex,0.0,1.0,0.0);

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
        case 'm':
            mouthAngle -= 1;
			glutPostRedisplay();
			break;
        case 'l':
            mouthAngle += 1;
			glutPostRedisplay();
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

void mouse(int button, int state,int x,int y)
{
    /* si on appuie sur le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
    }
    /* si on relache le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
{
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
     en fonction de la position actuelle de la souris et de la derniere
     position sauvegardee */
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
}

void zoom(int signe)
{
	camZoom += signe * camSpeed;
}

/*void mouse(int button, int state, int x, int y)
{

}

void mousemotion(int x, int y)
{

}*/
