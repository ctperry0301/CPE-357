// Cole Perry
// CPE 357 Assignment 1
// Write a program that draws a x-mas tree out of asterix and space signs using loops
// January 8 2020

#include <stdio.h>
#include <string.h>

int main() {
    // Vars
    int lines;
    int currentLine = 0;
    int lineLength;
    printf("How many lines would you like to print: ");
    scanf("%d", &lines);
    printf("\n");
    lineLength = lines * 2 + 1;

    while (currentLine < lines) {
        int stars = currentLine * 2 + 1;
        int spaces = lineLength - stars;
        for (int i = 0; i < spaces / 2; i++) { // print spaces before stars
            printf(" ");
        }
        for (int i = 0; i < stars; i++) { // print stars
            printf("*");
        }
        for (int i = 0; i < spaces / 2; i++) { // print spaces after stars
            printf(" ");
        }
        printf("\n"); 
        currentLine += 1;
    }

    ///Print the stem
    for (int i; i < 3; i++) {
        int space = 0;
        while (space < lineLength) { 
            if (space == lineLength/2) {
                printf("*");
            }
            else {
                printf(" ");
            }
            space += 1;
        }
        printf("\n");
    }
}