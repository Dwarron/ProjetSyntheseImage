/********************************************************/
/*                     Cylindre.cpp                                                 */
/********************************************************/
/*Modélisation cylindre a partir de sa représentation paramétrique               */
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
#include "Cylindre.h"

Cylindre::Cylindre(int n, float r, int subd)
{
    float x[n];
    float y[n];

    for(int i = 0; i < n-1; i++)
    {
        x[i] = r*cos( (2*i*pi)/6 );
        y[i] = r*cos( (2*i*pi)/6 );
    }
    x[n] = 0;
    y[n] = 0;

    int faces[n][3];

    for(int i = 0; i < n-1; i++)
    {
        faces[i][0] = i;
        faces[i][1] = (i+1)%n;
        faces[i][2] = n;
    }

    for(int i = 0; i < n-1; i++)
    {
        glBegin(GL_POLYGON);
            glVertex2f(x[faces[i][0]], y[faces[i][0]]);
            glVertex2f(x[faces[i][1]], y[faces[i][1]]);
            glVertex2f(x[faces[i][2]], y[faces[i][2]]);
        glEnd();
    }
}
