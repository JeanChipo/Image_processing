#include <stdio.h>
#include "bmp8.h"

int main() {
    t_bmp8 *image = bmp8_loadImage("./lena_gray.bmp");


    int choix = 0;
    printf("===image processing en c===\n\n");
    printf("quel traitement voulez-vous appliquer a l'image?\n 1) negatif\n 2) luminosite\n 3) filtre\n : ");
    scanf("%d", &choix);
    while (choix < 1 || choix > 3) {
        printf("\nchoix invalide, veuillez reessayer\n");
        scanf("%d", &choix);
    }

    switch (choix) {
        case 1:
            bmp8_negative(image);
            break;
        case 2:
            printf("entrez la valeur de luminosite (entre -255 et 255): \n");
            int brightness;
            scanf("%d", &brightness);
            while (brightness < -255 || brightness > 255) {
                printf("valeur invalide, veuillez reessayer\n");
                scanf("%d", &brightness);
            }
            bmp8_brightness(image, brightness);
            break;
        case 3:
            printf("choisissez le filtre:\n 1) box_ blur\n 2) gaussian_blur\n 3) outline\n 4) emboss\n 5) sharpen\n 6)custome : ");
            int filter_select = 0;
            scanf("%d", &filter_select);
            while (filter_select < 1 || filter_select > 5) {
                printf("choix invalide, veuillez reessayer\n");
                scanf("%d", &filter_select);
            }
            switch (filter_select)
            {
                case 1: 
                    int fliter1[3][3] = {
                        {1, 1, 1},
                        {1, 1, 1},
                        {1, 1, 1}
                    };
                    
                case 2:
                    int fliter2[3][3] = {
                        {1, 2, 1},
                        {2, 4, 2},
                        {1, 2, 1}
                    };
                case 3:
                    int fliter3[3][3] = {
                        {0, -1, 0},
                        {-1, 5, -1},
                        {0, -1, 0}
                    };
                case 4:
                    int fliter4[3][3] = {
                        {-2, -1, 0},
                        {-1, 1, 1},
                        {0, 1, 2}
                    };
                case 5:
                    int fliter5[3][3] = {
                        {0, -1, 0},
                        {-1, 5, -1},
                        {0, -1, 0}
                    };
                case 6:
                


                
            
            }
            break;
    }
    
    bmp8_printInfo(image);
    bmp8_saveImage("lena_gray_copy.bmp", image);
    bmp8_free(image);
    return 0;
}
