// Cole Perry
// CPE 357 Project 1
// Write a program that blends 2 given bmp images
// January 26 2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct tagBITMAPINFOHEADER
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
};

int main(int argc, char *argv[]){
    //ERROR HANDLING
    if (argc != 5) {
        printf("1: Command Line should be in the form [programname] [imagefile1] [imagefile2] [ratio] [outputfile]\n");
        return 0;
    }

    //ARGV variiables
    FILE *file1;
    FILE *file2;
    float ratio = atof(argv[3]);
    file1 = fopen(argv[1], "rb");
    file2 = fopen(argv[2], "rb");
   
    //ERROR HANDLING
    if (file1 == NULL){ 
        printf("2: Command Line should be in the form [programname] [imagefile1] [imagefile2] [ratio] [outputfile]\n");
        fclose(file1);
        fclose(file2);
        return 0;
    }

    //ERROR HANDLING
    else if(file2 == NULL) { 
        printf("3: Command Line should be in the form [programname] [imagefile1] [imagefile2] [ratio] [outputfile]\n");
        fclose(file1);
        fclose(file2);
        return 0;
    }

    //ERROR HANDLING
    else if ((ratio > 1) || (ratio < 0)) {
        printf("4: Command Line should be in the form [programname] [imagefile1] [imagefile2] [ratio] [outputfile]\n");
        fclose(file1);
        fclose(file2);
        return 0;
    }

    //Blend 2 given images and write to new file
    else {
        struct tagBITMAPFILEHEADER fh;
        struct tagBITMAPINFOHEADER fih;

        //GET FILE HEADER DATA for 1st image
        fread(&fh.bfType, 1, 2, file1);
        fread(&fh.bfSize, 1, 4, file1);
        fread(&fh.bfReserved1, 1, 2, file1);
        fread(&fh.bfReserved2, 1, 2, file1);
        fread(&fh.bfOffBits, 1, 4, file1);

        //GET FILE INFO HEADER DATA for 1st image
        fread(&fih.biSize, 1, 4, file1);
        fread(&fih.biWidth, 1, 4, file1);
        fread(&fih.biHeight, 1, 4, file1);
        int realWidth = fih.biWidth;
        int height = fih.biHeight;

        fread(&fih.biPlanes, 1, 2, file1);
        fread(&fih.biBitCount, 1, 2, file1);
        fread(&fih.biCompression, 1, 4, file1);
        fread(&fih.biSizeImage, 1, 4, file1);
        fread(&fih.biXPelsPerMeter, 1, 4, file1);
        fread(&fih.biYPelsPerMeter, 1, 4, file1);
        fread(&fih.biClrUsed, 1, 4, file1);

        struct tagBITMAPFILEHEADER fh2;
        struct tagBITMAPINFOHEADER fih2;

        //GET FILE HEADER DATA for 2nd image
        fread(&fh2.bfType, 1, 2, file2);
        fread(&fh2.bfSize, 1, 4, file2);
        fread(&fh2.bfReserved1, 1, 2, file2);
        fread(&fh2.bfReserved2, 1, 2, file2);
        fread(&fh2.bfOffBits, 1, 4, file2);

        //GET FILE INFO HEADER DATA for 2nd image
        fread(&fih2.biSize, 1, 4, file2);
        fread(&fih2.biWidth, 1, 4, file2);
        fread(&fih2.biHeight, 1, 4, file2);
        int realWidth2 = fih2.biWidth;
        int height2 = fih2.biHeight;

        fread(&fih2.biPlanes, 1, 2, file2);
        fread(&fih2.biBitCount, 1, 2, file2);
        fread(&fih2.biCompression, 1, 4, file2);
        fread(&fih2.biSizeImage, 1, 4, file2);
        fread(&fih2.biXPelsPerMeter, 1, 4, file2);
        fread(&fih2.biYPelsPerMeter, 1, 4, file2);
        fread(&fih2.biClrUsed, 1, 4, file2);

        //Malloc space for image1 and copy data in
        fseek(file1, fh.bfOffBits, 0);
        unsigned char *data = (unsigned char*)malloc(fih.biSizeImage);
        fread(data, 1, fih.biSizeImage, file1);

        //Malloc space for image2 and copy data in
        fseek(file2, fh2.bfOffBits, 0);
        unsigned char *data2 = (unsigned char*)malloc(fih.biSizeImage);
        fread(data2, 1, fih.biSizeImage, file2);

        //Loop through each pixel and put blended pixel in data
        for (int x = 0; x < realWidth; x++) {
            for (int y = 0; y < height; y++) {
                data[(realWidth * y + x) * 3 + 0] = getColor(realWidth, data, x, y, 0) * ratio + getColor(realWidth, data2, x, y, 0) * (1-ratio);
                data[(realWidth * y + x) * 3 + 1] = getColor(realWidth, data, x, y, 1) * ratio + getColor(realWidth, data2, x, y, 1) * (1-ratio);
                data[(realWidth * y + x) * 3 + 2] = getColor(realWidth, data, x, y, 2) * ratio + getColor(realWidth, data2, x, y, 2) * (1-ratio);
            }
        }

        fclose(file1);
        fclose(file2);

        //write looped data to result file
        FILE *resultFile;
        resultFile = fopen(argv[4], "wb+");
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
}


unsigned char getColor(int width, unsigned char *data, int x, int y, int color) {
    return data[(width * y + x) * 3 + color];
}
