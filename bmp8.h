/* 
Fichier d'en-tête de traitement d'images BMP 8 en niveaux de gris
Il contient les prototypes des fonctions de chargement, sauvegarde, et traitement d'images en niveaux de gris.
auteurs : @JeanChipo, @WarennOne
*/

#ifndef BMP8
#define BMP8

typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;

    unsigned int width; // offset à 18 de l'en-tête  
    unsigned int height; // offset à 22 de l'en-tête
    unsigned short colorDepth; // offset à 28 de l'en-tête
    unsigned int dataSize; // offset à 34 de l'en-tête
} t_bmp8;

t_bmp8 * bmp8_loadImage(const char * filename);
void bmp8_saveImage(const char * filename, t_bmp8 * img);
void bmp8_free(t_bmp8 * img);
void bmp8_printInfo(t_bmp8 * img);
void bmp8_negative(t_bmp8 * img);
void bmp8_brightness(t_bmp8 * img, int brightness);
void bmp8_threshold(t_bmp8 * img);
void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize);

#endif // BMP8