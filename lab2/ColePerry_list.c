// Cole Perry
// CPE 357 Assignment 2
// Write a program that uses a doubly linked list 
// January 17 2020
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
struct listelement
{
    struct listelement *next;
    struct listelement *prev;
    char text[1000];
};
//and this global list head:
struct listelement *head = NULL;

void pushString();
void printList();
void deleteItem();
void deleteList();

int main() {
    printf("1: Push String\n");
    printf("2: Print List\n");
    printf("3: Delete Item\n");
    printf("4: End Program \n"); 
    int selection;
    scanf("%d", &selection);
    printf("\n");
    if (selection == 1) {
        pushString();
    }
    else if (selection == 2) {
        printList();
        
    }
    else if (selection == 3) {
        deleteItem();
    }
    else if (selection == 4) {
        deleteList();
    }
    else {
        main();
    }
}

void pushString() {
    //Reads a string then makes list item then add it to the end off the list 
    //char word[1000] = {"Hello"}; 
    char word[1000]; //
    printf("Enter string: ");
    scanf("%s", &word[0]); //
    printf("\n");

    struct listelement* new_node = (struct listelement*)malloc(sizeof(struct listelement)); 
    new_node -> prev = new_node -> next = 0;
    if (head == NULL) { 
        //add first element
        head = new_node;
        strncpy(head -> text, word, 1000);
        printf("---String added---\n\n");
        main();
    }
    else{
        //Go to end of list, then add element
        struct listelement *temp;
        for (temp = head; temp -> next != 0; temp = temp -> next);
        temp -> next = new_node;
        new_node -> prev = temp;
        strncpy(new_node -> text, word, 1000);
        printf("---String added---\n\n");
        main();
    }
}
void printList() {
    //prints the list on the screen. Each item one row. 
    struct listelement *temp;
    temp = head;
    while (temp != NULL) { 
        printf("%s \n", temp -> text);
        temp = temp -> next;
    } 
    printf("\n---String Printed---\n\n");
    main();
}
void deleteItem() {
    //Reads a number and deletes corrospoding list item (first item is 1)
    //If item doesn't exist, i can handle how I want
    int item;
    struct listelement *temp;
    temp = head;
    //If item to be deleted DNE or front or end
    if (temp == NULL) {
        printf("---You have nothing to delete---\n\n");
        main();
    }

    int length = 0;
    struct listelement *temp2;
    temp2 = head;

    //Calculate length of linked list
    while (temp2 != NULL)  
    {  
        length+=1;  
        temp2 = temp2 -> next;  
    }  
    printf("Enter an int > 0: ");
    scanf("%i", &item);
    printf("\n");
    if (item > length) {
        printf("---You have %d items in your list---\n\n", length);
        main();
    }

    // Delete First
    else if (item-1 == 0 ) 
    { 
        head = temp -> next;  
        free(temp);
        printf("---Item Deleted---\n\n");
        main();
    } 

    // Delete Mid or Tail
    else{
        for (int i=0; temp != NULL && i < item-2; i++) 
        {
            temp = temp -> next; 
        }
        struct listelement *next = temp -> next -> next; 
        free(temp -> next);  
        temp -> next = next;
        printf("---Item Deleted---\n\n");  
        main();
    }
}

void deleteList() {
    struct listelement* current = head;  
    struct listelement* next;  
    while (current != NULL)  
    {  
        next = current -> next;  
        free(current);  
        current = next;  
    }  
    head = NULL; 
    free(head);
    exit(0);
}