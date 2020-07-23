// Cole Perry
// CPE 357 Lab 7
// Write a program that uses mutex with petersons Algorithm
// Feb 22 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


int main() {
    //variables
    int a;
    char *shared_arr = mmap(NULL, 1000, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1, 0); //shared memory array
    int *lock = mmap(NULL, sizeof(int) * 3, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1, 0); //shared memory array

    strcpy(shared_arr, "Temp---------");
    char text[1000];
    char text2[1000];
    strcpy(text, "Twenty years from now you will be more disappointed by the things that you didn’t do than by the ones you did do.");
    strcpy(text2, "I can’t give you a sure-fire formula for success, but I can give you a formula for failure: try to please everybody all the time.");
    lock[0]=lock[1]=0;
    lock[2] =0;//flag //maybe
    
    if (fork() == 0) { // Child
        for (int i = 0;; i++) {
            lock[1] = 1;
            lock[2] = 0;
            while (lock[0] == 1 && lock[2] == 0);
            //critical section
            if (i % 2 == 0) {
                strcpy(shared_arr, text);            } 
            else { 
                strcpy(shared_arr, text2);
            }
            lock[1] = 0;
            //end of critical section

        }
        return 0;
    }
    else { //Parent
        while(1) { 
            lock[0] = 1;
            lock[2] = 1;
            while (lock[1] == 1 && lock[2] == 1);
            //critical section
            printf("%s", shared_arr);
            printf("\n");
            //end of critical section
            lock[0] = 0;
            a--;a++;a--;
        }
        printf("Done\n");
    }
    return 0;
}

