/**
 * @file operation.h
 * @author Christopher Fields (cwfields)
 * 
 * Part of the component that provides functions for performing the
 * four basic arithmetic operations on signed long values. Will
 * automatically detect overflow or divide-by-zero, and exit program
 * with cooresponding error code.
 */

/** Exit status value for exiting from overflow */
#define FAIL_OVERFLOW 100
/** Exit status value for exiting from dividing by zero */
#define FAIL_DIVIDE_BY_ZERO 101

/**
 * Adds the given parameters and returns the result. Automatically
 * detects overflow if the result of the addition is outside the
 * range for a signed long. Immediately exits the program with the
 * appropriate error code in the case of overflow.
 * 
 * @param a first number to add
 * @param b second number to add
 * @return result of addition as a signed long
 */
long plus(long a, long b);

/**
 * Substracts the given parameters and returns the result. Automatically
 * detects overflow if the result of the subtraction is outside the
 * range for a signed long. Immediately exits the program with the appropriate
 * error code in the case of overflow.
 * 
 * @param a first number to subtract from
 * @param b second number to subtract with
 * @return result of subtraction as a signed long
 */
long minus(long a, long b);

/**
 * Multiplies the given parameters and returns the result. Automatically
 * detects overflow if the result of the muliplication is outside the
 * range for a signed long. Immediately exits the program with the appropriate
 * error code in the case of overflow.
 * 
 * @param a first number to multiply
 * @param b second number to multiply
 * @return result of multiplication as a signed long
 */
long times(long a, long b);

/**
 * Divides the given parameters and returns the result. Automatically
 * detects overflow if the result of the division is outside the
 * range for a signed long. Also detects any attempt to divide by zero.
 * Immediately exits the program with the appropriate error code in the
 * case of overflow or dividing by zero.
 * 
 * @param a first number to divide
 * @param b second number to divide
 * @return result of division as a signed long
 */
long divide(long a, long b);
