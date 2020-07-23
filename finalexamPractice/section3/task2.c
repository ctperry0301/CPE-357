#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);
    FILE* file;
    char inputBuffer[100];
    inputBuffer[0] = 0;
    int* childpid = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *childpid = 0;
    
    if(fork() == 0) {
        file = fopen("ouptut.txt", "w+");
        *childpid = getpid(); fflush(0);
        while(1 == 1) {
            sleep(5);
            char input[100];          
            write(fd[1], "\0", 1);
            int bytesRead = read(fd[0], input, 100);
            if (bytesRead != 0) {
                if(file != NULL) {
                    fwrite(input, 1, bytesRead - 1, file); fflush(file);
                }
            }  
        }
        fclose(file);
        return 0;
    }

    else {
        while (1 == 1) {
            printf("Enter Word:  ");
            scanf("%s", inputBuffer);
            write(fd[1], inputBuffer, strlen(inputBuffer));
            if(strncmp(inputBuffer, "q", 1) == 0) {
                kill(*childpid, SIGKILL);
                waitpid(*childpid, 0, WNOHANG);
                return 0;
            }
        }
    }
    
    return 0;
}