/**
 * @file filebuffer.c
 * @author Christopher Fields (cwfields)
 *
 * The implementation of the filebuffer component of the Base 64
 * Encode and Decode system. Allows the client to perform various
 * operations on a FileBuffer (defined in the header file of the
 * component), including initializing a new FileBuffer, free the
 * contents of a FileBuffer, appending a byte to a FileBuffer,
 * loading the contents of a binary file into a FileBuffer, and
 * saving the contents of a FileBuffer to a binary file.
 */

#include "filebuffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/** Constant used to represent multiplier for resizing capacity */
#define RESIZE_MULTIPLIER 2

FileBuffer *makeFileBuffer()
{
    FileBuffer *fileBuffer = malloc(sizeof(FileBuffer)); // Allocate new FileBuffer
    fileBuffer->data = malloc(sizeof(byte)); // Allocate new data array
    fileBuffer->capacity = 1; // Initial capacity
    fileBuffer->size = 0; // Initial size

    return fileBuffer;
}

void freeFileBuffer(FileBuffer *buffer)
{
    free(buffer->data); // Free data
    free(buffer); // Free buffer itself
}

void appendFileBuffer(FileBuffer *buffer, byte val)
{
    // Reallocate the buffer if it reaches capacity
    if (buffer->size >= buffer->capacity) {
        buffer->data = realloc(buffer->data, RESIZE_MULTIPLIER * buffer->capacity);
        buffer->capacity *= RESIZE_MULTIPLIER;
    }

    buffer->data[buffer->size] = val;
    buffer->size++;
}

FileBuffer *loadFileBuffer(const char *filename)
{
    // Open file stream for reading binary input
    errno = 0;
    FILE *inputStream = fopen(filename, "rb");
    if (!inputStream) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    FileBuffer *fileBuffer = makeFileBuffer();
    
    // Read all data into fileBuffer, reallocating fileBuffer's data field if necessary
    int len;
    while ((len = fread(fileBuffer->data + fileBuffer->size, sizeof(byte), fileBuffer->capacity - fileBuffer->size, inputStream)) != 0) {
        fileBuffer->data = realloc(fileBuffer->data, RESIZE_MULTIPLIER * fileBuffer->capacity);
        fileBuffer->capacity *= RESIZE_MULTIPLIER;
        fileBuffer->size += len;
    }

    fclose(inputStream);
    return fileBuffer;
}

void saveFileBuffer(FileBuffer *buffer, const char *filename)
{
    // Open file stream for writing binary output
    errno = 0;
    FILE *outputStream = fopen(filename, "wb");
    if (!outputStream) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    // Write the entire block of memory for the fileBuffer to the outputStream
    fwrite(buffer->data, sizeof(byte), buffer->size * sizeof(byte), outputStream);

    fclose(outputStream);
}
