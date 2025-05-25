#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

// Lecture et écriture d’une image en niveaux de gris //

t_bmp8 * bmp8_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb"); // rb for read binary
    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8)); // allocate memory for the image structure

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file); // read the header into a local array

    for (int i = 0; i < 54; i++) {  // copy the header into img->header
        img->header[i] = header[i];
    }

    // Read color table (palette) for 8-bit BMP
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    img->width = *(unsigned int *)&header[18];
    img->height = *(unsigned int *)&header[22];
    img->colorDepth = *(unsigned short *)&header[28]; // color depth is 2 bytes -> unsigned short
    img->dataSize = *(unsigned int *)&header[34];

    img->data = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char)); // allocate memory for the image data

    if (img->dataSize == 0)
        img->dataSize = ((img->width * img->colorDepth + 31)/32) * 4 * img->height;

    fread(img->data, sizeof(unsigned char), img->dataSize, file); // read the image data
    fclose(file);
    return img;
}

void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    FILE *file = fopen(filename, "wb"); // wb for write binary
    fwrite(img->header, sizeof(unsigned char), 54, file); // write the header
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file); // write the color table
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

// Traitements d’une image en niveaux de gris //

void bmp8_printInfo(t_bmp8 *img) {
    printf("Image Info:\n");
    printf("  | Width: %u \n", img->width);
    printf("  | Height: %u \n", img->height);
    printf("  | Color Depth: %u \n", img->colorDepth);
    printf("  | Data Size: %u \n", img->dataSize);
}


// inversion des couleurs d’une image en noir et blanc //

void bmp8_negative(t_bmp8 * img) {
    // parcourt chaque pixel de l'image et inverse sa valeur
    for (unsigned int i = 0; i < img->dataSize; i++) {
        unsigned int NewValue = 255 - img->data[i] ;
        img->data[i] = NewValue ;
    }
}



// augmentation de la luminosité d’une image en niveaux de gris //

void bmp8_brightness(t_bmp8 * img, int brightness) {
    // parcourt chaque pixel et ajoute la valeur de luminosité
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int newValue = img->data[i] + brightness;
        // vérification des limites de la valeur
        if (newValue < 0) {
            img->data[i] = 0;
        } else if (newValue > 255) {
            img->data[i] = 255;
        } else {
            img->data[i] = (unsigned char)newValue;
        }
    }
}

// cahngement des couleurs d’une image a 255 ou 0 //

void bmp8_threshold(t_bmp8 * img) {
    // parcourt chaque pixel et change la valeur a 255 ou 0 si elle est au dessus ou en dessous de 128
    for (unsigned int i = 0; i < img->dataSize; i++) {
        if (img->data[i] < 128) {
            img->data[i] = 0;
        } else {
            img->data[i] = 255;
        }
    }
}
// Filtres d’images //

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    int n = kernelSize / 2;
    unsigned char *newData = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char));

    for (unsigned int i = 0; i < img->dataSize; i++) {
        newData[i] = img->data[i];
    }

    for (unsigned int y = n; y < img->height - n; y++) {    // go trough each pixel of the image
        for (unsigned int x = n; x < img->width - n; x++) {
            float newValue = 0.0;

            for (int ky = -n; ky <= n; ky++) {  // apply the kernel for each pixel in the kernel
                for (int kx = -n; kx <= n; kx++) {
                    int pixelX = x + kx;
                    int pixelY = y + ky;
                    unsigned char pixelValue = img->data[pixelY * img->width + pixelX];
                    newValue += pixelValue * kernel[ky + n][kx + n];
                }
            }
            if (newValue < 0) {
                newValue = 0;
            } 
            else if (newValue > 255) {
                newValue = 255;
            }
            newData[y * img->width + x] = (unsigned char)newValue;  // set the new value to the new data
        }
    }    

    for (unsigned int i = 0; i < img->dataSize; i++) {  // copy the new data to the original data
        img->data[i] = newData[i];
    }

    free(newData);
}
