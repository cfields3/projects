/**
 * @file input.h
 * @author Christopher Fields (cwfields)
 * 
 * Header file for the input component from Project 4, reused for Project
 * 6. Provides prototype for a single function, readLine, which is
 * used for reading a single line of input from a given file, returning
 * a pointer to that string as dynamically allocated memory.
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

/**
 * Reads a single line of input from the given file and returns it as a
 * string inside a block of dynamically allocated memory. Used to read
 * commands from the user and to read employee descriptions from an employee
 * list file. Implements a resizable array to read in a line of text that
 * could be arbitrarily long. Returns NULL if there is no input to read.
 * 
 * @param fp pointer to the file stream to read a line from
 * @return char* a pointer to the start of the dynamically allocated string
 */
char *readLine(FILE *fp);

#endif
