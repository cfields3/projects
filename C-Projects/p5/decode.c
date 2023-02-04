/**
 * @file decode.c
 * @author Christopher Fields (cwfields)
 * 
 * Component of the Base64 encoding system responsible for
 * decoding inputed files from Base64. It will input files from
 * command-line arguments, reading input from the text input
 * file, converting the encoding characters to original binary
 * and printing the output to an output binary file.
 */

#include "state24.h"
#include "filebuffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>

/** Number of arguments necessary in executing the decode program */
#define NUM_ARGS 3
/** Command-line argument of the input filename */
#define ARG_INPUT 1
/** Command-line argument of the output filename */
#define ARG_OUTPUT 2

/**
 * The start of the execution of the decode program. Will input
 * input/output files as command-line arguments from the user. Reads the
 * text input file in the specified location and outputs text to the
 * output file whose location is also specified. Will exit and print error
 * messages to standard error if there is an issue with the files inputed.
 * 
 * @param argc the number of command-line arguments
 * @param argv list of command-line arguments
 * @return program exit status
 */
int main (int argc, char *argv[])
{
    // Invalid command-line arguments
    if (argc < NUM_ARGS) {
        fprintf(stderr, "usage: decode <input-file> <output-file>\n");
        return EXIT_FAILURE;
    }

    char *inputFilename = argv[ARG_INPUT];
    char *outputFilename = argv[ARG_OUTPUT];

    // Open a file stream for input, check if invalid with errno
    errno = 0;
    FILE *inputStream = fopen(inputFilename, "r");
    if (!inputStream) {
        perror(inputFilename);
        return EXIT_FAILURE;
    }

    FileBuffer *fileBuffer = makeFileBuffer();

    // Initialize a new State24 to hold 24 bits
    State24 state;
    initState(&state);

    byte buffer[STATE_CAPACITY];

    // Initialize character to first character in input
    char currentChar = fgetc(inputStream);

    // Continue reading characters from input until end-of-file or '=' is read
    while (currentChar != EOF && currentChar != '=') {
        if (!isspace(currentChar)) {
            if (state.bitCount / BITS_IN_CHAR >= STATE_CHARS) {
                getBytes(&state, buffer);
                for (int j = 0; j < STATE_CAPACITY; j++) {
                    appendFileBuffer(fileBuffer, buffer[j]);
                }
            }

            if (!validChar(currentChar)) {
                fprintf(stderr, "Invalid input file\n");
                freeFileBuffer(fileBuffer);
                fclose(inputStream);
                return EXIT_FAILURE;
            }

            addChar(&state, currentChar);
        }
        
        currentChar = fgetc(inputStream);
    }

    // Ensure that there are not any encoding characters after the '='
    while (currentChar != EOF && !isspace(currentChar)) {
        if (currentChar != '=') {
            fprintf(stderr, "Invalid input file\n");
            freeFileBuffer(fileBuffer);
            fclose(inputStream);
            return EXIT_FAILURE;
        }

        currentChar = fgetc(inputStream);
    }

    int numBytes = getBytes(&state, buffer);

    // Add the remaining bytes to the fileBuffer
    for (int i = 0; i < numBytes; i++) {
        appendFileBuffer(fileBuffer, buffer[i]);
    }

    // Print contents of the fileBuffer to the output file
    saveFileBuffer(fileBuffer, outputFilename);

    // Free dynamically allocated memory & file streams
    freeFileBuffer(fileBuffer);
    fclose(inputStream); 
}
