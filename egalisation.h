#ifndef EGALISATION
#define EGALISATION
#include "bmp8.h"
#include "bmp24.h"

unsigned int * bmp8_computeHistogram(t_bmp8 * img);
unsigned int * bmp8_computeCDF(unsigned int * hist, t_bmp8 * img);
void bmp8_equalize(t_bmp8 * img);


#endif