#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    int fd = shm_open("write", O_RDWR | O_CREAT, 0777);
    ftruncate(fd, sizeof(char)*100);
    char *sharedMem = mmap(0, sizeof(char)*1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    sharedMem[0] = 0;
    char input[100];
    int *childPID = mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    
    while(1==1) {
        printf("Enter Text: ");
        scanf("%s", input);
        if (strcmp(input, "q") == 0) {
            strcpy(sharedMem, "quitting");
            break;
        }
        if (sharedMem[0] == 0) {
            strcpy(sharedMem, input);
        }
        else {
            if ((strcmp(input, "time") == 0)) {
                strcpy(sharedMem, "time");
            }
            else {
                strcat(sharedMem, input);
            }
        }
        printf("Full Input: %s\n", sharedMem);
    }
    kill(*childPID, SIGKILL);
    wait(0);
    close (fd);
    sharedMem[0] = 0;
    shm_unlink("write");
    //munmap(2);
    return 0;

}