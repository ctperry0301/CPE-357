#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

unsigned char getColor(int width, unsigned char *data, int x,  int y, int color);
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;

struct tagBITMAPFILEHEADER
{
    WORD bfType; //specifies the file type
    DWORD bfSize; //specifies the size in bytes of the bitmap file
    WORD bfReserved1; //reserved; must be 0
    WORD bfReserved2; //reserved; must be 0
    DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader tothe bitmap bits
};
struct tagBITMAPINFOHEADER {
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
};

int main(){
    FILE *file = fopen("jar.bmp", "rb");
    if (file == NULL){
        printf("file not found\n");
        return 0;
    }
    struct tagBITMAPFILEHEADER fh;
    struct tagBITMAPINFOHEADER fih;

    //GET FILE HEADER DATA for 1st image
    fread(&fh.bfType, 1, 2, file);
    fread(&fh.bfSize, 1, 4, file);
    fread(&fh.bfReserved1, 1, 2, file);
    fread(&fh.bfReserved2, 1, 2, file);
    fread(&fh.bfOffBits, 1, 4, file);

    //GET FILE INFO HEADER DATA for 1st image
    fread(&fih.biSize, 1, 4, file);
    fread(&fih.biWidth, 1, 4, file);
    fread(&fih.biHeight, 1, 4, file);
    int realWidth = fih.biWidth;
    int height = fih.biHeight;
    fread(&fih.biPlanes, 1, 2, file);
    fread(&fih.biBitCount, 1, 2, file);
    fread(&fih.biCompression, 1, 4, file);
    fread(&fih.biSizeImage, 1, 4, file);
    fread(&fih.biXPelsPerMeter, 1, 4, file);
    fread(&fih.biYPelsPerMeter, 1, 4, file);
    fread(&fih.biClrUsed, 1, 4, file);

    fseek(file, fh.bfOffBits, 0);
    unsigned char *data = malloc(fih.biSizeImage);
    fread(data, 1, fih.biSizeImage, file);

    for (int x = 0; x < realWidth; x++) {
        for (int y = 0; y < height; y++) {
            data[(realWidth * y + x) * 3 + 0] = 255 - getColor(realWidth, data, x, y, 0);
            data[(realWidth * y + x) * 3 + 1] = 255 - getColor(realWidth, data, x, y, 1);
            data[(realWidth * y + x) * 3 + 2] = 255 - getColor(realWidth, data, x, y, 2);
        }
    }
    fclose(file);
    FILE *resultFile;
    resultFile = fopen("result.bmp", "wb+");
    fwrite(&fh.bfType, 2, sizeof(unsigned char), resultFile);
    fwrite(&fh.bfSize, 1, 4, resultFile);
    fwrite(&fh.bfReserved1, 1, 2, resultFile);
    fwrite(&fh.bfReserved2, 1, 2, resultFile);
    fwrite(&fh.bfOffBits, 1, 4, resultFile);
    fwrite(&fih, 1, sizeof(struct tagBITMAPINFOHEADER), resultFile);
    fwrite(data, fih.biSizeImage, 1, resultFile);
    fclose(resultFile);
    return 0;
}

unsigned char getColor(int width, unsigned char *data, int x, int y, int color) {
    return data[(width * y + x) * 3 + color];
}