// Cole Perry
// CPE 357 lab 5 part 3, works
// Write a program that forks in a specific order
// January Feb 7 2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
int main() {
    int fork1 = fork();
    if (fork1 == 0) {
        int fork2 = fork();
        if (fork2 == 0) {
            printf("1\n");
        }
        else {
            int i1;
            wait(&i1);
            printf("2\n");
        }
    }
    else {
        int i2;
        wait(&i2);
        int fork3 = fork();
        if (fork3 == 0) {
            printf("3\n");
        }
        else {
            int i3;
            wait(&i3);
            printf("4\n");
        }
    }
    return 0;
}