/**
 * @file encode.c
 * @author Christopher Fields (cwfields)
 * 
 * Component of the Base64 encoding system responsible for
 * encoding inputed files to Base64. It will input files from
 * command-line arguments, reading input from the binary input
 * file, converting the binary to readable encoding characters,
 * and printing the output to an output text file. The system also
 * allows the inclusion of flags that will allow the user to choose
 * to not print line breaks or not print padding characters.
 */

#include "state24.h"
#include "filebuffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

/** Minimum number of arguments that make up a valid command */
#define MIN_ARGS 3
/** Command-line argument of the input filename */
#define ARG_INPUT argc - 2
/** Command-line argument of the output filename */
#define ARG_OUTPUT argc - 1
/** The maximum characters printed on a line (with line breaks enabled) */
#define LINE_MAX 76
/** Number of leftover characters necessary for padding one '=' */
#define SINGLE_PADDING 2
/** Number of leftover characters necessary for padding two '=' */
#define DOUBLE_PADDING 3

/**
 * Helper function used in printing characters to a given stream based on the number
 * of characters already printed to that stream. In most cases, it will simply print
 * the character and increment the number of characters printed. However, if the number
 * of characters printed exceeds the maximum, it will also print a newline ('\n').
 * 
 * @param ch character to print to outputStream
 * @param outputStream stream to print ch to
 * @param numPrinted pointer to an integer representing the number of characters already printed
 * @param printBreaks boolean flag indicating whether to print spaces
 */
static void outputCharacter(char ch, FILE *outputStream, int *numPrinted, bool printBreaks)
{
    if (*numPrinted == LINE_MAX && printBreaks) {
        fputc('\n', outputStream);
        *numPrinted = 0;
    }
    fputc(ch, outputStream);
    (*numPrinted)++;
}

/**
 * The start of the execution of the encode program. Will input flags and
 * input/output files as command-line arguments from the user. Reads the
 * binary input file in the specified location and outputs text to the
 * output file whose location is also specified. Will exit and print error
 * messages to standard error if there is an issue with the files inputed.
 * 
 * @param argc the number of command-line arguments
 * @param argv list of command-line arguments
 * @return program exit status
 */
int main(int argc, char *argv[])
{
    // Invalid command-line arguments
    if (argc < MIN_ARGS) {
        fprintf(stderr, "usage: encode [-b] [-p] <input-file> <output-file>\n");
        return EXIT_FAILURE;
    }

    // Initialize input and output filenames based on command-line arguments
    char *inputFilename = argv[ARG_INPUT];
    char *outputFilename = argv[ARG_OUTPUT];

    // Boolean flags representing whether to print '=' symbols and line breaks
    bool printEquals = true;
    bool printBreaks = true;

    // Iterate through remaining command-line arguments that could represent flags
    for (int i = 1; i < ARG_INPUT; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            printEquals = false;
        } else if (strcmp(argv[i], "-b") == 0) {
            printBreaks = false;
        } else {
            fprintf(stderr, "usage: encode [-b] [-p] <input-file> <output-file>\n");
            return EXIT_FAILURE;
        }
    }

    FileBuffer *fileBuffer = loadFileBuffer(inputFilename);

    // Create an output text stream
    errno = 0;
    FILE *outputStream = fopen(outputFilename, "w");
    if (!outputStream) {
        perror(outputFilename);
        return EXIT_FAILURE;
    }

    State24 state;
    initState(&state);

    char buffer[STATE_CHARS];
    int numPrinted = 0;
    bool emptyFile = true;

    // Iterate through bytes of file buffer, adding 3 to the state before outputting to the outputStream
    for (int i = 0; i < fileBuffer->size; i++) {
        if (state.bitCount / BITS_IN_BYTE >= STATE_CAPACITY) {
            getChars(&state, buffer);
            for (int j = 0; j < STATE_CHARS; j++) {
                outputCharacter(buffer[j], outputStream, &numPrinted, printBreaks);
                emptyFile = false;
            }
        }

        addByte(&state, fileBuffer->data[i]);
    }

    int numChars = getChars(&state, buffer);

    // Ouput any remaining characters to the outputStream
    for (int i = 0; i < numChars; i++) {
        outputCharacter(buffer[i], outputStream, &numPrinted, printBreaks);
        emptyFile = false;
    }
    
    // Indicate the amount of padding with '=' characters (unless specified not to)
    if (numChars == SINGLE_PADDING && printEquals) {
        outputCharacter('=', outputStream, &numPrinted, printBreaks);
        outputCharacter('=', outputStream, &numPrinted, printBreaks);
    } else if (numChars == DOUBLE_PADDING && printEquals) {
        outputCharacter('=', outputStream, &numPrinted, printBreaks);
    }

    // Print a trailing newline if the input file was not empty
    if (!emptyFile)
        fputc('\n', outputStream);
    
    // Free dynamically allocated memory & file streams
    freeFileBuffer(fileBuffer);
    fclose(outputStream);
}
