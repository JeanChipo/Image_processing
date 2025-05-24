#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp8.h"
#include "bmp24.h"
#include "egalisation.h"


unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
    unsigned int *histogram = malloc(256 * sizeof(unsigned int));
    for (int i = 0; i < 256; i++) histogram[i] = 0;
    for (int i = 0; i < img->dataSize; i++) {
        histogram[img->data[i]]++;
    }
    return histogram;
}


unsigned int * bmp8_computeCDF(unsigned int * hist, t_bmp8 * img) {
    unsigned int *cdf = malloc(256 * sizeof(unsigned int));
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

    unsigned int *hist_eq = malloc(256 * sizeof(unsigned int));
    for (int i = 0; i < 256; i++) {
        hist_eq[i] = round(((float)(cdf[i] - cdf_min) / (img->dataSize - cdf_min)) * 255);
    }
    free(cdf);
    return hist_eq;
}


void bmp8_equalize(t_bmp8 * img) {
    unsigned int *hist = bmp8_computeHistogram(img);
    printf("aaaaa");
    unsigned int *hist_eq = bmp8_computeCDF(hist, img);
    for (int i = 0; i < 256; i++) {
        printf("Valeur %d : %d\n", hist[i], hist_eq[i]);
    }
    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = hist_eq[img->data[i]];
    }
    free(hist);
    free(hist_eq);
}
