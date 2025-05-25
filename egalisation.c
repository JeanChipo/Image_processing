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

void convert_YUV_to_RGB(int y, int u, int v, int *r, int *g, int *b) {
    *r = y + 1.13983 * v;
    *g = y - 0.39465 * u - 0.58060 * v;
    *b = y + 2.03211 * u;
}

unsigned int * bmp24_computeHistogram(t_bmp24 * img) {
    unsigned int *histogram = malloc(256 * sizeof(unsigned int));
    int trash;
    for (int i = 0; i < 256; i++) histogram[i] = 0;
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int y_yuv;
            convert_RGB_to_YUV(img->data[y][x].red, img->data[y][x].green, img->data[y][x].blue, &y_yuv, &trash, &trash);
            y_yuv = clamp_255(y_yuv);
            histogram[y_yuv]++;
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


int clamp_255(int value) {
    if (value < 0) {
        return 0;
    } else if (value > 255) {
        return 255;
    }
    return value;
}

void bmp24_equalize(t_bmp24 * img) {
    int width = img->width;
    int height = img->height;
    int nb_pixels = width * height;
    t_yuv *yuv_pixels = malloc(nb_pixels * sizeof(t_yuv));

    // convertition RGB -> YUV
    int i = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++, i++) {
            int r = img->data[y][x].red;
            int g = img->data[y][x].green;
            int b = img->data[y][x].blue;
            convert_RGB_to_YUV(r, g, b, &yuv_pixels[i].y, &yuv_pixels[i].u, &yuv_pixels[i].v);
            yuv_pixels[i].y = clamp_255(yuv_pixels[i].y);
        }
    }

    // hist_Y
    unsigned int *hist_Y = bmp24_computeHistogram(img);

    // hist_cumulé & normalisé
    unsigned int *cdf_Y = bmp24_computeCDF(hist_Y, img);

    // appliquer equalise à Y
    for (int i = 0; i < nb_pixels; i++) {
        yuv_pixels[i].y = cdf_Y[clamp_255(yuv_pixels[i].y)];
    }

    // reconvertir YUV -> RGB
    i = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++, i++) {
            int r, g, b;
            convert_YUV_to_RGB(yuv_pixels[i].y, yuv_pixels[i].u, yuv_pixels[i].v, &r, &g, &b);
            img->data[y][x].red = clamp_255(r);
            img->data[y][x].green = clamp_255(g);
            img->data[y][x].blue = clamp_255(b);
        }
    }

    free(yuv_pixels);
    free(hist_Y);
    free(cdf_Y);
}