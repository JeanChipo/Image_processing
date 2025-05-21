#include <stdio.h>
#include "bmp24.h"


t_bmp8 * bmp24_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb"); // rb for read binary
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24)); // allocate memory for the image structure
    t_bmp_header *header = (t_bmp_header *)malloc(sizeof(t_bmp_header)); // allocate memory for the header

    header->type = fread(header, sizeof(unsigned char), 16, file)
    header->size = fread(header, sizeof(unsigned char), 32, file)
    header->reserved1 = fread(header, sizeof(unsigned char), 16, file)
    header->reserved2 = fread(header, sizeof(unsigned char), 16, file)
    header->offset = fread(header, sizeof(unsigned char), 32, file)

    
        
    int *data_list = bmp24_allocate (int width, int height, int colorDepth);

    for (int i = 0; i < width*height*3; i++) {
        fread(data_list[i], sizeof(unsigned char), sizeof(uint8_t), file);
    }

    fclose(file);
    return img;
}

t_pixel ** bmp24_allocateDataPixels (int width, int height) {
    t_pixel **data = (t_pixel **)malloc(height * sizeof(t_pixel *));
    int y;
    for (y=0, y<height; y++;) {
        data[y] = (t_pixel *)malloc(width * sizeof(t_pixel));
        if (data[y] == NULL) {
            printf("Erreur d'allocation de mémoire pour la ligne %d\n", y);
            for (int j = 0; j < y; j++) {
                free(data[j]);
            }
            free(data);
            return NULL;
        }
    }
    return data;
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    for (int y = 0; y < height; y++) {
        free(pixels[y]);
    }
    free(pixels);
}


t_bmp24 * bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24));
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->data = bmp24_allocateDataPixels(width, height);

    if (img->data == NULL) {
        free(img);
        return NULL;
    }

    return img;
}

void bmp24_free(t_bmp24 *img) {
    if ((img != NULL) && (img->data != NULL)) {
        bmp24_freeDataPixels(img->data, img->height);
        }
    free(img);
}