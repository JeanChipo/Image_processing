#include <stdio.h>
#include "bmp8.h"

t_bmp8 * bmp8_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open BMP file.\n");
        return NULL;
    }
    
    size_t fread(void *data, size_t size, size_t nmemb, FILE *stream);
    
    int fclose(FILE *file);
    return (t_bmp8 *)file;
}


