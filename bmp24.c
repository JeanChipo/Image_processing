#include <stdio.h>
#include "bmp24.h"


t_pixel ** bmp24_allocateDataPixels (int width, int height) {
// Allouer dynamiquement de la mémoire pour une matrice de t_pixel de taille
// witdh × height, et renvoyer l’adresse allouée dans le tas. Si l’allocation échoue, la fonction affiche un
// message d’erreur et renvoie NULL.
}

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