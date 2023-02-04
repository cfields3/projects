/**
 * @file exclude.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 0, Question 3. Copies
 * all lines except line specified as command-line
 * argument from one file to another file. Uses only
 * Unix system calls, excluding all C Standard Library
 * functions.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/** Size of the buffer to read bytes into */
#define BUFFER_SIZE 64
/** Multiplier used for base 10 numbers */
#define BASE_10_MULT 10
/** ASCII value for zero */
#define ZERO_ASCII 48
/** ASCII value for nine */
#define NINE_ASCII 57

/**
 * Prints the usage message for the program
 * to standard error and exits the program.
 */
static void invalidArguments()
{
    char *usageMessage = "usage: exclude <input-file> <output-file> <line-number>\n";
    write(STDERR_FILENO, usageMessage, 57);
    _exit(1);
}

/**
 * Parses a line number read as a string to
 * an integer.
 * 
 * @param string input string to convert
 * @return line number parsed from inputted string
 */
static int parseLineNumber(const char *string)
{
    int lineNumber = 0;
    int digit = 1;
    int numDigits = 0;
    // Count the number of digits in the inputted string
    while(string[numDigits])
        numDigits++;
    // Iterate through each digit, adding its appropriate amount to lineNumber
    for(int i = numDigits - 1; i >= 0; i--) {
        if(string[i] <  ZERO_ASCII || string[i] > NINE_ASCII)
            invalidArguments();
        lineNumber += ((string[i] - ZERO_ASCII) * digit);
        digit *= BASE_10_MULT; // Increment digit to correspond with next number
    }
    return lineNumber;
}

/**
 * Program starting point. Opens files from
 * inputted command line arguments and copies
 * all lines, excluding the user specified
 * line number.
 *
 * @param argc number of command-line arguments
 * @param argv list of command-line arguments
 * @return program exit status
 */
int main(int argc, char const *argv[])
{
    if(argc != 4) {
        invalidArguments();
    }

    // Open the input file in read-only mode
    int input = open(argv[1], O_RDONLY);

    if (input == -1)
        invalidArguments();

    // Open the output file (or create one) in write-only mode
    int output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);

    if (output == -1)
        invalidArguments();

    // Store the inputted line to exclude
    int skipLine = parseLineNumber(argv[3]);

    // Use a buffer to store code read and line to store the current line data
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    int lineIndex = 0;
    int currentLine = 1;

    // Read first block of bytes from input file
    int bytesRead = read(input, buffer, BUFFER_SIZE);
    
    // Continually read blocks into the buffer until there are no more bytes to read
    while(bytesRead > 0) {
        for(int i = 0; i < bytesRead; i++) {
            // Set the next index of the line to the next index of buffer
            line[lineIndex] = buffer[i];
            if(buffer[i] != '\n') {
                lineIndex++; // Don't write anything yet, just increment the line index
            } else {
                if (currentLine != skipLine) {
                    write(output, line, lineIndex + 1); // Write the contents if not skipped
                }
                // Reset the lineIndex and increment the line counter
                lineIndex = 0;
                currentLine++;
            }
        }
        // Read another block of data into the buffer
        bytesRead = read(input, buffer, BUFFER_SIZE);
    }

    // Write any remaining text (if there is not a newline at the end of the file)
    if (lineIndex > 0)
        write(output, line, lineIndex);

    close(input);
    close(output);

    return 0;
}
