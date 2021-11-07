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
#include "Cylindre.h"
#include "Sphere.h"

#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif

float camZoom = 20;
float camZoomSpeed = 0.1;
float camMoveSpeed = 0.3;
char presse;
float anglex,angley,x,y,xold,yold;
float mouthAngleFireBall;
float mouthMaxAngleFireBall = 30;
float ailesAngle = 0;
float ailesSpeed = 0.05;
float maxAilesAngle = 15;
float minAilesAngle = -15;

bool animFireBall;
int startTimeAnimFireBall;
float animFireBallDuration = 8;
float fireBallRadius = 1;
float avancementFireBall = 0;
bool spawnFireBall = false;
float speedFireBall = 0.05;

Texture customTextures[2]; //tableau de toutes les textures utilisees

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
void anim();

float largeurCorp = 4;
float longueurCorp = 10;
float longueurHauteurCorp = 5;

float decalageHauteurAiles = 1;
float largeurAiles = 30;
float longueurAiles = 10;

void ailes()
{
    glPushMatrix();
        glTranslatef(0, longueurHauteurCorp / 2.0 - decalageHauteurAiles, 0);

        //aile droite
        glPushMatrix();
            glRotatef(ailesAngle, 0, 0, 1);
            glColor3f(0, 0, 0);
            glTranslatef(largeurCorp*0.5, 0, 0);

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
                glScalef(largeurAiles * 0.5, 1, 1);
                glPushMatrix();
                    glutSolidCube(1);

                    glTranslatef(1, 0, 0);
                    glPushMatrix();
                        glScalef(1, 0.5, 0.5);
                        glutSolidCube(1);
                    glPopMatrix();
                glPopMatrix();

            glPopMatrix();

        glPopMatrix();

        //aile gauche
        glPushMatrix();
            glTranslatef(-largeurCorp*0.5, 0, 0);
            glRotatef(-ailesAngle, 0, 0, 1);

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
                glScalef(largeurAiles * 0.5, 1, 1);
                glPushMatrix();
                    glutSolidCube(1);

                    glTranslatef(-1, 0, 0);
                    glPushMatrix();
                        glScalef(1, 0.5, 0.5);
                        glutSolidCube(1);
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();

        glPopMatrix();

    glPopMatrix();
}

float dimTete = 2;

void tete()
{
    customTextures[0].activer();    //texture tete

	glPushMatrix();
        glTranslated(0, 0, dimTete / 2.0);
        glColor3f(0, 0, 0);

        float longueurCubeTete = dimTete / 2.0;
        //texture qui va etre appliquer sur la tete du dragon pour avoir la tete noir et les yeux violet
        glBegin(GL_POLYGON);
            glTexCoord2f(0.0,0.0);   glVertex3f(longueurCubeTete, longueurCubeTete, longueurCubeTete);
            glTexCoord2f(0.0,1.0);   glVertex3f(longueurCubeTete,-longueurCubeTete, longueurCubeTete);
            glTexCoord2f(1.0,1.0);   glVertex3f(-longueurCubeTete,-longueurCubeTete, longueurCubeTete);
            glTexCoord2f(1.0,0.0);   glVertex3f(-longueurCubeTete, longueurCubeTete, longueurCubeTete);
        glEnd();

        //tete du dragon
        glPushMatrix();
            glColor3f(0,0,0);
            glScalef(dimTete, dimTete, dimTete);
            glutSolidCube(1);
        glPopMatrix();

        //machoire superieur
        glPushMatrix();
            glColor3f(0,0,0);
            glTranslated(0, -dimTete/12.0, dimTete);
            glScalef(dimTete/1.2, dimTete/4.0, dimTete);
            glutSolidCube(1);
        glPopMatrix();

        //machoire inferieur
        glPushMatrix();
            glTranslated(0, -dimTete/3.0, dimTete/2.0);     //pivot relatif au bout de la tete
            glRotatef(mouthAngleFireBall, 1, 0, 0);
            glTranslated(0, dimTete/3.0, -dimTete/2.0);

            glPushMatrix();
                glColor3f(0,0,0);
                glTranslated(0, -dimTete/3.0, dimTete - dimTete/16.0);
                glScalef(dimTete/1.2, dimTete/4.0, dimTete + dimTete/8.0); // plus grand que la machoire superieur afin d'avoir une animation plus propre
                glutSolidCube(1);
            glPopMatrix();

        glPopMatrix();

        if(spawnFireBall)
        {
            glPushMatrix();
                glTranslated(0, -dimTete/3.0, dimTete/2.0);     //pivot relatif au bout de la tete
                glRotatef(mouthAngleFireBall / 2.0, 1, 0, 0);
                glTranslated(0, dimTete/3.0, -dimTete/2.0);

                glTranslatef(0, 0, dimTete / 2.0 + fireBallRadius);
                glTranslatef(0, 0, avancementFireBall);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                customTextures[1].activer();
                Sphere s(10, 10, fireBallRadius);
                customTextures[1].desactiver();

            glPopMatrix();
        }

        glPushMatrix();
            glTranslated(0, dimTete - dimTete/3.0, 0);
            glScalef(dimTete/8.0, dimTete/3.0, dimTete/2.0);
            //Crete crane gauche
            glPushMatrix();
                glTranslated(-dimTete, 0, 0);
                glutSolidCube(1);
            glPopMatrix();
        //Crete crane droite
            glPushMatrix();
                glTranslated(dimTete, 0, 0);
                glutSolidCube(1);
            glPopMatrix();

        glPopMatrix();

        glPushMatrix();
            glTranslated(0, dimTete/12.0 , dimTete);
            glScalef(dimTete/8.0, dimTete/8.0, dimTete/2.0);
            glColor3f(1,0,0);

            //Crete museau gauche
            glPushMatrix();
                glTranslated(-dimTete, 0, 0);
                glutSolidCube(1);
            glPopMatrix();

            //Crete museau droite
            glPushMatrix();
                glTranslated(dimTete, 0, 0);
                glutSolidCube(1);
            glPopMatrix();

        glPopMatrix();

    glPopMatrix();
    customTextures[0].desactiver();
}

float avancementJambesArrieres = 2;
float hauteurJambesArrieres = longueurHauteurCorp / 2.0 - 1;
float largeurJambesArrieres = 1.5;
float longueurCuissesArrieres = 5;
float angleCuissesArrieres = 55;
float angleMolletsArrieres = -100;
float longueurMolletsArrieres = 3;
float largeurPiedsArrieres = 3;
float longueurPiedsArrieres = 4;
float hauteurPiedsArrieres = 1;

float avancementJambesAvants = 2;
float hauteurJambesAvants = longueurHauteurCorp / 2.0 - 1.5;
float largeurJambesAvants = 1;
float longueurCuissesAvants = 3;
float longueurMolletsAvants = 5;
float angleCuissesAvants = 135;
float angleMolletsAvants = 90;
float largeurPiedsAvants = largeurJambesAvants;
float longueurPiedsAvants = 2;
float hauteurPiedsAvants = 0.75;

void jambes()
{
    //arriere
    glPushMatrix();
        glTranslatef(0, hauteurJambesArrieres, -longueurCorp / 2.0 + avancementJambesArrieres);

        //droite
        glPushMatrix();
            //cuisse
            glTranslatef(largeurCorp/2.0 + largeurJambesArrieres / 2.0, -longueurCuissesArrieres/2.0, 0);
            glRotatef(angleCuissesArrieres, 1, 0, 0);
            glPushMatrix();
                glScalef(largeurJambesArrieres, largeurJambesArrieres, longueurCuissesArrieres);
                glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();
                //mollet
                glTranslatef(0, 0, longueurCuissesArrieres / 2.0);
                glRotatef(angleMolletsArrieres, 1, 0, 0);

                glPushMatrix();
                    glScalef(largeurJambesArrieres, largeurJambesArrieres, longueurMolletsArrieres);
                    glTranslatef(0, 0, -0.5);
                    glutSolidCube(1);
                glPopMatrix();

                glTranslatef(0, 0, -longueurMolletsArrieres);
                glRotatef(-angleMolletsArrieres - angleCuissesArrieres, 1, 0, 0);
                glTranslatef(0, -hauteurPiedsArrieres / 2.0, longueurPiedsArrieres / 2.0);
                glScalef(largeurPiedsArrieres, hauteurPiedsArrieres, longueurPiedsArrieres);
                glutSolidCube(1);
            glPopMatrix();

        glPopMatrix();

        //gauche
        glPushMatrix();
            //cuisse
            glTranslatef(-largeurCorp/2.0 - largeurJambesArrieres / 2.0, -longueurCuissesArrieres/2.0, 0);
            glRotatef(angleCuissesArrieres, 1, 0, 0);
            glPushMatrix();
                glScalef(largeurJambesArrieres, largeurJambesArrieres, longueurCuissesArrieres);
                glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();
                //mollet
                glTranslatef(0, 0, longueurCuissesArrieres / 2.0);
                glRotatef(angleMolletsArrieres, 1, 0, 0);

                glPushMatrix();
                    glScalef(largeurJambesArrieres, largeurJambesArrieres, longueurMolletsArrieres);
                    glTranslatef(0, 0, -0.5);
                    glutSolidCube(1);
                glPopMatrix();

                glTranslatef(0, 0, -longueurMolletsArrieres);
                glRotatef(-angleMolletsArrieres - angleCuissesArrieres, 1, 0, 0);
                glTranslatef(0, -hauteurPiedsArrieres / 2.0, longueurPiedsArrieres / 2.0);
                glScalef(largeurPiedsArrieres, hauteurPiedsArrieres, longueurPiedsArrieres);
                glutSolidCube(1);
            glPopMatrix();

        glPopMatrix();

    glPopMatrix();

    //avant
    glPushMatrix();
        glTranslatef(0, hauteurJambesAvants, longueurCorp / 2.0 - avancementJambesAvants);

        //droite
        glPushMatrix();
            //cuisse
            glTranslatef(largeurCorp/2.0 + largeurJambesAvants / 2.0, -longueurCuissesAvants/2.0, 0);
            glRotatef(angleCuissesAvants, 1, 0, 0);
            glPushMatrix();
                glScalef(largeurJambesAvants, largeurJambesAvants, longueurCuissesAvants);
                glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();
                //mollet
                glTranslatef(0, 0, longueurCuissesAvants / 2.0);
                glRotatef(angleMolletsAvants, 1, 0, 0);

                glPushMatrix();
                    glScalef(largeurJambesAvants, largeurJambesAvants, longueurMolletsAvants);
                    glTranslatef(0, 0, -0.5);
                    glutSolidCube(1);
                glPopMatrix();

                glTranslatef(0, 0, -longueurMolletsAvants);
                glRotatef(-angleMolletsAvants - angleCuissesAvants, 1, 0, 0);
                glTranslatef(0, -hauteurPiedsAvants / 2.0, longueurPiedsAvants / 2.0);
                glScalef(largeurPiedsAvants, hauteurPiedsAvants, longueurPiedsAvants);
                glutSolidCube(1);
            glPopMatrix();

        glPopMatrix();

        //gauche
        glPushMatrix();
            //cuisse
            glTranslatef(-largeurCorp/2.0 - largeurJambesAvants / 2.0, -longueurCuissesAvants/2.0, 0);
            glRotatef(angleCuissesAvants, 1, 0, 0);
            glPushMatrix();
                glScalef(largeurJambesAvants, largeurJambesAvants, longueurCuissesAvants);
                glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();
                //mollet
                glTranslatef(0, 0, longueurCuissesAvants / 2.0);
                glRotatef(angleMolletsAvants, 1, 0, 0);

                glPushMatrix();
                    glScalef(largeurJambesAvants, largeurJambesAvants, longueurMolletsAvants);
                    glTranslatef(0, 0, -0.5);
                    glutSolidCube(1);
                glPopMatrix();

                glTranslatef(0, 0, -longueurMolletsAvants);
                glRotatef(-angleMolletsAvants - angleCuissesAvants, 1, 0, 0);
                glTranslatef(0, -hauteurPiedsAvants / 2.0, longueurPiedsAvants / 2.0);
                glScalef(largeurPiedsAvants, hauteurPiedsAvants, longueurPiedsAvants);
                glutSolidCube(1);
            glPopMatrix();

        glPopMatrix();

    glPopMatrix();
}

int divisionsQueue = 5;
float hauteurQueue = 0;
float longueurQueue = 10;
float rayonQueue = 0.75;
int divisionsCou = 5;
float hauteurCou = 2;
float longueurCou = 2.5;
float rayonCou = 0.5;

void queueEtCou()
{
    //queue
    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        glTranslatef(0, hauteurQueue, longueurCorp/2.0 + longueurQueue / 2.0);

        Cylindre queue(rayonQueue, longueurQueue, 10);

    glPopMatrix();

    //cou
    glPushMatrix();
        glTranslatef(0, hauteurCou, longueurCorp/2.0 + longueurCou / 2.0);

        Cylindre cou(rayonCou, longueurCou, 10);

        glTranslatef(0, 0, longueurCou / 2.0);

        tete();
    glPopMatrix();
}

void dragon()
{
    glPushMatrix();

    //buste du dragon
    glPushMatrix();
        glColor3f(1,0.8,0);
        glScalef(largeurCorp, longueurHauteurCorp, longueurCorp);
        glutSolidCube(1);
    glPopMatrix();

    ailes();
    jambes();
    queueEtCou();

    glPopMatrix();
}

GLfloat sunPos[] = {-5.0, 10.0, 5.0, 0.0};
GLfloat sunDiffuse[] = {0.1, 0.1, 0.1, 1};
GLfloat sunSpecular[] = {0.1, 0.1, 0.1, 1};
GLfloat sunAmbient[] = {0.0, 0.0, 0.0, 1};

int main(int argc,char **argv)
{
    /* initialisation de glut et creation de la fenetre */
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Dragon");

	/* Initialisation d'OpenGL */
	glClearColor(0.5,0.5,0.5,0.0);
	glColor3f(1.0,1.0,1.0);
	glPointSize(2.0);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glShadeModel(GL_FLAT);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);

    // chargement des textures
    customTextures[0] = Texture("./Ender_dragon_skinHead.jpg");
    customTextures[1] = Texture("./feu.jpg");

	/* enregistrement des fonctions de rappel */
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavierDown);
	glutKeyboardUpFunc(clavierUp);
	glutSpecialFunc(clavierSpecialDown);
	glutSpecialUpFunc(clavierSpecialUp);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);
	glutIdleFunc(anim);

	/* Entree dans la boucle principale glut */
	glutMainLoop();
	return 0;
}

GLfloat diffuseMat[] = {0.5, 0.5, 0.5, 1};

void affichage()
{
    glMatrixMode(GL_MODELVIEW);

    glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

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

    /* Parametrage du placage de textures */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);// Comment la texture interagit avec la couleur du pixel

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMat);
    dragon();

    glLoadIdentity();
    glOrtho(camZoom,-camZoom,-0.4 * camZoom, 1.6 * camZoom ,-camZoom,camZoom);
    glRotatef(angley,1.0,0.0,0.0);
    glRotatef(anglex,0.0,1.0,0.0);

	glFlush();

	//On echange les buffers
	glutSwapBuffers();
}

void startAnimFireBall()
{
    animFireBall = true;
    spawnFireBall = false;
    avancementFireBall = 0;
    startTimeAnimFireBall = glutGet(GLUT_ELAPSED_TIME);
}

GLfloat lightFireBallDiffuse[] = {1, 1, 1, 1};

void anim()
{
    if(animFireBall)
    {
        float animTime = (glutGet(GLUT_ELAPSED_TIME) - startTimeAnimFireBall) / animFireBallDuration / 1000.0;

        if(animTime <= 0.33)
        {
            mouthAngleFireBall = mouthMaxAngleFireBall * (animTime * 3);
        }
        else
        {
            spawnFireBall = true;
            avancementFireBall += speedFireBall;

            GLfloat posLightFireBall[] = {0, hauteurCou, longueurCorp / 2.0 + longueurCou + dimTete + fireBallRadius + avancementFireBall, 1};

            glLightfv(GL_LIGHT1, GL_DIFFUSE, lightFireBallDiffuse);
            glLightfv(GL_LIGHT1, GL_POSITION, posLightFireBall);

            glEnable(GL_LIGHT1);

            if(animTime <= 0.67)
            {
                mouthAngleFireBall = (mouthMaxAngleFireBall * 2 / 3.0 - mouthMaxAngleFireBall * animTime) * 3;
            }
        }

        if(animTime >= 1)
        {
            animFireBall = false;
            glDisable(GL_LIGHT1);
        }
    }

    ailesAngle += ailesSpeed;
    if(ailesAngle >= maxAilesAngle || ailesAngle <= minAilesAngle)
        ailesSpeed *= -1;

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
        case 'b':
			startAnimFireBall();
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
