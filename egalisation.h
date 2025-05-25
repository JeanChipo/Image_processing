/* 
Fichier d'en-tête de traitement d'égalisation d'histogramme pour les images BMP 8 et 24 bits
Il contient les prototypes des fonctions de création d'histogramme, de calcul de CDF, et d'égalisation 
pour les images en niveaux de gris (BMP 8) et en couleur (BMP 24).
auteurs : @JeanChipo, @WarennOne
*/

#ifndef EGALISATION
#define EGALISATION
#include "bmp8.h"
#include "bmp24.h"

typedef struct {
    int y;
    int u;
    int v;
} t_yuv;


unsigned int * bmp8_computeHistogram(t_bmp8 * img);
unsigned int * bmp8_computeCDF(unsigned int * hist, t_bmp8 * img);
void bmp8_equalize(t_bmp8 * img);


void convert_RGB_to_YUV(int r, int g, int b, int *y, int *u, int *v);
void convert_YUV_to_RGB(int y, int u, int v, int *r, int *g, int *b);
int clamp_255(int value);

unsigned int * bmp24_computeHistogram(t_bmp24 * img);
unsigned int * bmp24_computeCDF(unsigned int * hist, t_bmp24 * img);
void bmp24_equalize(t_bmp24 * img);

#endif