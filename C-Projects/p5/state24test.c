#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "state24.h"

int main()
{
  ///////////////////////////////////////////////

  // Try the validChar() function.  That should be easy.
  assert( validChar( 'A' ) );
  assert( validChar( 'Z' ) );
  assert( validChar( 'a' ) );
  assert( validChar( 'z' ) );
  assert( validChar( '0' ) );
  assert( validChar( '9' ) );
  assert( validChar( '+' ) );
  assert( validChar( '/' ) );
  
  assert( ! validChar( '-' ) );
  assert( ! validChar( '.' ) );
  assert( ! validChar( '*' ) );
  assert( ! validChar( '[' ) );
  assert( ! validChar( '\\' ) );
  assert( ! validChar( '#' ) );
  assert( ! validChar( '?' ) );
  
  ///////////////////////////////////////////////
  
  // Make a new state and initialize it.
  State24 state;
  initState( &state );

  // Put three bytes in the state.
  addByte( &state, 0x75 );
  addByte( &state, 0xAD );
  addByte( &state, 0x39 );

  // Make sure we get those same bytes back.
  byte byteBuffer[ 3 ];
  assert( getBytes( &state, byteBuffer ) == 3 );
  assert( byteBuffer[ 0 ] ==  0x75 );
  assert( byteBuffer[ 1 ] ==  0xAD );
  assert( byteBuffer[ 2 ] ==  0x39 );
  
  // We don't have to reset the state of the buffer.  That should
  // have happened when we called getBytes().

  ///////////////////////////////////////////////
  
  // Try adding 24 bits as 3 bytes
  addByte( &state, 0x5B );
  addByte( &state, 0x26 );
  addByte( &state, 0xE8 );

  // Then extract them as four characters.  It's 5 elements in case the
  // the implementation of getChars() adds null termination.
  char charBuffer[ 5 ];
  assert( getChars( &state, charBuffer ) == 4 );
  assert( charBuffer[ 0 ] ==  'W' );
  assert( charBuffer[ 1 ] ==  'y' );
  assert( charBuffer[ 2 ] ==  'b' );
  assert( charBuffer[ 3 ] ==  'o' );

  ///////////////////////////////////////////////
  
  // Try adding 24 bits as 4 characters.
  addChar( &state, 'C' );
  addChar( &state, 'G' );
  addChar( &state, '9' );
  addChar( &state, 'g' );

  // Then extract them as bytes.
  assert( getBytes( &state, byteBuffer ) == 3 );
  assert( byteBuffer[ 0 ] == 0x08 );
  assert( byteBuffer[ 1 ] == 0x6F );
  assert( byteBuffer[ 2 ] == 0x60 );
  
  ///////////////////////////////////////////////
  
  // Try adding 16 bits as 2 bytes
  addByte( &state, 0xE3 );
  addByte( &state, 0x07 );

  // We should get 3 characters from this.
  assert( getChars( &state, charBuffer ) == 3 );
  assert( charBuffer[ 0 ] ==  '4' );
  assert( charBuffer[ 1 ] ==  'w' );
  assert( charBuffer[ 2 ] ==  'c' );

  ///////////////////////////////////////////////
  
  // Try adding 18 bits as 3 characters.
  addChar( &state, '7' );
  addChar( &state, 'B' );
  addChar( &state, '+' );

  // We should get 2 bytes from this.
  assert( getBytes( &state, byteBuffer ) == 2 );
  assert( byteBuffer[ 0 ] == 0xEC );
  assert( byteBuffer[ 1 ] == 0x1F );
  
  ///////////////////////////////////////////////
  
  // Try adding 8 bits as 1 byte
  addByte( &state, 0x4E );
  
  // We should get 2 characters from this.
  assert( getChars( &state, charBuffer ) == 2 );
  assert( charBuffer[ 0 ] ==  'T' );
  assert( charBuffer[ 1 ] ==  'g' );

  ///////////////////////////////////////////////
  
  // Try adding 12 bits as 2 characters.
  addChar( &state, '/' );
  addChar( &state, 'M' );

  // We should get 1 byte from this.
  assert( getBytes( &state, byteBuffer ) == 1 );
  assert( byteBuffer[ 0 ] == 0xFC );
  
  return EXIT_SUCCESS;
}
