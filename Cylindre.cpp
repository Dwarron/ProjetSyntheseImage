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

Cylindre::Cylindre(float r, float h, int subd)
{
    float coords[subd * 2][3];

    for(int i = 0; i < subd * 2; i++)
    {
        float angle = 2*PI/float(subd)*i;
        coords[i][0] = r * cos(angle);
        coords[i][1] = r * sin(angle);

        if(i >= subd)
        {
            coords[i][2] = h / 2.0;
        }
        else
        {
            coords[i][2] = -h / 2.0;
        }

    }
    int faces[subd][4];

    for(int i = 0; i < subd; i++)
    {
        faces[i][0] = i;
        faces[i][1] = (i+1)%subd;
        faces[i][2] = (i + 1)%subd + subd;
        faces[i][3] = i + subd;
    }

    for(int i = 0; i < subd; i++)
    {
        glBegin(GL_POLYGON);
            glVertex3f(coords[faces[i][0]][0], coords[faces[i][0]][1], coords[faces[i][0]][2]);
            glVertex3f(coords[faces[i][1]][0], coords[faces[i][1]][1], coords[faces[i][1]][2]);
            glVertex3f(coords[faces[i][2]][0], coords[faces[i][2]][1], coords[faces[i][2]][2]);
            glVertex3f(coords[faces[i][3]][0], coords[faces[i][3]][1], coords[faces[i][3]][2]);
        glEnd();
    }

    //couvercles
    for(int i = 0; i < subd; i++)
    {
        glBegin(GL_POLYGON);
            glVertex3f(coords[i][0], coords[i][1], coords[i][2]);
            glVertex3f(0, 0, -h / 2.0);
            glVertex3f(coords[(i + 1) % subd][0], coords[(i + 1) % subd][1], coords[(i + 1) % subd][2]);
        glEnd();
    }

    for(int i = subd; i < subd * 2; i++)
    {
        glBegin(GL_POLYGON);
            glVertex3f(coords[i][0], coords[i][1], coords[i][2]);
            glVertex3f(0, 0, h / 2.0);
            glVertex3f(coords[(i + 1) % subd][0], coords[(i + 1) % subd][1], coords[(i + 1) % subd][2]);
        glEnd();
    }
}
