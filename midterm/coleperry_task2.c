#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    //malloc size of array
    char *arr = malloc(sizeof(char) * 1000);
    int fork1 = fork();
    if (fork1 == 0) {
        int fork2 = fork();
        if (fork2 == 0) {
            //add firstname
            strcat(arr,"Cole ");
        }
        else {
            int a;
            wait(&a);
            strcat(arr,"Thomas ");
            //add middlename
        }
    }
    else {
        int b;
        wait(&b);
        int fork3 = fork();
        if (fork3 == 0) {
            int c;
            wait(&c);
            strcat(arr,"Kansas City ");            
            //add fav city
        }
        else {
            //add lastname
            strcat(arr,"Perry ");
        }
    }
    //print char array
    for (int i = 0; i < strlen(arr); i++) {
        printf("%c", arr[i]);
    }
    free(arr);
}