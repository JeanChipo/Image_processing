#include <stdio.h>
#include "bmp8.h"
#include "bmp24.h"
#include "egalisation.h"


unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
    unsigned int histogram[256] = {0};
    for (unsigned int i = 0; i < img->dataSize; i++) {
        histogram[img->data[i]]++;
    }
    return histogram;
}


unsigned int * bmp8_computeCDF(unsigned int * hist, t_bmp8 * img) {
    unsigned int cdf[256] = {0};

    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    unsigned int cdf_min = 255;
    for (int i = 0; i < 256; i++) {
        if (cdf_min > cdf[i]) {
            cdf_min = cdf[i];
        }
    }


    unsigned int hist_eq[256] = {0};
    for (int i = 0; i < 256; i++) {
        hist_eq[i] = round(((float)(cdf[i] - cdf_min) / (img->dataSize - cdf_min)) * 255);
    }
    
    return hist_eq;
}
