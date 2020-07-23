#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MATRIX_DIMENSION_XY 10

//SEARCH FOR TODO

//************************************************************************************************************************
// sets one element of the matrix
void set_matrix_elem(float *M,int x,int y,float f){
    M[x + y * MATRIX_DIMENSION_XY] = f;
}
//************************************************************************************************************************
// lets see it both are the same
int quadratic_matrix_compare(float *A,float *B)
{
for(int a = 0; a < MATRIX_DIMENSION_XY; a++)
    for(int b = 0; b < MATRIX_DIMENSION_XY; b++)
       if(A[a+b * MATRIX_DIMENSION_XY] != B[a +b * MATRIX_DIMENSION_XY]) 
        return 0;
   
return 1;
}
//************************************************************************************************************************
//print a matrix
void quadratic_matrix_print(float *C)
{
    printf("\n");
    for(int a = 0; a < MATRIX_DIMENSION_XY; a++) {
        printf("\n");
        for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
            printf("%.2f,",C[a + b* MATRIX_DIMENSION_XY]);
        }
    printf("\n");
}
//************************************************************************************************************************
// multiply two matrices
void quadratic_matrix_multiplication(float *A,float *B,float *C)
{
//nullify the result matrix first
for(int a = 0; a < MATRIX_DIMENSION_XY; a++)
    for(int b = 0; b<MATRIX_DIMENSION_XY; b++)
        C[a + b * MATRIX_DIMENSION_XY] = 0.0;
//multiply
for(int a = 0;a<MATRIX_DIMENSION_XY;a++) // over all cols a
    for(int b = 0;b<MATRIX_DIMENSION_XY;b++) // over all rows b
        for(int c = 0;c<MATRIX_DIMENSION_XY;c++) // over all rows/cols left
            {
                C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a + c*MATRIX_DIMENSION_XY]; 
            }
    //quadratic_matrix_print(C);
}
//************************************************************************************************************************
void synch(int par_id,int par_count,int *ready)
{
int synchid = ready[par_count]+1; //synchid is progressing with each synch - I know, one from section 1 and 3 had a similar idea.
ready[par_id]=synchid;
int breakout = 0;
while(1)
    {
    breakout=1;    
    for(int i=0;i<par_count;i++) 
        {
        if(ready[i]<synchid) //"less than" is super important here. Do you know why? Because one process could run ahead and increment its ready[par_is] var already.
            {breakout = 0;break;}
        }
    if(breakout==1)
        {
        ready[par_count] = synchid; //and here we increment the additional variable
        break;
        }
    }   
}
//************************************************************************************************************************

float RNG(float min, float max) {
    float value = rand() % 9 + 1; //get float between 1 to 9 
    return min + value * (max - min);
}

int main(int argc, char *argv[])
{
srand(time(0));
int par_id = atoi(argv[2]); // the parallel ID of this process
int par_count = 1; // the amount of processes
float *A,*B,*C; //matrices A,B and C
int *ready; //needed for synch

if(argc!=3){printf("no shared\n");}
else
    {
    par_id= atoi(argv[1]);
    par_count= atoi(argv[2]);
    // strcpy(shared_mem_matrix,argv[3]);
    }
if(par_count==1){printf("only one process\n");}

int fd[4];
if(par_id==0)
    {
    //Inits the shared memory for A,B,C, ready. shm_open with C_CREAT here! then ftruncate! then mmap
    int size = 1000 * sizeof(float);
    int m1 = shm_open("matrixA", O_RDWR | O_CREAT, 0777);
    int m2 = shm_open("matrixB", O_RDWR | O_CREAT, 0777);
    int m3 = shm_open("matrixC", O_RDWR | O_CREAT, 0777);
    int synchProg = shm_open("synchProg", O_RDWR | O_CREAT, 0777);
    ftruncate(m1, size);
    ftruncate(m2, size);
    ftruncate(m3, size);
    ftruncate(synchProg, 11 * sizeof(int));

    A = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m1, 0);
    B = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m2, 0);
    C = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m3, 0);
    ready = (int*)mmap(NULL, sizeof(int) * 12, PROT_READ | PROT_WRITE, MAP_SHARED, synchProg, 0);
    }
else {
    int size = 1000 * sizeof(float);
    int m1 = shm_open("matrixA", O_RDWR, 0777);
    int m2 = shm_open("matrixB", O_RDWR, 0777);
    int m3 = shm_open("matrixC", O_RDWR, 0777);
    int synchProg = shm_open("synchProg", O_RDWR, 0777);

    A = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m1, 0);
    B = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m2, 0);
    C = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m3, 0);
    ready = (int*)mmap(NULL, sizeof(int) * 12, PROT_READ | PROT_WRITE, MAP_SHARED, synchProg, 0);
    sleep(3); //needed for initalizing synch
}

synch(par_id, par_count, ready);

if(par_id==0) {
    for(int a = 0; a < MATRIX_DIMENSION_XY; a++) {
        for(int b = 0; b < MATRIX_DIMENSION_XY; b++) {
            set_matrix_elem(A, a, b, RNG(1, 9));
            set_matrix_elem(B, a, b, RNG(1, 9));
        }
    }
}
synch(par_id,par_count,ready);


int work = MATRIX_DIMENSION_XY / par_count;
int initialRow;
int finalRow;
if (par_count == 1) {
    initialRow = 0;
    finalRow = MATRIX_DIMENSION_XY;
}
if (par_count == 2) {
    if (par_id == 0) {
        initialRow = 0;
        finalRow = 5;
    }
    else if (par_id == 1) {
        initialRow = 5;
        finalRow = 10;
    }
}
if (par_count == 5) {
    initialRow = par_id * par_count;
    finalRow = initialRow + par_count;
} if (par_count == 10) {
    initialRow = par_id * par_count;
    finalRow = initialRow + par_count;
}

for(int a = initialRow; a < finalRow; a++)
      for(int b = 0; b < MATRIX_DIMENSION_XY; b++)
          C[a + b * MATRIX_DIMENSION_XY] = 0.0;
sleep(1); //NULLIFY ROWS, sleep for initialization

//dont include final row in calculation if not at end
if (finalRow != MATRIX_DIMENSION_XY) {
    for(int x = 0; x < MATRIX_DIMENSION_XY; x++) {
        for(int y = initialRow; y < finalRow; y++) {
            for(int z = 0; z < MATRIX_DIMENSION_XY; z++) {
                C[x + y * MATRIX_DIMENSION_XY] += A[z + y * MATRIX_DIMENSION_XY] * B[x + z * MATRIX_DIMENSION_XY]; 
            }
        }
    }   
}
// FINAL ROW NEEDS TO BE INCLUDED
else if (finalRow == MATRIX_DIMENSION_XY) {
    for(int x = 0; x < MATRIX_DIMENSION_XY; x++) {
        for(int y = initialRow; y <= finalRow; y++) {
            for(int z = 0; z < MATRIX_DIMENSION_XY; z++) {
                C[x + y * MATRIX_DIMENSION_XY] += A[z + y * MATRIX_DIMENSION_XY] * B[x + z * MATRIX_DIMENSION_XY]; 
            }
        }
    }  
}

synch(par_id,par_count,ready);

if (par_id==0)
    quadratic_matrix_print(C);
synch(par_id, par_count, ready);


close (fd[0]);
close (fd[1]);
close (fd[2]);
close (fd[3]);
shm_unlink("matrixA");
shm_unlink("matrixB");
shm_unlink("matrixC");
shm_unlink("synchProg");

//lets test the result:
float M[MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY];
quadratic_matrix_multiplication(A, B, M);

if (quadratic_matrix_compare(C, M)) {
    if (par_id==0) {
        printf("\nfull points!\n");
    }
}
else
    printf("\nbuuug!\n");

return 0;    

}