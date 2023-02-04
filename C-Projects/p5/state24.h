/**
 * @file state24.h
 * @author Christopher Fields (cwfields)
 * 
 * Header file for the state64 component of the encoding
 * and decoding base64 software system. Defines a state
 * and behavor for working with that state, including
 * initializing and adding/getting characters or bytes.
 * Also provides a function to allow the user to check
 * if an inputted character is a valid encoding character.
 */

#ifndef _STATE64_H_
#define _STATE64_H_

#include <stdbool.h>

// Include filebuffer to get the byte type.
#include "filebuffer.h"

/** Maximum number of bytes that State24 can hold. */
#define STATE_CAPACITY 3
/** Constant representing the number of characters that can be stored in a State24 struct */
#define STATE_CHARS 4
/** The number of bits in a byte */
#define BITS_IN_BYTE 8
/** The number of bits in a character */
#define BITS_IN_CHAR 6

/**
 * Representation of the current state, including
 * an array of bytes and an integer tracking the
 * number of bits added to the state. 
 */
typedef struct {
  /** Array of bytes representing the current system state. */
  byte bytes[STATE_CAPACITY];
  /** Number of bits currently loaded into the state. */
  int bitCount;
} State24;

/**
 * Will initialize the given pointer to a State24 struct to
 * an empty state, where no bits have been added yet.
 * 
 * @param state pointer to State24 struct to initialize
 */
void initState(State24 *state);

/**
 * Returns true if the parameter is one of the 64 encoding
 * characters for Base 64 encoding, and otherwise false.
 * 
 * @param ch character to check for valid encoding character
 * @return true if ch is a Base 64 encoding character
 */
bool validChar(char ch);

/**
 * Adds 8 bits to the state, given by the bits of byte b.
 * Behavior is undefined if the state doesn't contain enough
 * room for 8 additional bits.
 * 
 * @param state pointer to the State24 struct to add byte to
 * @param b the byte to add to the state
 */
void addByte(State24 *state, byte b);

/**
 * Adds 6 bits to the state, given an encoding character.
 * Behavior is undefined if the state doesn't contain enough
 * room for 6 additioanl bits or the given character isn't a
 * valid encoding character.
 * 
 * @param state pointer to the State24 struct to add character to
 * @param ch the character to add to the state
 */
void addChar(State24 *state, char ch);

/**
 * Fills in the given array with up to three bytes based on the
 * sequence of bits in the given State24 struct. Returns the number
 * of bytes written to the array and sets the given state to empty.
 * 
 * @param state pointer to the State24 struct to get bytes from
 * @param buffer array of bytes to fill based on the bits in state
 * @return the number of bytes written to the array
 */
int getBytes(State24 *state, byte buffer[]);

/**
 * Fills in the given array with character codes for up to 4 encoding
 * characters based on the sequence of bits in the State24 struct. Returns
 * the number of characters written to the array and sets the given state
 * to empty.
 * 
 * @param state pointer to the State24 struct to get characters from
 * @param buffer array of characters to fill based on the bits in state
 * @return the number of characters written to the array
 */
int getChars(State24 *state, char buffer[]);

#endif
