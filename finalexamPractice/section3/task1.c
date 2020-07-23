#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>

#define KRED "\x1B[31m" 
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

int fd[2];
void ifsignal() {
    dup2(fd[0],STDIN_FILENO);
}

int main()
{
    int *childpids = mmap(0,sizeof(int)*5, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (int i=0; i<5; i++) {
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
    if(fork()== 0) {
        childpids[0] = getpid();
        while(1 == 1) {
            sleep(4);
            char tempor[100];
            kill(parentpid,SIGUSR1);
            int written = write(fd[1],KCYN,strlen(KCYN));
        }
        return 0; 
    }

    if(fork()== 0) {
        childpids[1] = getpid();
        while(1 == 1)
        {
            sleep(5);
            kill(parentpid,SIGUSR1);
            write(fd[1],KMAG,strlen(KMAG));
        }
        return 0;
    }

    if(fork()== 0) {
        childpids[2] = getpid();
        while (1 == 1) {
            sleep(6);
            kill(parentpid,SIGUSR1);
            write(fd[1],KRED,strlen(KRED));
        }
        return 0;
    }

    if(fork()== 0) {
        childpids[3] = getpid();
        while(1 == 1) {
            sleep(7);
            kill(parentpid, SIGUSR1);
            write(fd[1], KMAG, strlen(KMAG));
        }
        return 0;
    }
    while(1 == 1) {
        userInput[0] = 0;
        printf("\nEnter Text: "); fflush(0);
        dup2(save_stdin, 0);
        int text;
        text = read(0, userInput, 50); 
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