#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef unsigned short WORD;
typedef unsigned int DWORD; 
typedef unsigned int LONG; 

typedef struct tagBITMAPFILEHEADER
{
    char bfType[2]; //specifies the file type
    int bfSize; //specifies the size in bytes of the bitmap file
    WORD bfReserved1; //reserved; must be 0
    WORD bfReserved2; //reserved; must be 0
    DWORD bfOffBits; //species the offset in bytes
} tagBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize; //specifies the number of bytes required by the struct
    LONG biWidth; //specifies width in pixels
    LONG biHeight; //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage; //size of image in bytes
    LONG biXPelsPerMeter; //number of pixels per meter in x axis
    LONG biYPelsPerMeter; //number of pixels per meter in y axis
    DWORD biClrUsed; //number of colors used by th ebitmap
    DWORD biClrImportant; //number of colors that are important
} tagBITMAPINFOHEADER;


int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("bad file\n");
    }
    struct tagBITMAPFILEHEADER fh;
    struct tagBITMAPINFOHEADER fih;
    fread(&fh.bfType, 1, 2, file);
    fread(&fh.bfSize, 1, 4, file);
    fread(&fh.bfReserved1, 1, 2, file);
    fread(&fh.bfReserved2, 1, 2, file);
    fread(&fh.bfOffBits, 1, 4, file);

    fread(&fih.biSize, 1, 4, file);
    fread(&fih.biWidth, 1, 4, file);
    fread(&fih.biHeight, 1, 4, file);
    fread(&fih.biPlanes, 1, 2, file);
    fread(&fih.biBitCount, 1, 2, file);
    fread(&fih.biCompression, 1, 4, file);
    fread(&fih.biSizeImage, 1, 4, file);
    
    fseek(file, fh.bfOffBits, 0);
    unsigned char *data = malloc(fih.biSizeImage);
    fread(data, 1, fih.biSizeImage, file);
    
    for (int x = 0; x < fih.biWidth; x++) {
        for (int y = 0; y < fih.biHeight; y++) {
            float a = (float) data[(fih.biWidth * y + x) * 3 + 0] / 255;
            float b = (float) data[(fih.biWidth * y + x) * 3 + 1] / 255;
            float c = (float) data[(fih.biWidth * y + x) * 3 + 2] / 255;
            data[(fih.biWidth * y + x) * 3 + 0] = a * a * 255;
            data[(fih.biWidth * y + x) * 3 + 1] = b * b * 255;
            data[(fih.biWidth * y + x) * 3 + 2] = c * c * 255;
        }
    }
    
    FILE *outputFile;
    outputFile = fopen(argv[2], "wb+");
    fwrite(&fh.bfType, 2, sizeof(unsigned char), outputFile);
    fwrite(&fh.bfSize, 1, 4, outputFile);
    fwrite(&fh.bfReserved1, 1, 2, outputFile);
    fwrite(&fh.bfReserved2, 1, 2, outputFile);
    fwrite(&fh.bfOffBits, 1, 4, outputFile);
    
    fwrite(&fih, 1, sizeof(struct tagBITMAPINFOHEADER), outputFile);
    fwrite(data, fih.biSizeImage, 1, outputFile);

    fclose(file);
    fclose(outputFile);
    return 0;
}