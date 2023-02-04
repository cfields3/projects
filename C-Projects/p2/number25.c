/**
 * @file number25.c
 * @author Christopher Fields (cwfields)
 * 
 * Base 25 implementation of the component that reads numeric values
 * from standard input and writes the results to standard output. Uses
 * functions defined in number.h to read values of input to be used in
 * calc.c. Will also exit program with appropriate error code if an
 * invalid input is given.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "number.h"

/** Specified base for the current program */
#define BASE 25
/** Base 25 for A or start of letters in Base 25 */
#define A_BASE_25 10
/** Base 25 for O or end of letters in Base 25 */
#define O_BASE_25 24
/** ASCII code for letter A */
#define LETTER_A_ASCII 65

int skipSpace()
{
    int ch = getchar();
    while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\v' || ch == '\f') {
        ch = getchar();
    }
    return ch;
}

long parseValue()
{
    // Value we've parsed so far.
    long value = 0;
  
    // Get the next input character.
    int ch = skipSpace();

    // Accommodate for negative values
    bool negative = false;
    if (ch == '-') {
        negative = true;
        ch = getchar();
    }

    if ( (ch < '0') || (ch > '9' && ch < 'A') || (ch > 'O') ) {
        exit(FAIL_INPUT);
    }

    // Keep reading as long as we're seeing digits.
    while ( (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'O') ) {
        // Convert from ASCII code for the next digit into the value
        // of that digit.  For example 'A' -> 10 or '7' -> 7
        int digit;
        if (ch >= '0' && ch <= '9') {
            digit = ch - ZERO_ASCII;
        } else if (ch >= 'A' && ch <= 'O') {
            digit = ch - LETTER_A_ASCII + A_BASE_25;
        }

        // Slide all digits we've read so far one place value to the
        // left.
        value = times(value, BASE);
  
        // Add this digit as a new, low-order digit.
        value = plus(value, digit);
    
        // Get the next input character.
        ch = skipSpace();
    }

    // Puts ch it back on the input stream so it's there for other code to parse.
    ungetc( ch, stdin );

    if (negative) {
        value *= -1;
    }

    return value;
}

/**
 * Prints a digit (and continues to recursively call itelf
 * for additional digits) in the inputted value in base 25.
 * 
 * @param val given value to print
 */
static void printDigit(long val)
{
    // While there are more digits to print.
    if ( val != 0 ) {
        // Get the next digit on the right.
        int digit = val % BASE;
    
        // Convert it to a character, e.g, 13 -> 'D' or 3 -> '3'
        char ch;
        if (digit >= 0 && digit <= 9) {
            ch = digit + ZERO_ASCII;
        } else if (digit >= A_BASE_25 && digit <= O_BASE_25) {
            ch = digit + LETTER_A_ASCII - A_BASE_25;
        }
    
        // Slide remaining digits to the right.
        val = divide(val, BASE);

        // Recursively call printValue to print the next digit
        printDigit(val);

        // Print out the next digit
        putchar(ch);
    }
}

void printValue(long val)
{
    // Base case if value is LONG_MIN
    if (val == LONG_MIN) {
        printf("-64IE1FOCNN5G78\n");
        exit(EXIT_SUCCESS);
    }

    // Check if the value is negative, printing out "-" if so
    if (val < 0) {
        putchar('-');
        val = times(val, -1);
    }

    // Print 0 if the orignal value was 0
    if (val == 0) {
        putchar('0');
    }

    // Print the digits in the value, recursively
    printDigit(val);
    printf("\n");
}
