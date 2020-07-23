#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
    int i;
    if(fork() == 0)
    {        
        if(fork() == 0)
        {
            printf("%s", "A");
            return 0;
        }
        else
        {
            wait(&i);
            printf("%s", "B");
            return 0;
        }

    }
    else
    {
        wait(&i);
        fflush(0);
        if(fork() == 0)
        {
            printf("%s", "C");
            return 0;
        }
        else
        {
            wait(&i);
            printf("%s", "D");
        }
    }
return 0;
}