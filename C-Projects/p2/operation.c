/**
 * @file operation.c
 * @author Christopher Fields (cwfields)
 * 
 * Implementation of operation.h that provides functions for
 * performing the four basic arithmetic operations on signed
 * long values. Will automatically detect overflow or
 * divide-by-zero, and will exit the program with the
 * cooresponding error code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "operation.h"

long plus(long a, long b)
{
    long result = a + b;

    // Values are positive but result becomes negative
    if (a > 0 && b > 0 && result < 0) {
        exit(FAIL_OVERFLOW);
    }

    // Values are negative but result becomes positive
    if (a < 0 && b < 0 && result > 0) {
        exit(FAIL_OVERFLOW);
    }

    return result;
}

long minus(long a, long b)
{
    long result = a - b;

    // a is negative, b is positive, and result is positive
    if (a < 0 && b > 0 && result > 0) {
        exit(FAIL_OVERFLOW);
    }

    // a is positive, b is negative, and result is negative
    if (a > 0 && b < 0 && result < 0) {
        exit(FAIL_OVERFLOW);
    }

    return result;
}

long times(long a, long b)
{ 
    // Both a and b are positive
    if ( a > 0 && b > 0 && a > (LONG_MAX / b) ) {
        exit(FAIL_OVERFLOW);
    }

    // Both a and b are negative
    if ( a < 0 && b < 0 && a < (LONG_MAX / b) ) {
        exit(FAIL_OVERFLOW);
    }

    // a is negative and b is positive
    if ( a < 0 && b > 0 && a < (LONG_MIN / b) ) {
        exit(FAIL_OVERFLOW);
    }

    // a is positive and b is negative
    if ( a > 0 && b < 0 && a > (LONG_MIN / b) ) {
        exit(FAIL_OVERFLOW);
    }

    return a * b;
}

long divide(long a, long b)
{
    // Divides by zero
    if (b == 0) {
        exit(FAIL_DIVIDE_BY_ZERO);
    }

    // Overflow due to one extra negative signed value
    if (a == LONG_MIN && b == -1) {
        exit(FAIL_OVERFLOW);
    }

    return a / b;
}
