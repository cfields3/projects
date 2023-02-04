/**
 * @file io.h
 * @author Christopher Fields (cwfields)
 *
 * Header file for the component dealing with file
 * input/output in the terminal for the Wordle system.
 * Includes the function prototypes for reading a line
 * from a file and changing the output color when printing
 * to standard output.
 */

#include <stdio.h>
#include <stdbool.h>

/**
 * Reads a line of text from the given file and stores it in str.
 * If the line is longer than n characters, the function reads the
 * whole line but stores just the first n characters. Returns true
 * if there was an additional line of input, and false if there was
 * not any additional input. Discards characters past the first n.
 * 
 * @param fp pointer to the file stream to input lines from
 * @param str string to store line of text in
 * @param n number of characters to store in string
 * @return true if there was an additional line of input
 */
bool readLine(FILE *fp, char str[], int n);

/**
 * Changes the text color to green. 
 */
void colorGreen();

/**
 * Changes the text color to yellow. 
 */
void colorYellow();

/**
 * Changes the text color back to the default.
 */
void colorDefault();
