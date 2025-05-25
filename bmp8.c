#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"


// ########################################################## //
// ### Lecture et écriture d’une image en niveaux de gris ### //
// ########################################################## //

t_bmp8 * bmp8_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb"); // rb for read binary
    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file); // lecture du header dans un tableau local de 54 octets

    for (int i = 0; i < 54; i++) {
        img->header[i] = header[i];  // copie du header dans le champ header de la structure t_bmp8
    }

    // lecture de la table de couleurs (palette) pour les BMP 8 bits
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    img->width = *(unsigned int *)&header[18];
    img->height = *(unsigned int *)&header[22];
    img->colorDepth = *(unsigned short *)&header[28]; // color depth fait 2 bytes -> unsigned short
    img->dataSize = *(unsigned int *)&header[34];

    // allocation de mémoire pour les données de l'image
    img->data = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char)); 

    // si la taille des données n'est pas spécifiée, on la calcule
    if (img->dataSize == 0)
        img->dataSize = ((img->width * img->colorDepth + 31)/32) * 4 * img->height;

    fread(img->data, sizeof(unsigned char), img->dataSize, file); // lecture des données de l'image
    fclose(file);
    return img;
}

void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    FILE *file = fopen(filename, "wb"); // wb pour write binary

    // écriture du header, de la table de couleurs, et des données de l'image dans le fichier
    fwrite(img->header, sizeof(unsigned char), 54, file); 
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file); 
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file); 
    fclose(file);
}

void bmp8_free(t_bmp8 *img) {
    // libération de la mémoire allouée pour l'image
    if (img) {
        if (img->data) {
            free(img->data); 
        }
        free(img);
    }
}


// ###################################################### //
// ###   Traitements d’une image en niveaux de gris   ### //
// ###################################################### //

void bmp8_printInfo(t_bmp8 *img) {
    printf("\n");
    printf("Image Info:\n");
    printf("  | Width: %u \n", img->width);
    printf("  | Height: %u \n", img->height);
    printf("  | Color Depth: %u \n", img->colorDepth);
    printf("  | Data Size: %u \n", img->dataSize);
    printf("\n");
}

void bmp8_negative(t_bmp8 * img) {
    // parcours chaque pixel de l'image et inverse sa valeur
    for (unsigned int i = 0; i < img->dataSize; i++) {
        unsigned int NewValue = 255 - img->data[i] ;
        img->data[i] = NewValue ;
    }
}


void bmp8_brightness(t_bmp8 * img, int brightness) {
    // parcours chaque pixel et ajoute la valeur de luminosité
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

void bmp8_threshold(t_bmp8 * img) {
    // parcours chaque pixel et change la valeur a 255 ou 0 si elle est au dessus ou en dessous de 128
    for (unsigned int i = 0; i < img->dataSize; i++) {
        if (img->data[i] < 128) {
            img->data[i] = 0;
        } else {
            img->data[i] = 255;
        }
    }
}

// ######################## //
// ### Filtres d’images ### //
// ######################## //

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    int n = kernelSize / 2;
    unsigned char *newData = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char));

    // copie des données de l'image dans le nouveau tableau
    for (unsigned int i = 0; i < img->dataSize; i++) {
        newData[i] = img->data[i];
    }

    // on parcours chaque pixel de l'image, en évitant les bords
    for (unsigned int y = n; y < img->height - n; y++) {    
        for (unsigned int x = n; x < img->width - n; x++) {
            float newValue = 0.0;

            for (int ky = -n; ky <= n; ky++) {  // on applique le filtre sur les pixels voisins (grille de 3x3 autour)
                for (int kx = -n; kx <= n; kx++) {
                    int pixelX = x + kx;
                    int pixelY = y + ky;
                    unsigned char pixelValue = img->data[pixelY * img->width + pixelX]; // accès au pixel dans les données de l'image
                    newValue += pixelValue * kernel[ky + n][kx + n];    // attribution
                }
            }

            // on fixe un plafond pour que la nouvelle valeur soit comprise entre 0 et 255
            if (newValue < 0) {
                newValue = 0;
            } 
            else if (newValue > 255) {
                newValue = 255;
            }

            newData[y * img->width + x] = (unsigned char)newValue;  // on applique la nouvelle valeur au pixel
        }
    }    

    for (unsigned int i = 0; i < img->dataSize; i++) {  // on copie les nouvelles données dans l'image
        img->data[i] = newData[i];
    }

    free(newData);  // ne pas oublier de libérer la mémoire allouée
}
