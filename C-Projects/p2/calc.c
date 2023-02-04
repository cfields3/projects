/**
 * @file calc.c
 * @author Christopher Fields (cwfields)
 * 
 * File for the top-level component of the calculator system
 * for Project 2. Responsible for reading and evaluating an
 * expression given through standard input and printing the
 * result in standard output. Utilizes functions from number.h
 * and operation.h to perform calculations, check for overflow,
 * and convert between base 10 and base 25 number systems.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "number.h"

/**
 * Reads the parts of an expression in between the plus or minus operators.
 * Will read a sequence of one or more numbers with multiply and/or divide
 * operators in between and return the value that part of the expression
 * evaluates to. If there's just a number with no multiply or divide operator
 * after it, then it will just return the value of that number.
 * 
 * @return the value that the term of the expression evaluates to
 */
static long parseTerm()
{
    long currentValue = parseValue();
    long term = currentValue;

    int operator = skipSpace();

    while (operator != '+' && operator != '-' && operator != '\n') {
        currentValue = parseValue();

        if (operator == '/') {
            term = divide(term, currentValue);
        } else if (operator == '*') {
            term = times(term, currentValue);
        } else {
            exit(FAIL_INPUT);
        }

        operator = skipSpace();
    }

    // Puts operator back on the input stream so it's there for other code to parse.
    ungetc( operator, stdin );
    return term;
}

/**
 * Program starting point. Reads input from standard input
 * using parseTerm() and skipSpace() functions. Prints the
 * value calculated using operations.c with the printValue()
 * function. Exits with appropriate exit status based on whether
 * the program successfully calculated expression or reached
 * an error state.
 * 
 * @return program exit status
 */
int main()
{
    long currentTerm = parseTerm();
    long result = currentTerm;
    int operator = skipSpace();

    while (operator != '\n') {    
        currentTerm = parseTerm();

        if (operator == '+') {
            result = plus(result, currentTerm);
        } else if (operator == '-') {
            result = minus(result, currentTerm);
        } else {
            exit(FAIL_INPUT);
        }

        operator = skipSpace();
    }

    printValue(result);

    return EXIT_SUCCESS;
}
