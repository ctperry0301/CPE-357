#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct chunkdata {
    char c;
    int offset;
} chunkdata;

int main() {
    FILE* file = fopen("crypt.bin", "rb");
    chunkdata *data = (chunkdata*)malloc(sizeof(chunkdata));
    fread(&data -> c, 1, sizeof(char), file);
    fread(&data -> offset, 1, sizeof(int), file);

    printf("%c", data -> c - data -> offset); // Print first letter
    
    while (data -> c != 0 && data -> offset != 0) {
        fread(&data -> c, 1, sizeof(char), file);
        fread(&data -> offset, 1, sizeof(int), file);
        char a = data -> c - data -> offset;
        printf("%c", a);
    }
    free(data);
    fclose(file);
    return 0;
}
