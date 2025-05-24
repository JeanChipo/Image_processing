#include <stdio.h>
#include "bmp8.h"
#include "bmp24.h"
#include "egalisation.h"


int * bmp8_computeHistogram(t_bmp8 * img) {
    int histogram[256] = {0};
    for (unsigned int i = 0; i < img->dataSize; i++) {
        histogram[img->data[i]]++;
    }
    return histogram;
}

