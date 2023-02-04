/**
 * @file state24.c
 * @author Christopher Fields (cwfields)
 * 
 * Implementation of the state24 component, storing the
 * current state of the encoding or decoding, represented
 * by up to 24 bits. Allows for the client to interact with
 * this state through various behaviors, including adding bytes
 * or characters to the state and getting bytes or characters
 * from the state. Also provides functions to set the initial
 * state of component and check if a character is a valid base64
 * encoding character.
 */

#include "state24.h"
#include "filebuffer.h"

#include <ctype.h>

/** The number of bits in a quarter of a byte */
#define QUARTER_BYTE BITS_IN_BYTE / 4
/** The number of bits in half of a byte */
#define HALF_BYTE BITS_IN_BYTE / 2
/** The number of bits in 3/4ths of a byte */
#define THREE_QUARTER_BYTE QUARTER_BYTE * 3
/** Value of the Base64 Encoding of a lowercase 'a' */
#define LOWER_A_ENCODING 0x1A
/** Value of the Base64 Encoding of a '0' */
#define ZERO_ENCODING 0x34
/** Value of the Base64 Encoding of a '+' */
#define PLUS_ENCODING 0x3E
/** Value of the Base64 Encoding of a '/' */
#define SLASH_ENCODING 0x3F
/** Index of the first character in a state */
#define STATE_CHAR1 0
/** Index of the second character in a state */
#define STATE_CHAR2 1
/** Index of the third character in a state */
#define STATE_CHAR3 2
/** Index of the fourth character in a state */
#define STATE_CHAR4 3
/** Mask used for converting bytes in state to the first character */
#define STATE_CHAR1_MASK 0xFC
/** Left mask used for converting bytes in state to the second character */
#define STATE_CHAR2_MASKL 0x03
/** Right mask used for converting bytes in state to the second character */
#define STATE_CHAR2_MASKR 0xF0
/** Left mask used for converting bytes in state to the third character */
#define STATE_CHAR3_MASKL 0x0F
/** Right mask used for converting bytes in state to the third character */
#define STATE_CHAR3_MASKR 0xC0
/** Mask used for converting bytes in state to the fourth character */
#define STATE_CHAR4_MASK 0x3F
/** Index of the first byte in a state */
#define STATE_BYTE1 0
/** Index of the second byte in a state */
#define STATE_BYTE2 1
/** Index of the third byte in a state */
#define STATE_BYTE3 2
/** Left mask used for converting second character added to a byte */
#define STATE_BYTE2_MASKL 0x30
/** Right mask used for converting second character added to a byte */
#define STATE_BYTE2_MASKR 0x0F
/** Left mask used for converting second character added to a byte */
#define STATE_BYTE3_MASKL 0x3C
/** Right mask used for converting third character added to a byte */
#define STATE_BYTE3_MASKR 0x03

void initState(State24 *state)
{
    // Set all bytes/bits in state to 0
    for (int i = 0; i < STATE_CAPACITY; i++)
        state->bytes[i] = 0;
    state->bitCount = 0;
}

bool validChar(char ch)
{
    if (isalnum(ch) || ch == '+' || ch == '/') {
        return true;
    } else {
        return false;
    }
}

void addByte(State24 *state, byte b)
{
    // At a new byte at size and increment the bitCount
    state->bytes[state->bitCount / BITS_IN_BYTE] = b;
    state->bitCount += BITS_IN_BYTE;
}

/**
 * Helper method used to convert an inputted character
 * to its equivalent encoded byte value, returning that
 * byte value (unsigned char).
 * 
 * @param ch character to encode
 * @return the encoded byte value of ch
 */
static byte convertChar(char ch)
{
    if (isupper(ch)) {
        return ch - 'A';
    } else if (islower(ch)) {
        return ch - 'a' + LOWER_A_ENCODING;
    } else if (isdigit(ch)) {
        return ch - '0' + (ZERO_ENCODING);
    } else if (ch == '+') {
        return PLUS_ENCODING;
    } else {
        return SLASH_ENCODING;
    }
}

void addChar(State24 *state, char ch)
{
    byte ch64 = convertChar(ch);
    
    // Add the character to the appropriate bytes in the state
    if (state->bitCount / BITS_IN_CHAR == STATE_CHAR1) {
        ch64 = ch64 << QUARTER_BYTE;
        state->bytes[STATE_BYTE1] |= ch64;
    } else if (state->bitCount / BITS_IN_CHAR == STATE_CHAR2) {
        byte leftEncode = (ch64 & STATE_BYTE2_MASKL) >> HALF_BYTE;
        byte rightEncode = (ch64 & STATE_BYTE2_MASKR) << HALF_BYTE;
        state->bytes[STATE_BYTE1] |= leftEncode;
        state->bytes[STATE_BYTE2] |= rightEncode;
    } else if (state->bitCount / BITS_IN_CHAR == STATE_CHAR3) {
        byte leftEncode = (ch64 & STATE_BYTE3_MASKL) >> QUARTER_BYTE;
        byte rightEncode = (ch64 & STATE_BYTE3_MASKR) << THREE_QUARTER_BYTE;
        state->bytes[STATE_BYTE2] |= leftEncode;
        state->bytes[STATE_BYTE3] |= rightEncode;
    } else if (state->bitCount / BITS_IN_CHAR == STATE_CHAR4) {
        state->bytes[STATE_BYTE3] |= ch64;
    }

    state->bitCount += BITS_IN_CHAR;
}

int getBytes(State24 *state, byte buffer[])
{
    // Iterate through each byte in the state, adding that byte to the buffer
    for (int i = 0; i < state->bitCount / BITS_IN_BYTE; i++) {
        buffer[i] = state->bytes[i];
    }

    int numBytes = state->bitCount / BITS_IN_BYTE;
    initState(state); // Revert state to initial state
    return numBytes;
}

/**
 * Helper method used to convert an inputted encoded byte
 * to its equivalent character, returning that character.
 * 
 * @param b the encoded byte to convert to a character
 * @return the character representing the encoded byte b
 */
static char convertByte(byte b)
{
    if (b < LOWER_A_ENCODING) {
        return 'A' + b;
    } else if (b < (ZERO_ENCODING)) {
        return 'a' + b - LOWER_A_ENCODING;
    } else if (b < PLUS_ENCODING) {
        return '0' + b - (ZERO_ENCODING);
    } else if (b == PLUS_ENCODING) {
        return '+';
    } else {
        return '/';
    }
}

int getChars(State24 *state, char buffer[])
{
    // Temporary array of bytes to hold encoded characters in binary
    byte characters[STATE_CHARS];
    
    // Calculate first character from bytes array in state
    characters[STATE_CHAR1] = (state->bytes[STATE_BYTE1] & STATE_CHAR1_MASK) >> QUARTER_BYTE;

    // Calculate second character from bytes array in state
    byte leftEncode = (state->bytes[STATE_BYTE1] & STATE_CHAR2_MASKL) << HALF_BYTE;
    byte rightEncode = (state->bytes[STATE_BYTE2] & STATE_CHAR2_MASKR) >> HALF_BYTE;
    characters[STATE_CHAR2] = leftEncode | rightEncode;

    // Calculate third character from bytes array in state
    leftEncode = (state->bytes[STATE_BYTE2] & STATE_CHAR3_MASKL) << QUARTER_BYTE;
    rightEncode = (state->bytes[STATE_BYTE3] & STATE_CHAR3_MASKR) >> THREE_QUARTER_BYTE;
    characters[STATE_CHAR3] = leftEncode | rightEncode;

    // Calculate final character from bytes array in state
    characters[STATE_CHAR4] = state->bytes[STATE_BYTE3] & STATE_CHAR4_MASK;

    // Number of characters read
    int count = state->bitCount / BITS_IN_CHAR;

    // Add an extra character to count, if necessary, round up rather than down
    if (state->bitCount % BITS_IN_CHAR != 0) {
        count++;
    }

    // Convert each byte from the character array of binary values to the characters' ASCII representation in buffer (as a char)
    for (int i = 0; i < count; i++) {
        buffer[i] = convertByte(characters[i]);
    }

    initState(state);
    return count;
}
