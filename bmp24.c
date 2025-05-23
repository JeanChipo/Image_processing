#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp24.h"

t_bmp24 * bmp24_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb");

    t_bmp_header header;
    t_bmp_info info;

    // Lecture du header BMP (14 octets)
    fread(&header.type, sizeof(uint16_t), 1, file);
    fread(&header.size, sizeof(uint32_t), 1, file);
    fread(&header.reserved1, sizeof(uint16_t), 1, file);
    fread(&header.reserved2, sizeof(uint16_t), 1, file);
    fread(&header.offset, sizeof(uint32_t), 1, file);

    // Lecture du header info BMP (40 octets)
    fread(&info.size, sizeof(uint32_t), 1, file);
    fread(&info.width, sizeof(int32_t), 1, file);
    fread(&info.height, sizeof(int32_t), 1, file);
    fread(&info.planes, sizeof(uint16_t), 1, file);
    fread(&info.bits, sizeof(uint16_t), 1, file);
    fread(&info.compression, sizeof(uint32_t), 1, file);
    fread(&info.imagesize, sizeof(uint32_t), 1, file);
    fread(&info.xresolution, sizeof(int32_t), 1, file);
    fread(&info.yresolution, sizeof(int32_t), 1, file);
    fread(&info.ncolors, sizeof(uint32_t), 1, file);
    fread(&info.importantcolors, sizeof(uint32_t), 1, file);

    t_bmp24 *img = bmp24_allocate(&header, &info, 24);

    // Aller à l'offset des données
    fseek(file, header.offset, SEEK_SET);

    int width = info.width;
    int height = info.height;
    int padding = (4 - (width * 3) % 4) % 4;

    // Lecture des pixels (BGR, bottom-up)
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            uint8_t bgr[3];
            fread(bgr, 1, 3, file);
            img->data[y][x].blue = bgr[0];
            img->data[y][x].green = bgr[1];
            img->data[y][x].red = bgr[2];
        }
        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);
    return img;
}

void bmp24_saveImage(const char * filename, t_bmp24 * img) {
    FILE *file = fopen(filename, "wb");
    if (!file) return;

    // Écriture du header BMP (14 octets)
    fwrite(&img->header.type, sizeof(uint16_t), 1, file);
    fwrite(&img->header.size, sizeof(uint32_t), 1, file);
    fwrite(&img->header.reserved1, sizeof(uint16_t), 1, file);
    fwrite(&img->header.reserved2, sizeof(uint16_t), 1, file);
    fwrite(&img->header.offset, sizeof(uint32_t), 1, file);

    // Écriture du header info BMP (40 octets)
    fwrite(&img->header_info.size, sizeof(uint32_t), 1, file);
    fwrite(&img->header_info.width, sizeof(int32_t), 1, file);
    fwrite(&img->header_info.height, sizeof(int32_t), 1, file);
    fwrite(&img->header_info.planes, sizeof(uint16_t), 1, file);
    fwrite(&img->header_info.bits, sizeof(uint16_t), 1, file);
    fwrite(&img->header_info.compression, sizeof(uint32_t), 1, file);
    fwrite(&img->header_info.imagesize, sizeof(uint32_t), 1, file);
    fwrite(&img->header_info.xresolution, sizeof(int32_t), 1, file);
    fwrite(&img->header_info.yresolution, sizeof(int32_t), 1, file);
    fwrite(&img->header_info.ncolors, sizeof(uint32_t), 1, file);
    fwrite(&img->header_info.importantcolors, sizeof(uint32_t), 1, file);

    int width = img->width;
    int height = img->height;
    int padding = (4 - (width * 3) % 4) % 4;
    uint8_t pad[3] = {0, 0, 0};

    // Écriture des pixels (BGR, bottom-up)
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            uint8_t bgr[3] = {
                img->data[y][x].blue,
                img->data[y][x].green,
                img->data[y][x].red
            };
            fwrite(bgr, 1, 3, file);
        }
        fwrite(pad, 1, padding, file);
    }

    fclose(file);
}

t_pixel ** bmp24_allocateDataPixels (int width, int height) {
    t_pixel **data = (t_pixel **)malloc(height * sizeof(t_pixel *));
    int y;
    for (y = 0; y < height; y++) {
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





void bmp24_negative (t_bmp24 * img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].red = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue = 255 - img->data[y][x].blue;
        }
    }
}



void bmp24_grayscale (t_bmp24 * img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t gray = (img->data[y][x].red + img->data[y][x].green + img->data[y][x].blue) / 3;
            img->data[y][x].red = gray;
            img->data[y][x].green = gray;
            img->data[y][x].blue = gray;
        }
    }
}


void bmp24_brightness (t_bmp24 * img, int value) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            if (img->data[y][x].red + value > 255) {
                img->data[y][x].red = 255;
            } else if (img->data[y][x].red + value < 0) {
                img->data[y][x].red = 0;
            } else {
                img->data[y][x].red = img->data[y][x].red + value;
            }

            if (img->data[y][x].green + value > 255) {
                img->data[y][x].green = 255;
            } else if (img->data[y][x].green + value < 0) {
                img->data[y][x].green = 0;
            } else {
                img->data[y][x].green = img->data[y][x].green + value;
            }

            if (img->data[y][x].blue + value > 255) {
                img->data[y][x].blue = 255;
            } else if (img->data[y][x].blue + value < 0) {
                img->data[y][x].blue = 0;
            } else {
                img->data[y][x].blue = img->data[y][x].blue + value;
            }
        }
    }
}