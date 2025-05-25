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
    unsigned int *hist_eq = bmp8_computeCDF(hist, img);
    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = hist_eq[img->data[i]];
    }
    free(hist);
    free(hist_eq);
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



unsigned int * bmp24_computeHistogram(t_bmp24 * img) {
    unsigned int *histogram = malloc(256 * sizeof(unsigned int));
    for (int i = 0; i < 256; i++) histogram[i] = 0;
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int y;
            convert_RGB_to_YUV(img->data[y][x].red, img->data[y][x].green, img->data[y][x].blue, &y, NULL, NULL);
            histogram[y]++;
        }
    }
        
    return histogram;
}


unsigned int * bmp24_computeCDF(unsigned int * hist, t_bmp24 * img) {
    unsigned int *cdf = malloc(256 * sizeof(unsigned int));
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    unsigned int cdf_min = 256;
    for (int i = 0; i < 256; i++) {
        if (cdf_min > cdf[i]) {
            cdf_min = cdf[i];
        }
    }

    unsigned int *hist_eq = malloc(256 * sizeof(unsigned int));
    for (int i = 0; i < 256; i++) {
        hist_eq[i] = round(((float)(cdf[i] - cdf_min) / (img->height*img->width - cdf_min)) * 255);
    }
    free(cdf);
    return hist_eq;
}




void bmp24_equalize(t_bmp24 * img) {
    int nb_pixels = img->width * img->height;
    t_yuv *yuv_pixels = malloc(nb_pixels * sizeof(t_yuv));

    // convertition RGB -> YUV
    for (int i = 0; i < nb_pixels; i++) {
        int r = img->data[i]->red;
        int g = img->data[i]->green;
        int b = img->data[i]->blue;
        convert_RGB_to_YUV(r, g, b, &yuv_pixels[i].y, &yuv_pixels[i].u, &yuv_pixels[i].v);
    }

    // hist_Y

    // hist_cumulé

    // nomalise CDF

    // appliquer equalise à Y

    // reconvertir YUV -> RGB

}