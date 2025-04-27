#ifndef BMP8
#define BMP8

typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;

    unsigned int width; // offset à 18 de l'en-tête  
    unsigned int height; // offset à 22 de l'en-tête
    unsigned int colorDepth; // offset à 28 de l'en-tête
    unsigned int dataSize; // offset à 34 de l'en-tête
} t_bmp8;

t_bmp8 * bmp8_loadImage(const char * filename);
void bmp8_saveImage(const char * filename, t_bmp8 * img);
void bmp8_free(t_bmp8 * img);
void bmp8_printInfo(t_bmp8 * img);
void bmp8_negative(t_bmp8 * img);
void bmp8_brightness(t_bmp8 * img, int brightness);
#endif // BMP8