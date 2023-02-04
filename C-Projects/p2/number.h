/**
 * @file number.h
 * @author Christopher Fields (cwfields)
 * 
 * Part of the component that reads numeric values from standard input
 * and writes the results to standard output. Offers an interface for
 * both number10.c and number25.c, since both implementations offer
 * the same interface. Depending on the file that is used (number10.c
 * or number25.c), will calculate numerical values using different base
 * systems. Will also exit program with appropriate error code if an
 * invalid input is given.
 */

#include "operation.h"

/** Exit status indicating that the program was given invalid input. */
#define FAIL_INPUT 102
/** ASCII code for number 0 */
#define ZERO_ASCII 48

/**
 * Reads characters from standard input. Keeps reading characters
 * until it reaches a non-whitespace character or EOF. It returns
 * the code for the first non-whitespace character it finds (or
 * EOF). Code inside the number component or elsewhere in the
 * program can use this to easily skip past whitespace
 * within an expression.
 * 
 * @return the code for the first non-whitespace character it finds
 */
int skipSpace();

/**
 * Reads the next number from the input in specified base. If errors are
 * detected in the input number, it terminates the program with
 * the correct exit status.
 * 
 * @return the next number from the input
 */
long parseValue();

/**
 * Prints the given value to standard output in specified base.
 * 
 * @param val given value to print
 */
void printValue(long val);
