/********************************************************/
/*                     Sphere.cpp                                                 */
/********************************************************/
/*Modélisation de la sphere a partir de sa représentation paramétrique               */
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

#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif

#include <cmath>
#include "Sphere.h"


Sphere::Sphere(int NP, int NM, float r)
{
    float coords[NM*NP][3];

    for(int i = 0; i < NP; i++)
        for(int j = 0; j < NM; j++)
        {
            float o = 2*pi*j/float(NM);
            float p = -pi/2.0+pi*i/float(NP);

            coords[j+i*NM][0] = cos(o) * cos(p) * r;
            coords[j+i*NM][1] = sin(o) * cos(p) * r;
            coords[j+i*NM][2] = sin(p) * r;
        }


    int faces[NP*NM][4];

    for(int j = 0; j < NP-1; j++)
       for(int i = 0; i < NM; i++)
        {
            faces[j*NM+i][0] = (i+1)%NM+NM*j;
            faces[j*NM+i][1] = (j+1)*NM+(i+1)%NM;
            faces[j*NM+i][2] = (j+1)*NM+i;
            faces[j*NM+i][3] = j*NM+i;
        }

    for(int i = 0; i < (NP-1) * NM; i++)
    {
        glBegin(GL_POLYGON);
            glVertex3f(coords[faces[i][0]][0], coords[faces[i][0]][1], coords[faces[i][0]][2]);
            glVertex3f(coords[faces[i][1]][0], coords[faces[i][1]][1], coords[faces[i][1]][2]);
            glVertex3f(coords[faces[i][2]][0], coords[faces[i][2]][1], coords[faces[i][2]][2]);
            glVertex3f(coords[faces[i][3]][0], coords[faces[i][3]][1], coords[faces[i][3]][2]);
        glEnd();
    }
}
