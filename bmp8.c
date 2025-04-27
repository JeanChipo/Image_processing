#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

t_bmp8 * bmp8_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb"); // rb for read binary
    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8)); // allocate memory for the image structure

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file); // read the header into a local array

    for (int i = 0; i < 54; i++) {  // copy the header into img->header
        img->header[i] = header[i];
    }

    img->width = *(unsigned int *)&header[18];
    img->height = *(unsigned int *)&header[22];
    img->colorDepth = *(unsigned short *)&header[28]; // color depth is 2 bytes -> unsigned short
    img->dataSize = *(unsigned int *)&header[34];

    img->data = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char)); // allocate memory for the image data

    fread(img->data, sizeof(unsigned char), img->dataSize, file); // read the image data
    fclose(file);
    return img;
}

void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    FILE *file = fopen(filename, "wb"); // wb for write binary
    fwrite(img->header, sizeof(unsigned char), 54, file); // write the header
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file); // write the image data
    fclose(file);
}

void bmp8_free(t_bmp8 *img) {
    if (img) {
        if (img->data) {
            free(img->data); 
        }
        free(img);
    }
}

void bmp8_printInfo(t_bmp8 *img) {
    printf("Image Info:\n");
    printf("  | Width: %u \n", img->width);
    printf("  | Height: %u \n", img->height);
    printf("  | Color Depth: %u \n", img->colorDepth);
    printf("  | Data Size: %u \n", img->dataSize);
}

// ################### //

/*
void bmp8_negative(t_bmp8 * img) {
    for (int i = 0; i < img->height; i++) {
        char * line = img->colorTable[i];
        for (int j = 0; j < img->width; j++) {
            line[j] = 255 - line[j];
        }
    }
} 
*/