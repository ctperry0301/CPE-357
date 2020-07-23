#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>

int fd1[2];
int fd2[2];
int flag = 0;
void overwrite() {
    flag = 1;
    dup2(fd1[0], 0); 
}

void add_null_term(char *input) {
    for(int i = 0; i < 1000; i++) {
        if(input[i]=='\n') {
            input[i+1]=0;
            break;
        }
    }
}
int main(void) {
    int *childpids = mmap(0,sizeof(int)*5,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    for(int i = 0;i < 5; i++) childpids[i] = 0;
    signal(SIGUSR1,overwrite);
    int parentpid = getpid();
    pipe(fd1);
    pipe(fd2);
    int save_stdin = dup(0);
    char inputText[1000];

        if (fork() == 0){
            int count = 0;
            while(1) {
                char temp[10000];
                childpids[0] = getpid();
                sleep(10);
                for (int i = 0; i < 10000; i++){
                    temp[i] = 0;
                }
                close(fd2[1]);
                if(childpids[4] == 1) {
                    read(fd2[0],temp, 10000 );
                    for (int i = 0; i < 10000; i++){
                        if (temp[i] == 10 || temp[i] == 32){
                            count++;
                        }
                    }
                    for (int i = 0; i < 10000; i++){
                        temp[i] =0;
                    }
                            
                    printf("\n\nYou have typed %d total words\n", count);
                    count--;
                    write(fd1[1],"\n",2);
                    kill(parentpid,SIGUSR1);
                }
                else {
                    printf("\n\nYou have typed 0 total words\n", );
                    write(fd[1],"\n",2);
                    kill(parentPid,SIGUSR1);
                }

            }
            return 0;
        }
        while(1) {
            printf("Enter Text: "); fflush(0);
            dup2(save_stdin,0);
            flag=0;
            read(0,inputText,1000);
            if(flag == 0){
                add_null_term(inputText);
            }

            if (strcmp(inputText, "q\n" ) == 0) {
                for(int i=0;i<5;i++) {
                    if(childpids[i]!=0) {
                        kill(childpids[i], SIGKILL);
                        int a3;
                        wait(&a3); //kils the kids
                    }
                }
                kill(parentpid, SIGKILL);
                return 0;
            }
            else{
                add_null_term(inputText);
                childpids[4] = 1;
                write(fd2[1], inputText, strlen(inputText));
            }
            printf("%s", inputText);
        }
        return 0;
    }