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
#include <cstdio>
#include <cstdlib>
#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>
#include "Texture.h"
//#include "Cylindre.cpp"
#include "Sphere.h"

#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif

#include <cmath>

float camZoom = 20;
float camZoomSpeed = 0.1;
float camMoveSpeed = 0.3;
char presse;
float anglex,angley,x,y,xold,yold, mouveFireBall = 0;
float mouthAngle = 0.0, mouthSpeed = 0.02;
Texture customTextures[3]; //tableau de toutes les textures

bool touchesPressees[6];

/* Prototype des fonctions */
void affichage();
void clavierDown(unsigned char touche, int x, int y);
void clavierUp(unsigned char touche, int x, int y);
void clavierSpecialDown(int touche, int x, int y);
void clavierSpecialUp(int touche, int x, int y);
void reshape(int x, int y);
void idle();
void mouse(int bouton, int etat, int x, int y);
void mousemotion(int x, int y);
void zoom(int signe);
void mouthAnim();

float dimTete = 2;

void teteDragon()
{
    customTextures[0].activer();

	glPushMatrix();
        glTranslated(0, 8, 4); //a determiné quand on aura le cout du dragon

        //texture qui va etre appliquer sur la tete du dragon pour avoir la tete noir et les yeux violet
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);   glVertex3f(1, 1, 1);
        glTexCoord2f(0.0,1.0);   glVertex3f(1,-1, 1);
        glTexCoord2f(1.0,1.0);   glVertex3f(-1,-1, 1);
        glTexCoord2f(1.0,0.0);   glVertex3f(-1, 1, 1);
        glEnd();

        //tete du dragon
        glPushMatrix();
        glColor3f(0.5,0.5,0);
        glScalef(dimTete, dimTete, dimTete);
        glutSolidCube(1);
        glPopMatrix();

        //machoire superieur
        glPushMatrix();
        glColor3f(0,0,0.9);
        glTranslated(0, -dimTete/12.0, dimTete);
        glScalef(dimTete/1.2, dimTete/4.0, dimTete);
        glutSolidCube(1);
        glPopMatrix();

        //machoire inferieur
        glPushMatrix();
            glTranslated(0, -dimTete/3.0, dimTete/2.0);
            glRotatef(mouthAngle, 1, 0, 0);
            glTranslated(0, dimTete/3.0, -dimTete/2.0);

            glPushMatrix();
            glColor3f(0,0.8,0);
            glTranslated(0, -dimTete/3.0, dimTete - dimTete/16.0);
            glScalef(dimTete/1.2, dimTete/4.0, dimTete + dimTete/8.0); // plus grand que la machoire superieur afin d'avoir une animation plus propre
            glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        //Crete crane gauche
        glPushMatrix();
        glColor3f(1,0,1);
        glTranslated(-dimTete/4.0, dimTete - dimTete/3.0, 0);
        glScalef(dimTete/8.0, dimTete/3.0, dimTete/2.0);
        glutSolidCube(1);
        glPopMatrix();

        //Crete crane droite
        glPushMatrix();
        glColor3f(0.15,0,0.15);
        glTranslated(dimTete/4.0, dimTete - dimTete/3.0, 0);
        glScalef(dimTete/8.0, dimTete/3.0, dimTete/2.0);
        glutSolidCube(1);
        glPopMatrix();

        //Crete museau gauche
        glPushMatrix();
        glColor3f(1,0,0);
        glTranslated(-dimTete/6.0, dimTete/12.0 , dimTete);
        glScalef(dimTete/8.0, dimTete/8.0, dimTete/2.0);
        glutSolidCube(1);
        glPopMatrix();

        //Crete museau droite
        glPushMatrix();
        glColor3f(1,0,0);
        glTranslated(dimTete/6.0, dimTete/12.0 , dimTete);
        glScalef(dimTete/6.0, dimTete/8.0, dimTete/2.0);
        glutSolidCube(1);
        glPopMatrix();

    glPopMatrix();
    customTextures[0].desactiver();
}

float CorpDragon = 8;


void JambeEtCorpDragon()
{
    glPushMatrix();

    //buste du dragon
    glPushMatrix();
    glColor3f(1,0.8,0);
    glTranslated(0, 2, 0);
    glScalef(CorpDragon, CorpDragon/2.0, CorpDragon*3);
    glutSolidCube(1);
    glPopMatrix();


    glPopMatrix();
}

float largeurAiles = 30;
float longueurAiles = 10;

void ailesDragon()
{
    glPushMatrix();
        glTranslatef(0, CorpDragon, 0);

        //aile droite
        glPushMatrix();
        glTranslatef(CorpDragon*0.5, 0, 0);

        //partie souple
        glBegin(GL_POLYGON);
            glVertex3f(0, 0, 0);
            glVertex3f(largeurAiles / 4.0, 0, -longueurAiles*5.0/6.0);
            glVertex3f(largeurAiles*3.0/5.0, 0, -longueurAiles);
            glVertex3f(largeurAiles*7.0/8.0, 0, -longueurAiles*9.0/10.0);
            glVertex3f(largeurAiles, 0, 0);
        glEnd();

        //armature
        glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
            glTranslatef(largeurAiles * 0.25, 0, 0);
            glPushMatrix();
                glScalef(largeurAiles * 0.5, 1, 1);
                glutSolidCube(1);
            glPopMatrix();

            glTranslatef(largeurAiles * 0.5, 0, 0);
            glPushMatrix();
                glScalef(largeurAiles * 0.5, 0.5, 0.5);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        glPopMatrix();

        //aile gauche
        glPushMatrix();
        glTranslatef(-CorpDragon*0.5, 0, 0);

        glColor3f(0, 0, 1);

        //partie souple
        glBegin(GL_POLYGON);
            glVertex3f(0, 0, 0);
            glVertex3f(-largeurAiles / 4.0, 0, -longueurAiles*5.0/6.0);
            glVertex3f(-largeurAiles*3.0/5.0, 0, -longueurAiles);
            glVertex3f(-largeurAiles*7.0/8.0, 0, -longueurAiles*9.0/10.0);
            glVertex3f(-largeurAiles, 0, 0);
        glEnd();

        //armature
        glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
            glTranslatef(-largeurAiles * 0.25, 0, 0);
            glPushMatrix();
            glScalef(-largeurAiles * 0.5, 1, 1);
            glutSolidCube(1);
            glPopMatrix();

            glTranslatef(-largeurAiles * 0.5, 0, 0);
            glPushMatrix();
            glScalef(-largeurAiles * 0.5, 0.5, 0.5);
            glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        glPopMatrix();

    glPopMatrix();
}

void Solar()
{
    glColor3f(0.8,0.6,0); //en attendant de mettre une texture de soleil, le soleil est jaune
    glTranslated(10, 20 , 10);
    Sphere test2(40,20,5);
}

int main(int argc,char **argv)
{
    /* initialisation de glut et creation de la fenetre */
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

    /* Chargement de la texture qui represente la tete du Dragon*/
    customTextures[0] = Texture("./Ender_dragon_skinHead.jpg");

	/* enregistrement des fonctions de rappel */
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavierDown);
	glutKeyboardUpFunc(clavierUp);
	glutSpecialFunc(clavierSpecialDown);
	glutSpecialUpFunc(clavierSpecialUp);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);
	glutIdleFunc(mouthAnim);

	/* Entree dans la boucle principale glut */
	glutMainLoop();
	return 0;
}

void affichage()
{
    if(touchesPressees[0])
    {
        if(touchesPressees[1])  //Z
            zoom(-1);
        else                    //z
            zoom(1);
    }
    if(touchesPressees[2])
    {
        anglex=anglex+camMoveSpeed;
    }
    if(touchesPressees[3])
    {
        anglex=anglex-camMoveSpeed;
    }
    if(touchesPressees[4])
    {
        angley=angley-camMoveSpeed;
    }
    if(touchesPressees[5])
    {
        angley=angley+camMoveSpeed;
    }

	/* effacement de l'image avec la couleur de fond */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glShadeModel(GL_SMOOTH);

	/*glLoadIdentity();
	gluLookAt(0.0, 0.0, 20.5, 0.0, 0.0, 0.0, 0.0, 20.0, camZoom);	// TODO : check si ça marche
	glRotatef(angley,1.0,0.0,0.0);
    glRotatef(anglex,0.0,1.0,0.0);*/

    /* Parametrage du placage de textures */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);// Comment la texture interagit avec la couleur du pixel

    /* Chargement de la texture qui represente la crete du Dragon*/
    //loadJpegImage("./Ender_dragon_skinCrete.jpg");

    /* Parametrage du placage de textures */
   /* glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);// Comment la texture interagit avec la couleur du pixel
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, image);// spécifier la texture avec l’image
    glEnable(GL_TEXTURE_2D);*/

    teteDragon();
    JambeEtCorpDragon();
    ailesDragon();
    Solar();

    //Cylindre test(10, 1, 0);

    glLoadIdentity();
    glOrtho(camZoom,-camZoom,-0.4 * camZoom, 1.6 * camZoom ,-camZoom,camZoom);
    glRotatef(angley,1.0,0.0,0.0);
    glRotatef(anglex,0.0,1.0,0.0);

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

    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    GLfloat dif_0[] = {0.1, 0.1, 0.1, 1.0};  //composante diffuse rouge
    GLfloat amb_0[] = {1.0, 0.0, 0.0, 1.0};  //composante ambiante rouge
    GLfloat spec_0[] = {1.0, 1.0, 1.0, 1.0}; //composante spéculaire blanche
    //GLfloat position_source[] = {1.0, 2.0, 3.0, 1.0};  //coordhomogènes : position
    GLfloat direction_source0[] = {1.0, 2.0, 3.0, 0.0};

    glLightfv(GL_LIGHT0, GL_POSITION, direction_source0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb_0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif_0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec_0);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glPopMatrix();*/
	glFlush();

	//On echange les buffers
	glutSwapBuffers();
}

void mouthAnim()
{
    mouthAngle -= mouthSpeed;

    if(mouthAngle >= 30 || mouthAngle <= 0)
        mouthSpeed = mouthSpeed * -1;

    glutPostRedisplay();
}

void clavierDown(unsigned char touche, int x, int y)
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
		case 'z': //zoomer sur le dragon
		case 'Z':
            touchesPressees[0] = true;
			break;
		case 'q' : /*la touche 'q' permet de quitter le programme */
			exit(0);
	}
    if(glutGetModifiers() & GLUT_ACTIVE_SHIFT)
        touchesPressees[1] = true;
    else
        touchesPressees[1] = false;
}

void clavierUp(unsigned char touche, int x, int y)
{
	switch (touche)
	{
		case 'z': //zoomer sur le dragon
		case 'Z':
            touchesPressees[0] = false;
			break;
	}
    if(glutGetModifiers() & GLUT_ACTIVE_SHIFT)
        touchesPressees[1] = true;
    else
        touchesPressees[1] = false;
}

//méthode qui permet d'utiliser les touches special pour deplacer la caméra
void clavierSpecialDown(int touche, int x, int y)
{
	switch (touche)
	{
        case GLUT_KEY_LEFT: //Deplacer la cam a droite
            touchesPressees[2] = true;
			break;
        case GLUT_KEY_RIGHT: //Deplacer la cam a gauche
            touchesPressees[3] = true;
			break;
        case GLUT_KEY_UP: //Deplacer la cam par le bas
            touchesPressees[4] = true;
			break;
        case GLUT_KEY_DOWN: //Deplacer la cam par le haut
            touchesPressees[5] = true;
			break;
	}
    if(glutGetModifiers() & GLUT_ACTIVE_SHIFT)
        touchesPressees[1] = true;
    else
        touchesPressees[1] = false;
}

void clavierSpecialUp(int touche, int x, int y)
{
	switch (touche)
	{
        case GLUT_KEY_LEFT: //Deplacer la cam a droite
            touchesPressees[2] = false;
			break;
        case GLUT_KEY_RIGHT: //Deplacer la cam a gauche
            touchesPressees[3] = false;
			break;
        case GLUT_KEY_UP: //Deplacer la cam par le bas
            touchesPressees[4] = false;
			break;
        case GLUT_KEY_DOWN: //Deplacer la cam par le haut
            touchesPressees[5] = false;
			break;
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
	camZoom += signe * camZoomSpeed;
}
