/** 
    @file driver.c
    @author Christopher Fields (cwfields)
    Main program for the hash map program. Provides the top-level
    main compoent. Using the other components, it reads and processes
    commands from standard input, updates the map as needed, and prints
    the repsponses to user commands.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "map.h"
#include "vtype.h"
#include "integer.h"
#include "text.h"
#include "input.h"

/** Maximum length for a command name. */
#define MAX_CMD 10
/** The initial capacity of the Map */
#define MAP_CAPACITY 100

/** 
    Front-end for the Integer and Text parsing functions.  This tries
    to make an integer from the given string, and, failing that, tries
    to make a Text object.
    @param init String containing the initializaiton text.
    @param n Optional return for the number of characters used from init.
    @return pointer to the new VType instance.
    
 */
static VType *parseVType( char const *init, int *n )
{
  VType *val = parseInteger( init, n );

  if ( ! val )
    val = parseText( init, n );
  
  return val;
}

/** Return true if the given string contains only whitespace.  This
    is useful for making sure there's nothing extra at the end of a line
    of user input.
    @paam str String to check for blanks.
    @return True if the string contains only blanks.
*/
static bool blankString( char *str )
{
  // Skip spaces.
  while ( isspace( *str ) )
    ++str;

  // Return false if we see non-whitespace before the end-of-string.
  if ( *str )
    return false;
  return true;
}

/**
   Starting point for the program.
   @return exit status for the program.
 */
int main()
{
  // Make our map, with a 100-element table.
  Map *map = makeMap( MAP_CAPACITY );

  // Keep reading input from the user.
  char *line;
  printf( "cmd> " );
  while ( ( line = readLine( stdin ) ) ) {
    // Echo the command back to the user.
    printf( "%s\n", line );

    // Extract the first word from the command.
    bool valid = false;
    char cmd[ MAX_CMD + 1 ];
    int n;
    if ( sscanf( line, "%10s%n", cmd, &n ) == 1 ) {
      // Pos keeps up with where we are in parsing the command.
      char *pos = line + n;
      if ( strcmp(cmd, "set" ) == 0 ) {
        // Parse the key from the command.
        VType *k = parseVType( pos, &n );
        if ( k ) {
          pos += n;

          // Parse the value from the command.
          VType *v = parseVType( pos, &n );
          if ( v ) {
            pos += n;

            // Make sure we got a key and there's nothing extra in the command.
            if ( blankString( pos ) ) {
              valid = true;
              mapSet(map, k, v);
            }
          }
        }
      } else if ( strcmp( cmd, "get" ) == 0 ) {
        // Parse the key from the command.
        VType *k = parseVType( pos, &n );
        if ( k ) {
          pos += n;

          // Make sure we got a key and there's nothing extra in the command.
          if ( blankString( pos ) ) {
            valid = true;
            VType *v = mapGet( map, k );
            // Report the value for this key, or undefined.
            if ( v ) {
              v->print( v );
              printf( "\n" );
            } else
              printf( "Undefined\n" );
          }

          // Free the key we parsed from the input.
          k->destroy( k );
        }
      } else if ( strcmp( cmd, "remove" ) == 0 ) {
        // Parse the key from the command.
        VType *k = parseVType( pos, &n );
        if ( k ) {
          pos += n;

          // Make sure we got a key and there's nothing extra in the command.
          if ( blankString( pos ) ) {
            valid = true;
            bool removed = mapRemove(map, k);
            // Report the value for this key, or undefined.
            if ( !removed ) {
              printf( "Not in map\n" );
            }
          }

          // Free the key we parsed from the input.
          k->destroy( k );
        }
      } else if ( strcmp( cmd, "size" ) == 0 ) {
        // Any extra input after the command?
        if ( blankString( pos ) ) {
          // Report the size of the map.
          valid = true;
          printf( "%d\n", mapSize( map ) );
        }
      } else if ( strcmp( cmd, "quit" ) == 0 ) {
        // Free the current command and the map before exiting.
        free( line );
        freeMap( map );
        exit( EXIT_SUCCESS );
      }
    }

    // Print a message if we didn't get a valid command.
    if ( ! valid )
      printf( "Invalid command\n" );

    // Free the last command and prompt for another one.
    free( line );
    printf( "\ncmd> " );
  }

  // Free the map before exiting.
  freeMap( map );
  return EXIT_SUCCESS;
}
