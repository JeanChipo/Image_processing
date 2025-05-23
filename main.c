#include <stdio.h>
#include "bmp8.h"
#include "bmp24.h"

void process_brightness(t_bmp8 * image) {
    int brightness;
    printf("entrez la valeur de luminosite (entre -255 et 255): \n");
    scanf("%d", &brightness);
    while (brightness < -255 || brightness > 255) {
        printf("valeur invalide, veuillez reessayer\n");
        scanf("%d", &brightness);
    }
    bmp8_brightness(image, brightness);
}

void process_filters(t_bmp8 * image) {
    printf("choisissez le filtre:\n 1) box_blur\n 2) gaussian_blur\n 3) outline\n 4) emboss\n 5) sharpen\n 6) custom : ");
    int filter_select = 1;
    scanf("%d", &filter_select);
    while (filter_select < 1 || filter_select > 6) {
        printf("choix invalide, veuillez reessayer\n");
        scanf("%d", &filter_select);
    }
    switch (filter_select) {
        case 1: {
            float filter1[3][3] = {
                {0.11, 0.11, 0.11},
                {0.11, 0.11, 0.11},
                {0.11, 0.11, 0.11}
            };
            float *kernel1[3] = { filter1[0], filter1[1], filter1[2] };
            bmp8_applyFilter(image, kernel1, 3);
            break;
        }
        case 2: {
            float filter2[3][3] = {
                {0.0625, 0.125, 0.0625},
                {0.125, 0.25, 0.125},
                {0.0625, 0.125, 0.0625}
            };
            float *kernel2[3] = { filter2[0], filter2[1], filter2[2] };
            bmp8_applyFilter(image, kernel2, 3);
            break;
        }
        case 3: {
            float filter3[3][3] = {
                {0, -1, 0},
                {-1, 4, -1},
                {0, -1, 0}
            };
            float *kernel3[3] = { filter3[0], filter3[1], filter3[2] };
            bmp8_applyFilter(image, kernel3, 3);
            break;
        }
        case 4: {
            float filter4[3][3] = {
                {-2, -1, 0},
                {-1, 1, 1},
                {0, 1, 2}
            };
            float *kernel4[3] = { filter4[0], filter4[1], filter4[2] };
            bmp8_applyFilter(image, kernel4, 3);
            break;
        }
        case 5: {
            float filter5[3][3] = {
                {0, -1, 0},
                {-1, 5, -1},
                {0, -1, 0}
            };
            float *kernel5[3] = { filter5[0], filter5[1], filter5[2] };
            bmp8_applyFilter(image, kernel5, 3);
            break;
        }
        case 6: {
            float filter6[3][3];
            printf("Entrez les 9 valeurs du filtre 3x3 :\n");
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    printf("Valeur [%d][%d] : ", i, j);
                    float value;
                    scanf("%f", &value);
                    while(value < -255 || value > 255) {
                        printf("Valeur invalide, veuillez entrer une valeur entre -255 et 255 : ");
                        scanf("%f", &value);
                    }
                    filter6[i][j] = value;
                }
            }
            float *kernel6[3] = { filter6[0], filter6[1], filter6[2] };
            bmp8_applyFilter(image, kernel6, 3);
            break;
        }
    }
}


int main() {
    int bmp_ver; 
    printf("quel format de bmp utilise votre image? \n 1) bmp8 - niveau de gris \n 2) bmp24 - en couleur \n");
    scanf("%d", &bmp_ver);
    while (bmp_ver < 1 || bmp_ver > 2) {
        printf("format invalide, veuillez saisir si votre bmp est sur 8 bits (niveau de gris) ou 24 bits (en couleur) \n");
        scanf("%d", &bmp_ver);
    }
    switch (bmp_ver) {
        case 1: {
            t_bmp8 *image = bmp8_loadImage("./lena_gray.bmp");

            int choix = 0;
            printf("quel traitement voulez-vous appliquer a l'image?\n 1) negatif\n 2) luminosite\n 3) threshold\n 4) filtres: ");
            scanf("%d", &choix);
            while (choix < 1 || choix > 4) {
                printf("\nchoix invalide, veuillez reessayer\n");
                scanf("%d", &choix);
            }
        
            switch (choix) {
                case 1: {
                    bmp8_negative(image);
                    break;
                }
                case 2: {
                    process_brightness(image);
                    break;
                }
                case 3: {
                    bmp8_threshold(image);
                    break;
                }
                case 4: {
                    process_filters(image);
                    break;
                }
            }

            bmp8_printInfo(image);
            bmp8_saveImage("lena_gray_copy.bmp", image);
            bmp8_free(image);
            printf("Traitement termine. Image sauvegardee sous 'lena_gray_copy.bmp'.\n");

        break;
        }
        case 2: {
            t_bmp24 *image = bmp24_loadImage("./lena_color.bmp");
            printf("choisissez le traitement a appliquer:\n 1) negatif\n 2) niveaux de gris\n");
            int choix = 0;
            scanf("%d", &choix);
            switch (choix) {
                case 1: {
                    bmp24_negative(image);
                    break;
                }
                case 2: {
                    bmp24_grayscale (image);
                    break;
                }
            }
            bmp24_saveImage("lena_color_copy.bmp", image);
            // hello
            break;
        } 
    }

    return 0;
}
