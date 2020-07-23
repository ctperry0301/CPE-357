#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()  {
    int fd[2]; pipe(fd);

    write(fd[1],"helloworld",6);   close(fd[0]);close(fd[1]);

    char text[100];

    close(fd[1]);

    dup2(fd[0],STDIN_FILENO);

    close(fd[0]);

    scanf("%s",text);

    printf("%s\n",text);

//What will be printed?
}