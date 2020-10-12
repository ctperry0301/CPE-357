#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    for (int i = 0; i < atoi(argv[3]); i++) {
        char *args[4];
        args[0] = malloc(sizeof(char)*100);
        args[1] = malloc(sizeof(int));
        args[2] = malloc(sizeof(int));
        strcpy(args[0], argv[1]);
        sprintf(args[1], "%d", i);
        sprintf(args[2], "%d", atoi(argv[3]));
        args[3] = 0;
        //printf("args[0]: %s \t args[1]: %d \t args[2]: %d\n", args[0], atoi(args[1]), atoi(args[2]));
        if (fork() == 0) {
            execv(args[0],args);
        }
        free(args[0]);
        free(args[1]);
        free(args[2]);
    }
    int l;
    wait(&l);
    gettimeofday(&stop, NULL);
    printf("Time taken for multiplication:  %f seconds\n", ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000.0000) ;
    return 0;
}