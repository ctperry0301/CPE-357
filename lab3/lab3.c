// Cole Perry
// CPE 357 Lab 3
// Write a program that allocates and deallocates specific locations in memory
// January 24 2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PAGESIZE 1024 
#define ONEMB 1048576

void analyze();
void myfree(unsigned char *);
unsigned char *mymalloc(unsigned int size);

typedef struct chunkhead {
    unsigned int size;
    unsigned int info;
    unsigned char *next;
    unsigned char *prev;
} chunkhead;

unsigned char myheap[ONEMB];

int main() {
    struct chunkhead *ch = (chunkhead*) myheap;
    ch -> size = ONEMB - sizeof(ch);
    ch -> info = 0;
    ch -> next = 0;
    ch -> prev = 0;

    unsigned char *a,*b,*c,*d;
    a = mymalloc(1000);
    b = mymalloc(1024);
    c = mymalloc(1000);
    d = mymalloc(3000);
    analyze();
    
    //free(b);
    //free(a);
    //analyze();
    //return 0;

}

//Analyze function is very similar to this function, except it will just print out the data
unsigned char *mymalloc(unsigned int size) {
    struct chunkhead *ch = (chunkhead*) myheap;
    int IsSpaceAvailable = 0;
    for (;ch != NULL; ch = (chunkhead *) ch -> next) {
        if ((size == ch -> size) && (ch -> info == 0)) {
            ch->info = 1;
            IsSpaceAvailable = 1;
            return (unsigned char*)(ch+1);
        }
        else if ((size < ch -> size) && (ch -> info == 0)) {
            ch -> size = size;
            ch -> info = 1;
            struct chunkhead *newch = (chunkhead*) (ch + sizeof(ch) + size);
            ch -> next = (unsigned char*)newch; 
            newch -> size = size; //ONEMB - size - sizeof(ch)
            newch -> info = 0;
            newch -> prev = (unsigned char*) ch; 
            IsSpaceAvailable = 1;
            return (unsigned char*)(newch+1);
        }
    }
    if(IsSpaceAvailable == 0) {
        return NULL;
    }
    return NULL;
}
/*
    for (;ch != NULL; ch = (chunkhead *) ch -> next) {
        //int sizeNeeded = size + ((PAGESIZE-(size % PAGESIZE)) % PAGESIZE); //For specific page size
        if ((ch -> info == 0) && (size <= ch -> size)) {
            if (size == ch -> size) {
                ch->info = 1;
                return (unsigned char*)(ch+1);
            }
            else {
                ch -> size = size;
                ch -> info = 1;
                struct chunkhead *newch = (chunkhead*) (ch + sizeof(ch) + size);
                ch -> next = (unsigned char*)newch; 
                newch -> size = size;
                newch -> info = 0;
                newch -> prev = (unsigned char*) ch; 
                IsSpaceAvailable = 1;
                return (unsigned char*)(newch+1);
            }
        }
    }
    if (IsSpaceAvailable == 0) {
        return(NULL);
    }
}
*/

void analyze() {
    struct chunkhead *ch = (chunkhead*) myheap;
    int i = 1;
    for (;ch != NULL; ch = (chunkhead *) ch -> next) {
        printf("Chunk #%i:\n", i);
        printf("Size = %i bytes\n", ch -> size);
        if (ch -> info == 1){
           printf("Occupied\n"); 
        } 
        else {
            printf("Free\n");
        }
        printf("Next = %p\n", ch -> next);
        printf("Prev = %p\n", ch -> prev);
        i++;
    }
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

