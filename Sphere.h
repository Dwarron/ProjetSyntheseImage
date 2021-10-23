/********************************************************/
/*                     Sphere.h                                                 */
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

/*class Sphere*/
class Sphere{
    public:
        Sphere(int NP, int NM, float r);

    private:
        float pi = 3.1415926;
};
