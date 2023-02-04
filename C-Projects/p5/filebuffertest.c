#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "filebuffer.h"

int main()
{
  // Make an empty buffer.
  FileBuffer *buffer = makeFileBuffer();

  // Add some bytes to the buffer.
  appendFileBuffer( buffer, 0x25 );
  appendFileBuffer( buffer, 0xF0 );
  appendFileBuffer( buffer, 0x8C );
  appendFileBuffer( buffer, 0x00 );
  appendFileBuffer( buffer, 0x1A );
  appendFileBuffer( buffer, 0x9B );
  appendFileBuffer( buffer, 0x36 );

  // Make sure the buffer contains all the bytes
  // we just put in it.
  assert( buffer->data[ 0 ] == 0x25 );
  assert( buffer->data[ 1 ] == 0xF0 );
  assert( buffer->data[ 2 ] == 0x8C );
  assert( buffer->data[ 3 ] == 0x00 );
  assert( buffer->data[ 4 ] == 0x1A );
  assert( buffer->data[ 5 ] == 0x9B );

  // Write the buffer out to a file.
  saveFileBuffer( buffer, "output.bin" );

  freeFileBuffer( buffer );

  // Read that file and make sure it contains what we just wrote.
  {
    // Make sure we can open the file.
    FILE *fp = fopen( "output.bin", "rb" );
    assert( fp );

    // Make sure we can read all the bytes we just tried to write.
    byte x[ 1024 ];
    assert( fread( x, 1, 6, fp ) == 6 );

    // Make sure the file contains the bytes we just tried to write to it.
    assert( x[ 0 ] == 0x25 );
    assert( x[ 1 ] == 0xF0 );
    assert( x[ 2 ] == 0x8C );
    assert( x[ 3 ] == 0x00 );
    assert( x[ 4 ] == 0x1A );
    assert( x[ 5 ] == 0x9B );

    fclose( fp );
  }

  // Use a file buffer to read the file's contents.
  buffer = loadFileBuffer( "output.bin" );

  // Make sure the file contains the bytes vaues it should.
  assert( buffer->data[ 0 ] == 0x25 );
  assert( buffer->data[ 1 ] == 0xF0 );
  assert( buffer->data[ 2 ] == 0x8C );
  assert( buffer->data[ 3 ] == 0x00 );
  assert( buffer->data[ 4 ] == 0x1A );
  assert( buffer->data[ 5 ] == 0x9B );

  freeFileBuffer( buffer );

  return EXIT_SUCCESS;
}
