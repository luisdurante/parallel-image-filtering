#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <cmath>
#include <string>

class MyException : public std::exception
{
    private:
       std::string ex;
    public:
        MyException(const char* err) : ex(err) {}       
};

Image::Image(const char* fileName) {
    if(read(fileName)) {
        printf("Lendo %s\n", fileName);
        size = w * h * channels;
    } else {
        printf("Falha na leitura %s\n", fileName);
        throw MyException("Exception");
    }
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels){
    size = w * h * channels;
    data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w,img.h,img.channels) {
    memcpy(data, img.data, img.size);
}

Image::~Image() {
    stbi_image_free(data);
}

bool Image::read(const char* fileName) {
    data = stbi_load(fileName, &w, &h, &channels, 0);
    return data != NULL;
}

bool Image::write(const char* fileName) {
    ImageType type = getFileType(fileName);
	int success;
    switch (type) {
        case PNG:
            success = stbi_write_png(fileName, w, h, channels, data, w * channels);
        break;
        case BMP:
            success = stbi_write_bmp(fileName, w, h, channels, data);
        break;
        case JPG:
            success = stbi_write_jpg(fileName, w, h, channels, data, 100);
        break;
        case TGA:
            success = stbi_write_tga(fileName, w, h, channels, data);
        break;
    }
    return success != 0;
}

ImageType Image::getFileType(const char* fileName) {
    const char* ext = strrchr(fileName, '.');
	if(ext != nullptr) {
		if(strcmp(ext, ".png") == 0) {
			return PNG;
		}
		else if(strcmp(ext, ".jpg") == 0) {
			return JPG;
		}
		else if(strcmp(ext, ".bmp") == 0) {
			return BMP;
		}
		else if(strcmp(ext, ".tga") == 0) {
			return TGA;
		}
	}
	return PNG;
}

Image& Image::grayscale_lum() {
    // preserva a luminosidade
    if (channels < 3) {
        throw MyException("Exception");
    } else {
        #pragma omp parallel for
        for (size_t i = 0; i < size; i+= channels)
        {
            // https://en.wikipedia.org/wiki/Grayscale Colourimetric (perceptual luminance-preserving) conversion to greyscale)
            int gray = data[i] * 0.2126 + data[i+1] * 0.7152 + data[i+3] * 0.0722;
            memset(data+i, gray, 3);
        }
    }
    return *this;
}

Image& Image::sepia() {
    if (channels < 3) {
        throw MyException("Exception");
    } else {
        #pragma omp parallel for
        for (size_t i = 0; i < size; i+= channels)
        {
            // https://www.techrepublic.com/blog/how-do-i/how-do-i-convert-images-to-grayscale-and-sepia-tone-using-c/
            uint8_t inputRed = data[i]; 
            uint8_t inputGreen = data[i+1];
            uint8_t inputBlue = data[i+2];

            int red = (inputRed * 0.393) + (inputGreen * 0.769) + (inputBlue * 0.189); 
            int green = (inputRed * 0.349) + (inputGreen * 0.686) + (inputBlue * 0.168);
            int blue = (inputRed * 0.272) + (inputGreen * 0.534) + (inputBlue * 0.131);

            if(red > 255) red = 255;
            if(red < 0 ) red = 0;
            if(green > 255) green = 255;
            if(green < 0 ) green = 0;
            if(blue > 255) blue = 255;
            if(blue < 0 ) blue = 0;

            data[i] = red;
            data[i+1] = green;
            data[i+2] = blue;
        }
    }
    return *this;
}

Image& Image::invertColors() {
    if (channels < 3) {
        throw MyException("Exception");
    } else {
        
        #pragma omp parallel for
        for (size_t i = 0; i < size; i+= channels)
        {
            data[i] = 255 - data[i];
            data[i+1] = 255 - data[i+1];
            data[i+2] = 255 - data[i+2];
        }
    }
    return *this;
}