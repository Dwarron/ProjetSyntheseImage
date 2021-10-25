/********************************************************/
/*                     Texture.h                                                 */
/********************************************************/
/*        Class qui s'occupe des textures du projet                    */
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

/*class Texture*/
class Texture{
    public:
        Texture();
        Texture(char *fichier);
        unsigned char texture[256][256][3];

        void activer();
        void desactiver();

    private:
        void loadJpegImage(char *fichier);
};
