#include <stdio.h>
#include <stdlib.h>
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

void convert_RGB_to_YUV(int r, int g, int b, int *y, int *u, int *v) {
    *y = 0.299 * r + 0.587 * g + 0.114 * b;
    *u = -0.14713 * r - 0.28886 * g + 0.436 * b ;
    *v = 0.615 * r - 0.51499 * g - 0.10001 * b;
}

int convert_YUV_to_RGB(int y, int u, int v, int *r, int *g, int *b) {
    *r = y + 1.13983 * v;
    *g = y - 0.39465 * u - 0.58060 * v;
    *b = y + 2.03211 * u;
}

void bmp24_equalize(t_bmp24 * img) {
    int nb_pixels = img->width * img->height;
    t_yuv *yuv_pixels = malloc(nb_pixels * sizeof(t_yuv));

    // convertition RGB -> YUV
    for (int i = 0; i < nb_pixels; i++) {
        int r = img->data[3*i];
        int g = img->data[3*i + 1];
        int b = img->data[3*i + 2];
        convert_RGB_to_YUV(r, g, b, &yuv_pixels[i].y, &yuv_pixels[i].u, &yuv_pixels[i].v);
    }

    // hist_Y

    // hist_cum 

    // nomalise CDF

    // appliquer equalise à Y

    // reconvertir YUV -> RGB

}