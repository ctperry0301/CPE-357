// Cole Perry
// CPE 357 Lab 6
// Write a nearly indestructible program that 
//creates a new child everytime child process is ended
// Feb 14 2020

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h> 

void ifsignal(int i) {
    printf("yes\n");
}   

int main() {  
    //signal(SIGINT, ifsignal);
    signal(SIGTERM, ifsignal);
    //signal(SIGSTP, ifsignal);
    //signal(SIGKILL, ifsignal);
 
    printf("original: %d\n", getpid());
    while(1) {
        if (fork() == 0) {
            printf("child: %d\n", getpid());
            while(1) {
                DIR *fileName;
                struct dirent *dir;
                fileName = opendir(".");
                if (fileName) {
                    while ((dir = readdir(fileName)) != NULL) {
                    printf("%s\n", dir->d_name);
                    }
                    closedir(fileName);
                }
                time_t T = time(NULL);
                struct tm tm = *localtime(&T); 
                printf("%d:%d\n\n",tm.tm_hour, tm.tm_min);
                sleep(10);
            
            }
            return 0;
        }
        else {
            printf("parent: %d\n", getpid());
            wait(NULL);
            fflush(NULL);
        }
    }
    return 0;
    exit(0);
}
