/**
 * @file filebuffer.h
 * @author Christopher Fields (cwfields)
 * 
 * Header file for the filebuffer component of the Base 64 Encode
 * and Decode system. Includes the definition of a FileBuffer struct
 * and typedef for a byte as well as function prototypes for the
 * behavior of the component. Defines behavior to make a FileBuffer,
 * free a FileBuffer, append a byte to a FileBuffer, load the contents
 * of a file into a FileBuffer, and save the contents of a FileBuffer
 * to a file.
 */

#ifndef _FILEBUFFER_H_
#define _FILEBUFFER_H_

/** A shorthand for talking about a byte. */
typedef unsigned char byte;

/** Representation of a resizable array of bytes, with functions to
 * make it easy to read and write the contents of this array to a
 * file.
 */
typedef struct {
  /** Resizable array of bytes stored in this filebuffer. */
  byte *data;
  /** Current maximum number of elements that can be stored in data array. */
  int capacity;
  /** Current number of elements stores in data array. */
  int size;
} FileBuffer;

/**
 * Dynamically allocates a FileBuffer, initializing it
 * so that it contians an empty sequence of bytes.
 * 
 * @return a pointer to the new FileBuffer 
 */
FileBuffer *makeFileBuffer();

/**
 * Frees all memory used to represent the given FileBuffer.
 * 
 * @param buffer pointer to the FileBuffer to free
 */
void freeFileBuffer(FileBuffer *buffer);

/**
 * Adds a byte with the given value to the end of the byte
 * sequence stored inside the given FileBuffer. Grows the
 * internal data array in FileBuffer if necessary to add
 * the given byte to the end.
 * 
 * @param buffer the FileBuffer to add the given byte to
 * @param val the value of the byte to add to the buffer
 */
void appendFileBuffer(FileBuffer *buffer, byte val);

/**
 * Reads a binary input file, stores its contents in the
 * resizable array inside a new FileBuffer and returns it
 * to the caller. Allows the client to access the file's
 * contents through the FileBuffer's data array.
 * 
 * @param filename name of the file to read binary input from
 * @return pointer to the FileBuffer to load file contents to
 */
FileBuffer *loadFileBuffer(const char *filename);

/**
 * Saves the contents of the given FileBuffer to a binary file
 * with the given name.
 * 
 * @param buffer the FileBuffer to read from
 * @param filename the name of the file to save buffer to
 */
void saveFileBuffer(FileBuffer *buffer, const char *filename);

#endif
