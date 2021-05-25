#include <stdint.h>
#include <stddef.h>
#include <cstdio>

enum ImageType {
    PNG, JPG, BMP, TGA
};


struct Image {
    uint8_t* data = NULL; //1 byte
    size_t size = 0;
    int w;
    int h;
    int channels; // quantos valores de cores por pixel (RGB) = 3;

    Image(const char* fileName);
    Image(int w, int h, int channels); // imagem em branco
    Image(const Image& img); // copiar imagem
    ~Image(); //construtor padrao

    bool read(const char* fileName);
    bool write(const char* fileName);

    ImageType getFileType(const char* fileName);

    Image& grayscale_avg();
    Image& grayscale_lum();

    Image& sepia();

    Image& invertColors();
};