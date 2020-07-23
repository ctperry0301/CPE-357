// Cole Perry
// CPE 357 Lab 8
// Write a program that uses a pipe
// Feb 28 2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mypipe
{
    char* pipebuffer; // Pointer to the array of pipe
    int buffersize; // Length of pipe 
    int start_occupied; // Where mywrite will write from
    int end_occupied; // Where myread will read from
    int num; 
} mypipe;

//initializes (malloc) the pipe with a size of "size" and sets start and end.
void init_pipe(mypipe* pipe, int size);

//writes "size" bytes from buffer into the pipe, returns size
int mywrite(mypipe *pipe, char* buffer, int size);

//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty
int myread(mypipe* pipe, char* buffer, int size);

int main() {    
    
    char text[100];
    mypipe pipeA;
    init_pipe(&pipeA, 32);
    mywrite(&pipeA, "hello world", 12);
    mywrite(&pipeA, "it's a nice day", 16);
    myread(&pipeA, text, 12);
    printf("%s\n", text);
    myread(&pipeA, text, 16);
    printf("%s\n", text);
    
    mywrite(&pipeA, "and now we test the carryover", 30);
    myread(&pipeA, text, 30);
    printf("%s\n", text);
    return 0;
}

//initializes (malloc) the pipe with a size of "size" and sets start and end.
void init_pipe(mypipe* pipe, int size) {
    pipe -> pipebuffer = malloc(size); //sets size of pipe
    pipe -> buffersize = size; // Length of pipe 
    pipe -> start_occupied = 0; // Where mywrite will write from
    pipe -> end_occupied = 0; // Where myread will read from
}

//writes "size" bytes from buffer into the pipe, returns size
int mywrite(mypipe *pipe, char* buffer, int size) {
    for (int i = 0; i < size - 1; i++) {
        //Check for carry over
        //if (pipe -> end_occupied == pipe -> buffersize) {
        //    pipe -> end_occupied = 0;
        //}
        pipe -> pipebuffer[pipe-> end_occupied] = buffer[i]; 
        pipe -> end_occupied += 1;
    }
    return size;
}

int myread(mypipe* pipe, char* text, int size) {
    int max_size = 0;
    for (int i = 0; i < size - 1; i++) {
        if (pipe -> start_occupied == pipe -> end_occupied) {
            text[pipe -> start_occupied] = 0;
            return max_size;
        }
        text[i] = pipe -> pipebuffer[pipe -> start_occupied];
        pipe -> start_occupied += 1;
        max_size += 1;
    }
    return size;
}