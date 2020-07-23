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
#include <time.h>

int main() {
    int fd = shm_open("write", O_RDWR, 0777);
    char *sharedMem = mmap(0, sizeof(char)*1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    while(1==1) {
        sleep(2);
        if (sharedMem[0] == 0) {
            printf("No shared Mem \n");
        }
        else {
            if (strncmp(sharedMem, "quitting", 8) == 0) {
                break;
            }
            else if (strncmp(sharedMem, "time", 4) == 0) {
                time_t localTime;
                time(&localTime);
                printf("%s", ctime(&localTime));
                sharedMem[0] = 0;
            }
            else {
                printf("Shared mem is %s\n", sharedMem);
            }
        }
    }
    close(fd);
    sharedMem[0] = 0;
    shm_unlink("write");
}