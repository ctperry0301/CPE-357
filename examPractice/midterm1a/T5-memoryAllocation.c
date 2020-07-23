#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

int main() {
    printf("Enter a word, \"print\" or \"quit\" to print heap\n");
    char *wordList[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char word[100];
    for(int i = 0; i < 10; i++) {
        wordList[i] = mmap(NULL, sizeof(char) * 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    }
    while (1 == 1) {
        printf("Enter word: \n");
        scanf("%s", word);
        //END PROGRAM, free every word
        if (strcmp(word, "quit") == 0) {
            for(int i = 0; i < 10; i++) {
                munmap(wordList[i], 100);
            }
            exit(0);
        }
        // To print each word in heap 
        else if (strcmp(word, "print") == 0) {
            for (int i = 0; i < 10; i++) {
                printf("%s\n", wordList[i]);
            }
        }
        // Add or Delete word
        else {
            for(int i = 0; i < 10; i++)
            {
                if (strcmp(wordList[i], word) == 0) { //if the item is already in the list
                    wordList[i][0] = 0;
                    word[0] = '\0';  //Delete Word
                }
            }
            for(int i = 0; i < 10; i++)
            {
                if (wordList[i][0] == 0) { //if spot available
                    strcpy(wordList[i], word);
                    word[0] = '\0'; //
                    break;
                }
            }   
        }
    }
    return 0;
}