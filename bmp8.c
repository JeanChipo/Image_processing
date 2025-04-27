#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"



t_bmp8 * bmp8_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb"); // rb for read binary
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    // Allouer de la mémoire pour l'image
    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));
    if (!img) {
        printf("Erreur : Impossible d'allouer de la mémoire pour l'image\n");
        fclose(file);
        return NULL;
    }

    // Lire l'en-tête
    if (fread(img->header, sizeof(unsigned char), 54, file) != 54) {
        printf("Erreur : Impossible de lire l'en-tête BMP\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Extraire les informations depuis l'en-tête
    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];

    // Vérifier que l'image est bien en 8 bits
    if (img->colorDepth != 8) {
        printf("Erreur : Image non supportée. Profondeur de couleur = %u (8 bits requis)\n", img->colorDepth);
        free(img);
        fclose(file);
        return NULL;
    }

    // Lire la table de couleur (1024 octets)
    if (fread(img->colorTable, sizeof(unsigned char), 1024, file) != 1024) {
        printf("Erreur : Impossible de lire la table de couleur\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Allouer de la mémoire pour les données d'image
    img->data = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char));
    if (!img->data) {
        printf("Erreur : Impossible d'allouer la mémoire pour les données de l'image\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Lire les données d'image
    if (fread(img->data, sizeof(unsigned char), img->dataSize, file) != img->dataSize) {
        printf("Erreur : Impossible de lire les données de l'image\n");
        free(img->data);
        free(img);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return img;
}

void bmp8_free(t_bmp8 *img) {
    if (img) {
        if (img->data) free(img->data);
        free(img);
    }
}

// ################### //

void bmp8_negative(t_bmp8 * img) {
    for (int i = 0; i < img->height; i++) {
        char * line = img->colorTable[i];
        for (int j = 0; j < img->width; j++) {
            line[j] = 255 - line[j];
        }
    }
}