/********************************************************/
/*                     Cylindre.h                                                 */
/********************************************************/
/*Modélisation du cylindre a partir de sa représentation paramétrique               */
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

/*class Cylindre*/
class Cylindre{
    public:
        Cylindre(int n, float r, int subd); //subd == subdivision

    private:
        float pi = 3.1415926;
};
