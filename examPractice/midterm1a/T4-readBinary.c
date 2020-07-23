#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

typedef struct datafile
{
    char type;
    int size;
    char type2;
    int offset;
    char text[10];
} datafile;

int main(){
    FILE *file;
    file = fopen("file.bin","rb");
    if (file == NULL) {
        printf("bad file\n");
    }
    datafile *data = (datafile*)malloc(sizeof(datafile));
    fread(&data -> type, 1, sizeof(char), file);
    fread(&data -> size, 1, sizeof(int), file);
    fread(&data -> type2, 1, sizeof(char), file);
    fread(&data -> offset, 1, sizeof(int), file);
    fread(&data -> text, 1, sizeof(char)*10, file);
    fclose(file);

    printf("Type: %c\n", data -> type);
    printf("Size: %i\n", data -> size);
    printf("Type2: %c\n", data -> type2);
    printf("Offset: %i\n", data -> offset);
    printf("Text: %s\n", data -> text);
    return 0;
}