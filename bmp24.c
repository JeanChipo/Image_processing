#include <stdio.h>
#include <stdlib.h>
#include "bmp24.h"


t_bmp24 * bmp24_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb"); // rb for read binary
    t_bmp24 * img;
    
    t_bmp_header *header = (t_bmp_header *)malloc(sizeof(t_bmp_header)); // allocate memory for the header
    header->type = fread(header, sizeof(unsigned char), 16, file);
    header->size = fread(header, sizeof(unsigned char), 32, file);
    header->reserved1 = fread(header, sizeof(unsigned char), 16, file);
    header->reserved2 = fread(header, sizeof(unsigned char), 16, file);
    header->offset = fread(header, sizeof(unsigned char), 32, file);

    t_bmp_info *info = (t_bmp_info *)malloc(sizeof(t_bmp_info));
    info->size = fread(info, sizeof(unsigned char), 32, file);
    info->width = fread(info, sizeof(unsigned char), 32, file);
    info->height = fread(info, sizeof(unsigned char), 32, file);
    info->planes = fread(info, sizeof(unsigned char), 16, file);
    info->bits = fread(info, sizeof(unsigned char), 16, file);
    info->compression = fread(info, sizeof(unsigned char), 32, file);
    info->imagesize = fread(info, sizeof(unsigned char), 32, file);
    info->xresolution = fread(info, sizeof(unsigned char), 32, file);
    info->yresolution = fread(info, sizeof(unsigned char), 32, file);
    info->ncolors = fread(info, sizeof(unsigned char), 32, file);
    info->importantcolors = fread(info, sizeof(unsigned char), 32, file);
        
    img = bmp24_allocate (header, info, 24);

    fclose(file);
    free(header);
    free(info);
    return img;
}

void bmp24_saveImage(const char * filename, t_bmp24 * img) {
    FILE *file = fopen(filename, "wb");

    // Écriture du header BMP (14 octets)
    t_bmp_header *header = &img->header;
    fwrite(&header->type, sizeof(uint16_t), 16, file);
    fwrite(&header->size, sizeof(uint32_t), 32, file);
    fwrite(&header->reserved1, sizeof(uint16_t), 16, file);
    fwrite(&header->reserved2, sizeof(uint16_t), 16, file);
    fwrite(&header->offset, sizeof(uint32_t), 32, file);
    free(header);

    // Écriture du header info BMP (généralement 40 octets)
    t_bmp_info *header_info = &img->header_info;
    fwrite(&header_info->size, sizeof(uint32_t), 32, file);
    fwrite(&header_info->width, sizeof(int32_t), 32, file);
    fwrite(&header_info->height, sizeof(int32_t), 32, file);
    fwrite(&header_info->planes, sizeof(uint16_t), 16, file);
    fwrite(&header_info->bits, sizeof(uint16_t), 16, file);
    fwrite(&header_info->compression, sizeof(uint32_t), 32, file);
    fwrite(&header_info->imagesize, sizeof(uint32_t), 32, file);
    fwrite(&header_info->xresolution, sizeof(int32_t), 32, file);
    fwrite(&header_info->yresolution, sizeof(int32_t), 32, file);
    fwrite(&header_info->ncolors, sizeof(uint32_t), 32, file);
    fwrite(&header_info->importantcolors, sizeof(uint32_t), 32, file);

    // Écriture des pixels (BGR, ligne par ligne, avec padding)
    for (int i = 0; i < img->header_info.width*img->header_info.height; i++) {
        t_pixel *pixel = img->data[i];
        fwrite(&pixel->red, sizeof(uint8_t), 8, file);
        fwrite(&pixel->green, sizeof(uint8_t), 8, file);
        fwrite(&pixel->blue, sizeof(uint8_t), 8, file);
    }

    fclose(file);
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


t_bmp24 * bmp24_allocate(t_bmp_header *header, t_bmp_info *header_info, int colorDepth) {
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24));
    if (!img) return NULL;

    img->header = *header;
    img->header_info = *header_info;
    img->width = header_info->width;
    img->height = header_info->height;
    img->colorDepth = colorDepth;
    img->data = bmp24_allocateDataPixels(img->width, img->height);

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