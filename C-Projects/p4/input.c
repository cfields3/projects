/**
 * @file input.c
 * @author Christopher Fields (cwfields)
 * 
 * Implementation of the input component in the Agency Database Management
 * system. Includes a single function, readLine, which reads a line of input
 * and returns a pointer to a string with that input as dynamically allocated
 * memory.
 */

#include "input.h"

#include <stdlib.h>
#include <string.h>

/** Multiple for resizing the line string size when at capacity */
#define RESIZE_MULTIPLE 2

char *readLine(FILE *fp)
{
    // Allocate an array of characters (string) to store the line contents
    char *line = (char *) malloc(sizeof(char));
    int size = 0;
    int capacity = 1;

    // Read the first character of input
    int nextChar = fgetc(fp);

    if (nextChar == EOF) { // No more input to read
        free(line); // Free memory! (Not returning a reference to it so caller cannot)
        return NULL;
    }

    // Read each character until EOF or newline is reached, dynamically resizing the line
    while (nextChar != EOF && nextChar != '\n') {
        if (size >= capacity) {
            capacity *= RESIZE_MULTIPLE; // Increase capacity by specified factor
            line = (char *) realloc(line, capacity * sizeof(char *)); // Reallocate to reflect capacity increase
        }
        line[size++] = nextChar; // Add the character to the line string
        nextChar = fgetc(fp); // Read the next character of input
    }

    // Reallocate the line to be more space efficient (only big enough to store line contents)
    line = (char *) realloc(line, size + 1);
    line[size] = '\0';

    return line;
}