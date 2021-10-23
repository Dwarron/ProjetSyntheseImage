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
//#include "Texture.cpp"
//#include "Cylindre.cpp"
#include "Sphere.h"

#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif

#include <cmath>

float camZoom = 20;
float camSpeed = 1;
char presse;
int anglex,angley,x,y,xold,yold, mouveFireBall = 0;
float mouthAngle = 0.0, mouthSpeed = 0.02;
//Texture *customTextures; //tableau de toutes les textures

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche, int x, int y);
void clavierSpecial(int touche, int x, int y);
void reshape(int x, int y);
void idle();
void mouse(int bouton, int etat, int x, int y);
void mousemotion(int x, int y);
void zoom(int signe);
void mouthAnim();

float dimTete = 2;

void teteDragon()
{
	glPushMatrix();
        glTranslated(4, 8, 0); //a determiné quand on aura le cout du dragon

        //texture qui va etre appliquer sur la tete du dragon pour avoir la tete noir et les yeux violet
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);   glVertex3f(1, 1, 1);
        glTexCoord2f(0.0,1.0);   glVertex3f(1,-1, 1);
        glTexCoord2f(1.0,1.0);   glVertex3f(1,-1,-1);
        glTexCoord2f(1.0,0.0);   glVertex3f(1, 1,-1);
        glEnd();

        //tete du dragon
        glPushMatrix();
        glColor3f(150,150,0);
        glScalef(dimTete, dimTete, dimTete);
        glutSolidCube(1);
        glPopMatrix();

        //machoire superieur
        glPushMatrix();
        glColor3f(0,0,250);
        glTranslated(dimTete, -dimTete/12.0, 0);
        glScalef(dimTete, dimTete/4.0, dimTete/1.20);
        glutSolidCube(1);
        glPopMatrix();

        //machoire inferieur
        glPushMatrix();
            glTranslated(dimTete/2.0, -dimTete/3.0, 0);
            glRotatef(mouthAngle, 0, 0, 1);
            glTranslated(-dimTete/2.0, dimTete/3.0, 0);

            glPushMatrix();
            glColor3f(0,250,0);
            glTranslated(dimTete - dimTete/16.0, -dimTete/3.0, 0);
            glScalef(dimTete + dimTete/8.0, dimTete/4.0, dimTete/1.20); // plus grand que la machoire superieur afin d'avoir une animation plus propre
            glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();

        //Crete crane gauche
        glPushMatrix();
        glColor3f(255,0,255);
        glTranslated(0, dimTete - dimTete/3.0, dimTete/4.0);
        glScalef(dimTete/2.0, dimTete/3.0, dimTete/8.0);
        glutSolidCube(1);
        glPopMatrix();

        //Crete crane droite
        glPushMatrix();
        glColor3f(55,0,55);
        glTranslated(0, dimTete - dimTete/3.0, -dimTete/4.0);
        glScalef(dimTete/2.0, dimTete/3.0, dimTete/8.0);
        glutSolidCube(1);
        glPopMatrix();

        //Crete museau gauche
        glPushMatrix();
        glColor3f(255,0,0);
        glTranslated(dimTete, dimTete/12.0 , dimTete/6.0);
        glScalef(dimTete/2.0, dimTete/8.0, dimTete/6.0);
        glutSolidCube(1);
        glPopMatrix();

        //Crete museau droite
        glPushMatrix();
        glColor3f(255,0,0);
        glTranslated(dimTete, dimTete/12.0 , -dimTete/6.0);
        glScalef(dimTete/2.0, dimTete/8.0, dimTete/6.0);
        glutSolidCube(1);
        glPopMatrix();

    glPopMatrix();
}

float LPiedAvant = 3;
float lPiedAvant = 1;
float HPiedAvant = 2;
float LCuisseAvant = 1;
float lCuisseAvant = 4;
float HCuisseAvant = 1;
float LCuisseArriere = 2;
float lCuisseArriere = 4;
float HCuisseArriere = 2;
float CorpDragon = 8;


void JambeEtCorpDragon()
{
    glPushMatrix();

    //pied de la jambe avant droite
    glPushMatrix();
    glColor3f(255,0,0);
    glTranslated(0, 0, -CorpDragon/2.0 - lPiedAvant/2.0);
    glScalef(LPiedAvant, lPiedAvant, HPiedAvant);
    glutSolidCube(1);
    glPopMatrix();

    //pied de la jambe avant gauche
    glPushMatrix();
    glColor3f(255,0,0);
    glTranslated(0, 0, +CorpDragon/2.0 + lPiedAvant/2.0);
    glScalef(LPiedAvant, lPiedAvant, HPiedAvant);
    glutSolidCube(1);
    glPopMatrix();

    glRotatef(40, 0, 0, 1);

    //molet jambe avant droite
    glPushMatrix();
    glColor3f(0,255,0);
    glTranslated(-LCuisseAvant/2.0, lCuisseAvant/2.0 + HCuisseAvant/2.0, -CorpDragon/2.0 - lPiedAvant/2.0);
    glScalef(LCuisseAvant, lCuisseAvant, HCuisseAvant);
    glutSolidCube(1);
    glPopMatrix();

    //molet jambe avant gauche
    glPushMatrix();
    glColor3f(0,255,0);
    glTranslated(-LCuisseAvant/2.0, lCuisseAvant/2.0 + HCuisseAvant/2.0, CorpDragon/2.0 + lPiedAvant/2.0);
    glScalef(LCuisseAvant, lCuisseAvant, HCuisseAvant);
    glutSolidCube(1);
    glPopMatrix();

    glTranslated(-LCuisseAvant/2.0, lCuisseAvant, 0);
    glRotatef(-90, 0, 0, 1);

    //lCuisseAvant*2.0 - HCuisseAvant - lPiedJambeAvant/2.0

    //cuisse jambe avant droite
    glPushMatrix();
    glColor3f(0,0,255);
    glTranslated(-LCuisseAvant, lCuisseAvant - HCuisseAvant - lPiedAvant/2.0, -CorpDragon/2.0 - lPiedAvant/2.0);
    glScalef(LCuisseAvant, lCuisseAvant*1.5, HCuisseAvant);
    glutSolidCube(1);
    glPopMatrix();

    //cuisse jambe avant gauche
    glPushMatrix();
    glColor3f(0,0,255);
    glTranslated(-LCuisseAvant, lCuisseAvant - HCuisseAvant - lPiedAvant/2.0, CorpDragon/2.0 + lPiedAvant/2.0);
    glScalef(LCuisseAvant, lCuisseAvant*1.5, HCuisseAvant);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();

    //buste du dragon
    glPushMatrix();
    glColor3f(1,0.8,0);
    glTranslated(-LCuisseAvant*8.0, lCuisseAvant*1.5, 0);
    glScalef(CorpDragon*3.0, CorpDragon/2.0, CorpDragon);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();

    //pied de la jambe arriere droite
    glPushMatrix();
    glColor3f(1,0,0);
    glTranslated(-CorpDragon*2.0, 0, -CorpDragon/2.0 - HPiedAvant/2.0);
    glScalef(LPiedAvant*2.0, lPiedAvant, HPiedAvant*2.0);
    glutSolidCube(1);
    glPopMatrix();

    //pied de la jambe arriere gauche
    glPushMatrix();
    glColor3f(1,0,0);
    glTranslated(-CorpDragon*2.0, 0, CorpDragon/2.0 + HPiedAvant/2.0);
    glScalef(LPiedAvant*2.0, lPiedAvant, HPiedAvant*2.0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-CorpDragon*2.0 , lCuisseAvant/2.0 + HCuisseAvant/2.0, 0);
        glRotatef(-60, 0, 0, 1);

        //molet jambe arriere droite
        glPushMatrix();
        glColor3f(0,1,0);
        glTranslated(0, -lCuisseArriere/2.0, -CorpDragon/2.0 - HPiedAvant/2.0);
        glScalef(LCuisseArriere, lCuisseArriere, HCuisseArriere);
        glutSolidCube(1);
        glPopMatrix();

        //molet jambe arriere gauche
        glPushMatrix();
        glColor3f(0,1,0);
        glTranslated(0, -lCuisseArriere/2.0, CorpDragon/2.0 + HPiedAvant/2.0);
        glScalef(LCuisseArriere, lCuisseArriere, HCuisseArriere);
        glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-CorpDragon*2.0 , lCuisseAvant, 0);
        glRotatef(30, 0, 0, 1); //30 car 90 + (-60)

        //cuisse jambe arriere droite
        glPushMatrix();
        glColor3f(0,0,1);
        glTranslated(0, 0, -CorpDragon/2.0 - HPiedAvant/2.0);
        glScalef(LCuisseArriere, lCuisseArriere, HCuisseArriere);
        glutSolidCube(1);
        glPopMatrix();

        //cuisse jambe arriere gauche
        glPushMatrix();
        glColor3f(0,0,1);
        glTranslated(0, 0, CorpDragon/2.0 + HPiedAvant/2.0);
        glScalef(LCuisseArriere, lCuisseArriere, HCuisseArriere);
        glutSolidCube(1);
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
    //Texture teteDrago("./Ender_dragon_skinHead.jpg");
    //customTextures[0] = teteDragon;

	/* enregistrement des fonctions de rappel */
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutSpecialFunc(clavierSpecial);
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
	/* effacement de l'image avec la couleur de fond */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glShadeModel(GL_SMOOTH);

	/*glLoadIdentity();
	gluLookAt(0.0, 0.0, 20.5, 0.0, 0.0, 0.0, 0.0, 20.0, camZoom);	// TODO : check si ça marche
	glRotatef(angley,1.0,0.0,0.0);
    glRotatef(anglex,0.0,1.0,0.0);*/

    /* Parametrage du placage de textures */
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, customTextures[0].texture);// spécifier la texture avec l’image
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);// Comment la texture interagit avec la couleur du pixel
    glEnable(GL_TEXTURE_2D);*/

    /* Chargement de la texture qui represente la crete du Dragon*/
    //loadJpegImage("./Ender_dragon_skinCrete.jpg");

    /* Parametrage du placage de textures */
    /*glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);// Comment la texture interagit avec la couleur du pixel
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, image);// spécifier la texture avec l’image
    glEnable(GL_TEXTURE_2D);*/

    teteDragon();
    JambeEtCorpDragon();
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

    if(mouthAngle <= -30 || mouthAngle >= 0)
        mouthSpeed = mouthSpeed * -1;

    glutPostRedisplay();
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
		case 'z': //zoomer sur le dragon
			zoom(1);
			break;
		case 'Z': //dezoomer
			zoom(-1);
			break;
		case 'q' : /*la touche 'q' permet de quitter le programme */
			exit(0);
	}
}

//méthode qui permet d'utiliser les touches special pour déplacer la caméra
void clavierSpecial(int touche, int x, int y)
{
	switch (touche)
	{
        case GLUT_KEY_LEFT: //Deplacer la cam a droite
            anglex=anglex+1;
            glutPostRedisplay();;
			break;
        case GLUT_KEY_RIGHT: //Deplacer la cam a gauche
            anglex=anglex-1;
            glutPostRedisplay();;
			break;
        case GLUT_KEY_UP: //Deplacer la cam par le bas
            angley=angley-1;
            glutPostRedisplay();;
			break;
        case GLUT_KEY_DOWN: //Deplacer la cam par le haut
            angley=angley+1;
            glutPostRedisplay();;
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
