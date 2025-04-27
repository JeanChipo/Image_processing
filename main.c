#include <stdio.h>
#include "bmp8.h"

int main() {
    t_bmp8 *image = bmp8_loadImage("./lena_gray.bmp");

    if (image == NULL) {
        printf("Erreur lors du chargement de l'image.\n");
        return 1;
    }

    printf("Image chargée avec succès !\n");
    printf("Largeur : %u pixels\n", image->width);
    printf("Hauteur : %u pixels\n", image->height);
    printf("Profondeur de couleur : %u bits\n", image->colorDepth);

    // Libérer la mémoire
    bmp8_free(image);
    return 0;
}
