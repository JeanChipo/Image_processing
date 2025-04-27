#include <stdio.h>
#include "bmp8.h"

int main() {
    t_bmp8 *image = bmp8_loadImage("./lena_gray.bmp");

    if (image == NULL) {
        printf("Erreur lors du chargement de l'image.\n");
        return 1;
    }

    bmp8_saveImage("lena_gray_copy.bmp", image);
    bmp8_printInfo(image);
    bmp8_free(image);
    return 0;
}
