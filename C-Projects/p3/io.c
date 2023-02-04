/**
 * @file io.c
 * @author Christopher Fields (cwfields)
 *
 * Implementation of the component dealing with the
 * input/output in the terminal for the Wordle system.
 * Includes functions for reading a line from a file and
 * changing the output color when printing to standard output.
 */

#include "io.h"

#include <stdlib.h>
#include <stdio.h>

bool readLine(FILE *fp, char str[], int n)
{
    // Iterate through every character of n-length input
    for (int i = 0; i < n; i++) {
        int ch = fgetc(fp); // Store the character (or EOF) in ch
        if (ch >= 'a' && ch <= 'z') {
            str[i] = ch; // If valid character, set current index of str to ch
        } else if (ch == EOF) {
            return false;
        } else {
            fprintf(stderr, "Invalid word file\n");
            exit(EXIT_FAILURE);
        }
    }
    
    while (fgetc(fp) != '\n') {
        // Read remaining part of line
    }

    str[n] = '\0'; // Store null terminator in last index to mark the end
    return true; // There was additional input
}

void colorGreen()
{
    printf("\x1B\x5B\x33\x32\x6D");
}

void colorYellow()
{
    printf("\x1B\x5B\x33\x33\x6D");
}

void colorDefault()
{
    printf("\x1B\x5B\x30\x6D");
}
