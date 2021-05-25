/*
Feito por:

    Luis Durante
    Guilherme Rabelo
    Victor Moreno
    Vinicius Stein
    Mauricio Freire
*/

#include "Image.h"
#include <string>
#include <vector>
#include <dirent.h>
#include <cstring>
#include <iostream>

std::vector<std::string> ListDir(const std::string& path);
bool applyGrayScaleFilter(const Image& img, std::string fileName);
bool applySepiaFilter(const Image& img, std::string fileName);
bool applyInvertedColorsFilter(const Image& img, std::string fileName);

int main(int argc, char** argv) {
    std::string imagesFolder = "./images/";
    std::vector<std::string> images = ListDir(imagesFolder);

    #pragma omp parallel for
    for (size_t i = 0; i < images.size(); i++){
        std::string _image = imagesFolder + images[i];

        // conversão de string para array de char
        int n = _image.length();  
        char fileName[n + 1];
        strcpy(fileName, _image.c_str());

        try
        {
            Image originalImage(fileName);

            #pragma omp parallel
            #pragma omp sections 
            {
                #pragma omp section
                    applyGrayScaleFilter(originalImage, images[i]);

                #pragma omp section
                    applySepiaFilter(originalImage, images[i]);

                #pragma omp section
                    applyInvertedColorsFilter(originalImage, images[i]);
            }

        }
        catch(const std::exception& e)
        {
            continue;
        }
    }

    return 0;
}

std::vector<std::string> ListDir(const std::string& path) {
    struct dirent *entry;
    DIR *dp;
    std::vector<std::string> images;

    dp = ::opendir(path.c_str());
    if (dp == NULL) {
        perror("opendir: Path does not exist or could not be read.");
        exit(1);
        return images;
    }

    while ((entry = ::readdir(dp))) {
        if (strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0 ) continue;
        images.push_back(entry->d_name);
    }

    ::closedir(dp);
    return images;
}

bool applyGrayScaleFilter(const Image& img, std::string fileName) {
    Image gray = img;
    try
    {
        gray.grayscale_lum();
    }
    catch(const std::exception& e)
    {
        std::cout << "Erro ao gravar arquivo " << fileName << " em tons de cinza\n";
        return false;
    }
    
    

    std::string grayScaleFileName = "./results/grayScale-" + fileName;

    // conversão de string para array de char
    int n = grayScaleFileName.length();
    char filteredFileName[n + 1];
    strcpy(filteredFileName, grayScaleFileName.c_str());

    gray.write(filteredFileName);
    return true;
}

bool applySepiaFilter(const Image& img, std::string fileName) {
    Image sepia = img;
    try
    {
        sepia.sepia();
    }
    catch(const std::exception& e)
    {
        std::cout << "Erro ao gravar arquivo " << fileName << " em sepia\n";
        return false;
    }
    

    std::string sepiaFileName = "./results/sepia-" + fileName;

    // conversão de string para array de char
    int n = sepiaFileName.length();
    char filteredFileName[n + 1];
    strcpy(filteredFileName, sepiaFileName.c_str());

    sepia.write(filteredFileName);
    return true;
}

bool applyInvertedColorsFilter(const Image& img, std::string fileName) {
    Image inverted = img;
    try
    {
        inverted.invertColors();
    }
    catch(const std::exception& e)
    {
        std::cout << "Erro ao gravar arquivo " << fileName << " em cores invertidas\n";
    }

    std::string invertedColorsFileName = "./results/invertedColors-" + fileName;

    // conversão de string para array de char
    int n = invertedColorsFileName.length();
    char filteredFileName[n + 1];
    strcpy(filteredFileName, invertedColorsFileName.c_str());

    inverted.write(filteredFileName);
    return true;
}
