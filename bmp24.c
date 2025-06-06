/* 
Fichier de traitement d'images BMP 24 en couleur.
Il contient les fonctions de chargement, sauvegarde, et traitement d'images en couleur.
auteurs : @JeanChipo, @WarennOne
*/

#include <stdio.h>
#include <stdlib.h>
#include "bmp24.h"


// ################################################### //
// ### Lecture et écriture d’une image en couleurs ### //
// ################################################### //

t_bmp24 * bmp24_loadImage(const char * filename) {
    /* charge une image .bmp24 */
    // Ouvre le fichier BMP
    FILE *file = fopen(filename, "rb");

    t_bmp_header header;
    t_bmp_info info;

    // Lecture du header BMP 
    fread(&header.type, sizeof(uint16_t), 1, file);
    fread(&header.size, sizeof(uint32_t), 1, file);
    fread(&header.reserved1, sizeof(uint16_t), 1, file);
    fread(&header.reserved2, sizeof(uint16_t), 1, file);
    fread(&header.offset, sizeof(uint32_t), 1, file);

    // Lecture du header info BMP 
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
    /* sauvegarde une image .bmp24 */
    // Ouvre le fichier BMP en écriture
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
    // alloue de la mémoire pour un tableau de pixels 2D
    t_pixel **data = (t_pixel **)malloc(height * sizeof(t_pixel *));
    int y;

    // on parcours chaque ligne de l'image et alloue de la mémoire pour chaque ligne pour stocker les pixels
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
    return data;    // avec data le tableau de pixels
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    /* libère la mémoire allouée pour les pixels de l'image */
    for (int y = 0; y < height; y++) {
        free(pixels[y]);
    }
    free(pixels);
}


t_bmp24 * bmp24_allocate(t_bmp_header *header, t_bmp_info *header_info, int colorDepth) {
    /* alloue de la mémoire pour une image BMP 24 bits */
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24));

    // on alloue la mémoire pour t_bmp24
    img->header = *header;
    img->header_info = *header_info;
    img->width = header_info->width;
    img->height = header_info->height;
    img->colorDepth = colorDepth;
    img->data = bmp24_allocateDataPixels(img->width, img->height);

    if (img->data == NULL) {    // on s'assure que l'allocation de mémoire pour les pixels a réussi
        free(img);
        return NULL;
    }

    return img;
}

void bmp24_free(t_bmp24 *img) {
    /* libère la mémoire allouée pour une image BMP 24 bits */
    if ((img != NULL) && (img->data != NULL)) { // si l'image et les pixels existent
        bmp24_freeDataPixels(img->data, img->height);   // on libère les pixels
        }
    free(img);
}



// ########################################## //
// ### Traitements d’une image en couleur ### //
// ########################################## //

void bmp24_negative (t_bmp24 * img) {
    /* applique un effet négatif à l'image */
    // parcourt chaque pixel de l'image et inverse ses valeurs RGB
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].red = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue = 255 - img->data[y][x].blue;
        }
    }
}


void bmp24_grayscale (t_bmp24 * img) {
    /* convertit l'image en niveaux de gris */

    // parcourt chaque pixel de l'image et calcule la valeur moyenne des composantes RGB
    // puis assigne cette valeur à chaque composante RGB pour obtenir une image en niveaux de gris
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
    /* ajuste la luminosité de l'image */

    // parcourt chaque pixel de l'image et ajoute la valeur de luminosité à chaque composante RGB
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // vérification des limites de la valeur du rouge
            if (img->data[y][x].red + value > 255) {
                img->data[y][x].red = 255;
            } else if (img->data[y][x].red + value < 0) {
                img->data[y][x].red = 0;
            } else {
                img->data[y][x].red = img->data[y][x].red + value;
            }
            
            // vérification des limites de la valeur du vert
            if (img->data[y][x].green + value > 255) {
                img->data[y][x].green = 255;
            } else if (img->data[y][x].green + value < 0) {
                img->data[y][x].green = 0;
            } else {
                img->data[y][x].green = img->data[y][x].green + value;
            }

            // vérification des limites de la valeur du bleu
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


uint8_t clamp(int value) {
    /* rogne une valeur entre 0 et 255, utilisée dans bmp24_convolution */
    if (value < 0) return 0;
    else if (value > 255) return 255;
    else return (uint8_t)value;
}

t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize) {
    /* applique une convolution à un pixel de l'image en utilisant un noyau (masque) */
    int n = kernelSize / 2;
    float sum_red = 0, sum_green = 0, sum_blue = 0;
    t_pixel new_pixel;

    // on parcours le noyau (masque du filtre), et on applique le fitre sur les pixels voisins
    for (int ky = -n; ky <= n; ky++) {
        for (int kx = -n; kx <= n; kx++) {
            int pixelX = x + kx;
            int pixelY = y + ky;
            if (pixelX < 0 || pixelX >= img->width || pixelY < 0 || pixelY >= img->height) {
                continue; // on ignore les pixels en dehors des limites de l'image
            }
            float k = kernel[ky + n][kx + n];
            sum_red += k * img->data[pixelY][pixelX].red;
            sum_green += k * img->data[pixelY][pixelX].green;
            sum_blue += k * img->data[pixelY][pixelX].blue;
        }
    }

    // Rogner a [0,255] pour avoir des pixels valides
    new_pixel.red = clamp((int)sum_red);
    new_pixel.green = clamp((int)sum_green);
    new_pixel.blue = clamp((int)sum_blue);

    return new_pixel;
}
