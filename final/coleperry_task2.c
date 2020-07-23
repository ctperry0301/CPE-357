#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define YLW "\x1B[33m\n\0"
#define RED "\x1B[31m\n\0"

int fd[2];
void ifsignal() {
    dup2(fd[0], 0);
}

int main() {
   int *childpids = mmap(0,sizeof(int)*2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (int i = 0; i < 3; i++) {
        childpids[0] = 0;
    }
    int parentpid; 
    parentpid = getpid();
    int save_stdin;
    save_stdin = dup(0);
    char userInput[100];
    userInput[0] = 0;
    pipe(fd);
    signal(SIGUSR1, ifsignal);
    
    if(fork() == 0) {
        childpids[0] = getpid();
        sleep(10);
        kill(parentpid, SIGUSR1);
        write(fd[1], YLW, strlen(YLW));
        kill(parentpid, SIGUSR1);

        sleep(10);
        kill(parentpid, SIGUSR1);
        write(fd[1], RED, strlen(RED));
        kill(parentpid, SIGUSR1);
        
        sleep(10);
        for(int i=0;i<10;i++) {
            kill(parentpid, SIGKILL);
            kill(childpids[i], SIGKILL);
            int a3;
            wait(&a3);
        }
        kill(parentpid, SIGKILL);
        exit(0);

    }
    
    while(1==1) {
        userInput[0] = 0;
        int text;
        if(childpids[1] == 0) {
            printf("\nEnter Text: "); fflush(0);
            dup2(save_stdin, 0);
        }
        text = read(0, userInput, 100); 
        userInput[text] = 0;
        if (strcmp(userInput, "quit\n") == 0) {
            for (int i = 0; i < 3; i++) {
                kill(childpids[i], SIGKILL);
                int a3;
                wait(&a3);
            }
            kill(parentpid, SIGKILL);
            return 0;
        }
        printf("%s", userInput); fflush(0);
    }
    return 0;
}