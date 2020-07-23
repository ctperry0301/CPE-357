// Cole Perry
// CPE 357 Project 1
// Write a program that blends 2 given bmp images
// January 26 2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>//for using mmap

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
    clock_t start, end;
    double cpu_time_used;
    int use_fork; //for determining whether or not to run profram with fork
    if (argc != 5) {
        printf("1: Command Line should be in the form [programname] [imagefile1] [brightness] [Parallel] [outputfile]\n");
        return 0;
    }
    if (atoi(argv[3]) == 0) {
        use_fork = 0;
        printf("No fork needed\n");
    }
    else{
        use_fork = 1;
        printf("create a fork\n");
    }

    //ARGV variiables
    FILE *file1;
    float brightnessRatio = atof(argv[2]);
    int brightness = brightnessRatio * 255;
    file1 = fopen(argv[1], "rb");

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

    //Malloc space for image1 and copy data in
    fseek(file1, fh.bfOffBits, 0);
    unsigned char *data = (unsigned char*)mmap(NULL, fih.biSizeImage, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    fread(data, 1, fih.biSizeImage, file1);

    start = clock();
    if (use_fork == 1) { 
        if (fork() == 0) { //first half
            //do work for lower half of pixels
            for (int x = 0; x < realWidth; x++) {
                for (int y = 0; y < height/2; y++) {
                    int location = (realWidth * y + x) * 3;
                    if ((getColor(realWidth, data, x, y, 0) + brightness) >= 255) {
                        data[location + 0] = 255;
                    }
                    else{
                        data[location + 0] = getColor(realWidth, data, x, y, 0) + brightness;
                    }
                    if ((getColor(realWidth, data, x, y, 1) + brightness) >= 255) {
                        data[location + 1] = 255;
                    }
                    else{
                        data[location + 1] = getColor(realWidth, data, x, y, 1) + brightness;
                    }            
                    if ((getColor(realWidth, data, x, y, 2) + brightness) >= 255) {
                        data[location + 2] = 255;
                    }
                    else {
                        data[location + 2] = getColor(realWidth, data, x, y, 2) + brightness;
                    }
                }
            }
            return 0;
        }
        else {  //second half
            //do work for upper half of pixels
            for (int x = 0; x < realWidth; x++) {
                for (int y = height/2; y < height; y++) {
                    int location = (realWidth * y + x) * 3;
                    if ((getColor(realWidth, data, x, y, 0) + brightness) >= 255) {
                        data[location + 0] = 255;
                    }
                    else{
                        data[location + 0] = getColor(realWidth, data, x, y, 0) + brightness;
                    }
                    if ((getColor(realWidth, data, x, y, 1) + brightness) >= 255) {
                        data[location + 1] = 255;
                    }
                    else{
                        data[location + 1] = getColor(realWidth, data, x, y, 1) + brightness;
                    }            
                    if ((getColor(realWidth, data, x, y, 2) + brightness) >= 255) {
                        data[location + 2] = 255;
                    }
                    else {
                        data[location + 2] = getColor(realWidth, data, x, y, 2) + brightness;
                    }
                }
            }
            int i;
            wait(&i);
        } 
    }
    else {
        //to not use fork
        //Loop through each pixel and put blended pixel in data
        for (int x = 0; x < realWidth; x++) {
            for (int y = 0; y < height; y++) {
                int location = (realWidth * y + x) * 3;
                if ((getColor(realWidth, data, x, y, 0) + brightness) >= 255) {
                    data[location + 0] = 255;
                }
                else{
                    data[location + 0] = getColor(realWidth, data, x, y, 0) + brightness;
                }
                if ((getColor(realWidth, data, x, y, 1) + brightness) >= 255) {
                    data[location + 1] = 255;
                }
                else{
                    data[location + 1] = getColor(realWidth, data, x, y, 1) + brightness;
                }            
                if ((getColor(realWidth, data, x, y, 2) + brightness) >= 255) {
                    data[location + 2] = 255;
                }
                else {
                    data[location + 2] = getColor(realWidth, data, x, y, 2) + brightness;
                }
            }
        }
    }
    end = clock();

    cpu_time_used = ((double) (end - start)); //in microseconds
    printf("Time used: %f\n", cpu_time_used);

    fclose(file1);
    //write looped data to result file
    FILE *resultFile;
    resultFile = fopen("a.bmp", "wb+");
    fwrite(&fh.bfType, 2, sizeof(unsigned char), resultFile);
    fwrite(&fh.bfSize, 1, 4, resultFile);
    fwrite(&fh.bfReserved1, 1, 2, resultFile);
    fwrite(&fh.bfReserved2, 1, 2, resultFile);
    fwrite(&fh.bfOffBits, 1, 4, resultFile);
    fwrite(&fih, 1, sizeof(struct tagBITMAPINFOHEADER), resultFile);
    fwrite(data, fih.biSizeImage, 1, resultFile);
    fclose(resultFile);

    munmap(data, fih.biSizeImage); //clear allocated memory
    return 0;
}


unsigned char getColor(int width, unsigned char *data, int x, int y, int color) {
    return data[(width * y + x) * 3 + color];
}
