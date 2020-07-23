#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/dirent.h>
#include <dirent.h> 
#include <sys/stat.h>

#define S_IFMT 0170000
#define S_IFDIR 0040000
#define S_IFREG 0100000

int main() {
    char cwd[200];
    char input[200];
    printf("\nYour Absolute Location is: %s ", getcwd(cwd, sizeof(cwd))); fflush(0);
    printf("\n\n");
    DIR *fileName;
    struct dirent *dir;
    fileName = opendir(".");
    if (fileName) {
        while ((dir = readdir(fileName)) != NULL) {
            printf("%s ", dir->d_name);
            struct stat sb;
            stat(dir->d_name, &sb);
            printf("File type is: ");
            switch (sb.st_mode & S_IFMT) {
                case S_IFREG:  printf("is a File\n"); break;
                case S_IFDIR:  printf("is a Folder\n"); break;
            }
        }
        closedir(fileName);
    }
    printf("\n");
}