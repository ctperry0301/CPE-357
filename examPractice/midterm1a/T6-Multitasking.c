#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>


int main() {
    int *arr = (int*)mmap(NULL, 1000*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int fork1 = fork();
    int fork2 = fork();
    int fork3 = fork();

    if (fork1 == 0) {
        if (fork2 == 0) {
            for (int i = 0; i <= 249; i++) {
                arr[i] = i;
            }
        }
        else {
            for (int i = 250; i <= 499; i++) {
                arr[i] = i;
            }
        }
    }
    else {
        if (fork3 == 0) {
            for (int i = 500; i <= 749; i++) {
                arr[i] = i;
            }
        }
        else {
            for (int i = 750; i <= 999; i++) {
                arr[i] = i;
            }
        }
    }
    if (fork1 == 0) {
        return 0;
    }
    else {
        int a;
        wait(&a);
    }
    if (fork2 == 0) {
        return 0;
    }
    else {
        int b;
        wait(&b);
    }
    if (fork3 == 0) {
        return 0;
    }
    else {
        int c;
        wait(&c);
    }

    for(int i=0;i<1000;i++){
        /*if (i%10==0){
            //an attempt to make the outcome somewhat readable
            printf("\n");
        }*/
        printf("%d ", arr[i]);
    }

    printf("\n\n");
    munmap(arr, 1000*sizeof(int));
    return 0;
}