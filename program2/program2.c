// Cole Perry
// CPE 357 Program 2
// Write a program that allocates and deallocates specific locations in memory
// Feb 6 2020
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#define PAGESIZE 4096 
#define ONEMB 1048576

void analyze();
void myfree(unsigned char *);
void *startofheap = NULL;
unsigned char *mymalloc(unsigned int size);
//int chunkCount = 0;
typedef unsigned char byte;

typedef struct chunkhead {
    unsigned int size;
    unsigned int info;
    unsigned char *next;
    unsigned char *prev;
} chunkhead;

int a[ONEMB];

int main() {
    struct chunkhead *ch = (chunkhead*) a;
    ch -> size = ONEMB - sizeof(ch);
    ch -> info = 0;
    ch -> next = 0;
    ch -> prev = 0;
    
    byte*a[100];
    analyze();//50% points
    
    for(int i=0;i<100;i++)
        a[i]= mymalloc(1000);
    for(int i=0;i<90;i++)
        myfree(a[i]);
        analyze(); //50% of points if this is correct
        myfree(a[95]);
        a[95] = mymalloc(1000);
        analyze();//25% points, this new chunk should fill the smaller free one
    //(best fit)
    for(int i=90;i<100;i++)
        myfree(a[i]);
        analyze();// 25% should be an empty heap now with the start address
    //from the program start    
}

//Analyze function is very similar to this function, except it will just print out the data
unsigned char *mymalloc(unsigned int size) {
    struct chunkhead *ch = (chunkhead*) a;
    int IsSpaceAvailable = 0;
    startofheap = 1;
    for (int i = 0; ch != NULL; ch = (chunkhead *) ch -> next) {
        //int sizeNeeded = size + ((PAGESIZE-(size % PAGESIZE)) % PAGESIZE); //For specific page size
        if ((ch -> info == 0) && (size <= ch -> size)) { //&& (ch -> next != NULL)
            struct chunkhead *newch = (chunkhead*) a;
            ch -> size = size;
            ch -> info = 1;
            //ch -> next = (unsigned char*) newch;
            ch -> next = (unsigned char*) sizeof(ch) + size;
            newch -> size = size;
            newch -> info = 0;
            newch -> prev = (unsigned char*) ch; 
            IsSpaceAvailable = 1;
            return (unsigned char*)(ch+1);
        }
    }

    if (IsSpaceAvailable == 0) {
        return(NULL);
    }
    else {
        return 0; // will never get here
    }
}

void analyze() {
    printf("\n--------------------------------------------------------------\n");
    if(!startofheap)
    {
        printf("no heap");
        return;
    }
    
    chunkhead* ch = (chunkhead*) startofheap;
    for (int no=0; ch; ch = (chunkhead*)ch->next,no++)
    {
        printf("%d | current addr: %x |", no, ch);
        printf("size: %d | ", ch->size);
        printf("info: %d | ", ch->info);
        printf("next: %x | ", ch->next);
        printf("prev: %x", ch->prev);
        printf(" \n");
    }
    //printf("program break on address: %x\n", sbrk(0));
    
}


void myfree(unsigned char *address) {
    struct chunkhead *ch = (chunkhead*)(address-sizeof(chunkhead));
    ch -> info = 0;
    //4 cases.
    // #1, no merge //Just 1 Cell
    // #2, merge with next //add size + size of chunkhead + size of next chunkhead
    // #3, merge with prev
    // #4, merge with next and prev
}

