#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>

int fd[2];
void ifsignal(int i) {
    dup2(fd[0], 0);
}

int main() {
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
        int total = 1;
        char childReport[10];
        while(1 == 1) {
            sleep(5);
            sprintf(childReport, "%i", total);
            kill(parentpid, SIGUSR1);
            write(fd[1],"\n5s x ",strlen("\n5s x "));
            kill(parentpid, SIGUSR1);
            write(fd[1], childReport, strlen(childReport));
            total += 1;
            childReport[0] = 0;
        }   
        return 0; 
    }

    if(fork()== 0) {
        childpids[2] = getpid();
        int total = 1;
        char childReport[10];
        while (1 == 1) {
            sleep(10);
            sprintf(childReport, "%d", total);
            kill(parentpid,SIGUSR1);
            write(fd[1],"\n10s x ",strlen("\n10s x "));
            kill(parentpid,SIGUSR1);
            write(fd[1],childReport,strlen(childReport));
            total += 1;
            childReport[0] = 0;
        }
        return 0;
    }

    if(fork()== 0) {
        childpids[3] = getpid();
        int total = 1;
        char childReport[10];
        while (1 == 1) {
            sleep(20);
            sprintf(childReport, "%d", total);
            kill(parentpid,SIGUSR1);
            write(fd[1],"\n20s x ",strlen("\n20s x "));
            kill(parentpid,SIGUSR1);
            write(fd[1],childReport,strlen(childReport));
            total += 1;
            childReport[0] = 0;
        }
        return 0;
    }

    if(fork() == 0)
    {
        childpids[4] = getpid();
        int total = 1;
        char childReport[10];
        while(1 == 1) {
            sleep(30);
            sprintf(childReport, "%d", total);
            kill(parentpid,SIGUSR1);
            write(fd[1],"\n30s x ",strlen("\n30s x "));
            kill(parentpid,SIGUSR1);
            write(fd[1],childReport,strlen(childReport));
            total += 1;
            childReport[0] = 0;
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