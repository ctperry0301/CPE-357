#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>

#define KNRM "\x1B[0m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"

int fd[2];
void ifsignal() {
    dup2(fd[0], 0);
}

int main() {
   int *childpids = mmap(0,sizeof(int)*3, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (int i = 0; i < 3; i++) {
        childpids[i] = 0;
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
        while(1 == 1)
        {
            sleep(5);
            childpids[2] = 1;
            kill(parentpid, SIGUSR1);
            write(fd[1], KCYN, strlen(KCYN));
            kill(parentpid, SIGUSR1);
            time_t currentTime;
            time(&currentTime);
            write(fd[1], ctime(&currentTime), strlen(ctime(&currentTime)));
            kill(parentpid, SIGUSR1);
            childpids[2] = 0;
            write(fd[1], KNRM, strlen(KNRM));
        }
        return 0; 
    }

    if (fork() == 0) {
        childpids[1] = getpid();
        while(1 == 1) {
            sleep(11);
            childpids[2] = 1;
            write(fd[1], KMAG, strlen(KMAG));
            kill(parentpid, SIGUSR1);
            time_t currentTime;
            time(&currentTime);
            write(fd[1], ctime(&currentTime), strlen(ctime(&currentTime)));
            kill(parentpid, SIGUSR1);
            childpids[2] = 0;
            write(fd[1], KNRM, strlen(KNRM));
        }
        return 0;
    }
    
    while(1 == 1) {
        userInput[0] = 0;
        int text;
        if(childpids[2] == 0) {
            printf("\nEnter Text: "); fflush(0);
            dup2(save_stdin, 0);
        }
        text = read(0, userInput, 100); 
        userInput[text] = 0;
        if (strcmp(userInput, "q\n") == 0) {
            for (int i = 0; i < 5; i++) {
                kill(childpids[i], SIGKILL);
                int a3;
                wait(&a3); //kils the kids
            }
            kill(parentpid, SIGKILL);
            return 0;
        }
        printf("%s", userInput); fflush(0);
    }
    return 0;
}