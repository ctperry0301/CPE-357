#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(){
   int fork1 = fork();
   if (fork1 == 0) {
       int fork2 = fork();
       if (fork2 == 0) {
           printf("1\n");
       }
       else {
           int i1;
           wait(&i1);
           int fork3 = fork();
           if (fork3 == 0) {
               printf("2\n");
           }
           else {
               int i2;
               wait(&i2);
               printf("3\n");
           }
       }
   }
   else {
       int i3;
       wait(&i3);
       int fork4 = fork();
       if (fork4 == 0) {
           printf("4\n");
       }
       else {
           int i4;
            wait(&i4);
            printf("5\n");
       }
   }
   return 0;
}