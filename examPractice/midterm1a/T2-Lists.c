#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

typedef struct listelement {
    struct listelement *next,*prev;
    int number;
} listelement;

// Add global head
struct listelement *head = NULL;

void new_element(int num) {
    /*
    int num;
    printf("Enter an integer: ");
    scanf("%i", &num); 
    printf("\n");
    */
    struct listelement *new_node = (struct listelement*)malloc(sizeof(struct listelement));
    if (head == NULL) {
        //if list empty, create head
        head = new_node;
        new_node -> prev = NULL;
        new_node -> next = NULL;
        new_node -> number = num;
    }
    else {
        //first element already exists, add to front
        new_node -> prev = NULL;
        new_node -> next = head;
        new_node -> number = num;

        head -> prev = new_node; 
        head = new_node;
    }
}

void printlist(){
    struct listelement *temp;
    if (head == NULL) {  //if list empty
        printf("List is Empty\n\n\n");
    }
    else {
        printf("----------Printing List----------\n");
        for (temp = head; temp != NULL; temp = temp -> next){
            printf("number: %d\n", temp -> number);
            printf("prev: %p\n", temp -> prev);
            printf("next: %p\n", temp -> next);
            printf("\n");
        }
        printf("----------List Printed----------\n\n\n");
    }
}

void remove_element(int d) {
    struct listelement *temp;
    for (temp = head; temp != NULL; temp = temp -> next) {
        if (temp -> number == d) { 
            if (temp -> prev == NULL){
                head = temp -> next;
            }
            if (temp -> prev != NULL){
                temp -> prev -> next = temp -> next;
            }
            if (temp -> next != NULL) {
                temp -> next -> prev = temp -> prev;
            }
            free(temp);
        }
    }
}

void delete_list(){
    listelement *temp;
    listelement *current = head;
    while (current != NULL)  
    {  
        temp = current -> next;  
        free(current);  
        current = temp;  
    }  
    free(current);
    head = NULL;
}

int main(){    
    new_element(1);
    new_element(2);
    new_element(3);
    new_element(1);
    printlist();

    remove_element(3);
    printlist();
    
    delete_list();
    printlist();
    return 0;
}