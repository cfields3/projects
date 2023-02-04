/**
 * @file scrabbleServer.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 5, Problem 2. Creates
 * a multi-threaded server that is able to server
 * clients through TCP/IP sockets. Creates a Scrabble
 * board and allows clients to add words to the board
 * if they do not collide with existing words and
 * optionally meet words found in a dictionary file,
 * if provided.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <semaphore.h>

/** Port number allocated and assigned to server */
#define PORT_NUMBER "26022"

/** Maximum word length */
#define WORD_LIMIT 26

/** Lock used for mutual exclusion in accessing server state */
sem_t lock;

/** Number of rows on the board */
int rows;

/** Number of columns on the board */
int columns;

/** Representation of the current Scrabble board */
char **currentBoard;

/** Number of valid words read through file */
int numValidWords;

/** List of the valid words read through file */
char **validWords;

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( EXIT_FAILURE );
}

// Print out a usage message, then exit.
static void usage() {
  printf( "usage: scrabbleServer <rows> <cols>\n" );
  exit( 1 );
}

/**
 * Checks if a given word is valid; a word is valid if 
 * it matches one of the words given in validWords.
 * 
 * @param word the word to check if is valid
 * @return whether or not the word is valid
*/
static bool checkWordValid( char *word ) {
  for ( int i = 0; i < numValidWords; i++ ) {
    if ( strcmp( word, validWords[ i ] ) == 0 )
      return true;
  }
  return false;
}

/**
 * Checks if a given board is valid. Ensures that all
 * horizontal and vertical sequences of two or more
 * characters make up a valid word.
 * 
 * @param board the board to check validity of
 * @return whether or not the board is valid
*/
static bool checkBoardValid( char board[rows][columns] ) {
  // Check for Valid Rows
  for ( int row = 0; row < rows; row++ ) {
    for ( int column = 0; column < columns; column++ ) {
      if ( board[ row ][ column ] != 0 ) { // We found a potential start of a word
        char word[ WORD_LIMIT + 1 ];
        int idx = 0;
        // Continue to iterate through board until empty character or end of board
        while ( column < columns && board[ row ][ column ] != 0 ) {
          word[ idx ] = board[ row ][ column ];
          column++;
          idx++;
        }
        word[ idx ] = '\0'; // Add a null terminator to ensure word is read correctly
        // Check all sequences of two or more characters for valid (don't check 1 letter sequences)
        if ( strlen( word ) >= 2 && !checkWordValid( word ) )
          return false;
      }
    }
  }
  
  // Check for Valid Columns (similar but iterate horizontally)
  for ( int column = 0; column < columns; column++ ) {
    for ( int row = 0; row < rows; row++ ) {
      if ( board[ row ][ column ] != 0 ) { // We found a potential start of a word
        char word[ WORD_LIMIT + 1 ];
        int idx = 0;
        // Continue to iterate through board until empty character or end of board
        while ( row < rows && board[ row ][ column ] != 0 ) {
          word[ idx ] = board[ row ][ column ];
          row++;
          idx++;
        }
        word[ idx ] = '\0'; // Add a null terminator to ensure word is read correctly
        // Check all sequences of two or more characters for valid (don't check 1 letter sequences)
        if ( strlen( word ) >= 2 && !checkWordValid( word ) )
          return false;
      }
    }
  }

  return true;
}

/**
 * Helper method for the command "across." Places a word with its
 * left end at the given row and column and extending to the right.
 * If the command is invalid (word extends off the board, contains
 * characters other that lowercase letters), it will return false.
 * 
 * @param r the row to start the word at
 * @param c the column to start the word at
 * @param word the word to add to the board
 * @return whether or not the word was added successfully
*/
static bool across( int r, int c, char *word ) {
  // Check if word is too long (above WORD_LIMIT) or too short (below 1 character)
  if ( word[ WORD_LIMIT ] != '\0' || word[ 0 ] == '\0' )
    return false;

  // Check if word would extend off and/or starts off board
  if ( c + strlen( word ) > columns || r >= rows )
    return false;
  
  sem_wait( &lock ); // Lock reading/updating board under mutual exclusion

  // Create an updated board to store local change (before copying to currentBoard)
  char updatedBoard[rows][columns];
  for ( int i = 0; i < rows; i++ ) {
    for ( int j = 0; j < columns; j++ ) {
      updatedBoard[ i ][ j ] = currentBoard[ i ][ j ];
    }
  }

  // Iterate through each character of the word
  for ( int i = 0; word[ i ]; i++ ) { 
    if ( word[ i ] < 97 || word[ i ] > 122 ) { // If character is not a lowercase letter
      sem_post( &lock );
      return false;
    }  
    
    if ( updatedBoard[ r ][ c + i ] == 0 || updatedBoard[ r ][ c + i ] == word[ i ] ) {  // If character is not valid
      updatedBoard[ r ][ c + i ] = word[ i ];
    } else {
      sem_post( &lock );
      return false;
    }

  }

  // Check if the board is valid, only if there was a words file provided
  if ( numValidWords != -1 && !checkBoardValid( updatedBoard ) ) {
    sem_post( &lock );
    return false;
  }

  // The board is valid - copy the updatedBoard to the currentBoard
  for ( int i = 0; i < rows; i++ ) {
    for ( int j = 0; j < columns; j++ ) {
      currentBoard[ i ][ j ] = updatedBoard[ i ][ j ];
    }
  }

  sem_post( &lock ); // Done reading/writing board, unlock mutual exclusion

  return true;
}

/**
 * Helper method for the command "down." Places a word with its
 * top end at the given row and column and extending downwards.
 * If the command is invalid (word extends off the board, contains
 * characters other that lowercase letters), it will return false.
 * 
 * @param r the row to start the word at
 * @param c the column to start the word at
 * @param word the word to add to the board
 * @return whether or not the word was added successfully
*/
static bool down( int r, int c, char *word ) {
  // Check if word is too long (above WORD_LIMIT) or too short (below 1 character)
  if ( word[ WORD_LIMIT ] != '\0' || word[ 0 ] == '\0' )
    return false;

  // Check if word would extend off and/or starts off board
  if ( r + strlen( word ) > rows || c >= columns )
    return false;

  sem_wait( &lock ); // Lock reading/updating board under mutual exclusion

  // Create an updated board to store local change (before copying to currentBoard)
  char updatedBoard[rows][columns];
  for ( int i = 0; i < rows; i++ ) {
    for ( int j = 0; j < columns; j++ ) {
      updatedBoard[ i ][ j ] = currentBoard[ i ][ j ];
    }
  }

  // Iterate through each character of the word
  for ( int i = 0; word[ i ]; i++ ) {
    if ( word[ i ] < 97 || word[ i ] > 122 ) { // If character is not a lowercase letter
      sem_post( &lock );
      return false;
    }
    
    if ( updatedBoard[ r + i ][ c ] == 0 || updatedBoard[ r + i ][ c ] == word[ i ] ) { // If character is not valid
      updatedBoard[ r + i ][ c ] = word[ i ];
    } else {
      sem_post( &lock );
      return false;
    }
  }

  // Check if the board is valid, only if there was a words file provided
  if ( numValidWords != -1 && !checkBoardValid( updatedBoard ) ) {
    sem_post( &lock );
    return false;
  }

  // The board is valid - copy the updatedBoard to the currentBoard
  for ( int i = 0; i < rows; i++ ) {
    for ( int j = 0; j < columns; j++ ) {
      currentBoard[ i ][ j ] = updatedBoard[ i ][ j ];
    }
  }

  sem_post( &lock ); // Done reading/writing board, unlock mutual exclusion

  return true;
}

/**
 * Helper method for the command "board." Prints the current
 * state of the board to the given file descriptor. Will
 * include a border and print spaces for all missing characters.
 * 
 * @param fp the file pointer to print to
*/
static void board( FILE *fp ) {
  sem_wait( &lock ); // Lock reading board under mutual exclusion
  
  // Print top border.
  fprintf( fp, "+" );
  for ( int i = 0; i < columns; i++ ) {
    fprintf( fp, "-" );
  }
  fprintf( fp, "+\n" );

  // Print the contents (and side borders) of the board.
  for ( int i = 0; i < rows; i++ ) {
    fprintf( fp, "|" );

    for ( int j = 0; j < columns; j++ ) {
      if ( currentBoard[ i ][ j ] == 0 ) {
        fprintf( fp, " " );
      } else {
        fprintf( fp, "%c", currentBoard[ i ][ j ] );
      }
    }

    fprintf( fp, "|\n" );
  }

  // Print the bottom border.
  fprintf( fp, "+" );
  for ( int i = 0; i < columns; i++ ) {
    fprintf( fp, "-" );
  }
  fprintf( fp, "+\n" );

  sem_post( &lock ); // Done reading board, unlock mutual exclusion
}

/**
 * Helper function to look for a "words" file in the
 * current director and read all words into the validWords
 * file if it exists.
*/
static void readValidWords() {
  FILE *wordsFile = fopen( "words", "r" );

  if ( !wordsFile ) {
    numValidWords = -1; // Indicate we did not find a words file in the current directory
  } else {
    numValidWords = 0;
    int capacity = 2;
    validWords = malloc( capacity * sizeof(char *) ); // Allocate space for capacity char pointers
    char wordBuffer[ WORD_LIMIT + 1 ];
    while ( fscanf( wordsFile, "%26s", wordBuffer ) == 1 ) { // Read the next word found in words file
      if ( numValidWords >= capacity ) { // Grow the validWords array as needed
        capacity *= 2;
        validWords = realloc( validWords, capacity * sizeof(char *) );
      }
      validWords[ numValidWords ] = malloc( strlen(wordBuffer) + 1 ); // Allocate room for the new word
      strcpy( validWords[ numValidWords ], wordBuffer );
      numValidWords++;
    }
  }
}

/** handle a client connection, close it when we're done. */
void *handleClient( void *arg ) {
  // Read in the socket from the argument pointer
  int *sockPtr = (int *) arg;
  int sock = *sockPtr;

  // Here's a nice trick, wrap a C standard IO FILE around the
  // socket, so we can communicate the same way we would read/write
  // a file.
  FILE *fp = fdopen( sock, "a+" );
  
  // Prompt the user for a command.
  fprintf( fp, "cmd> " );

  // Temporary values for parsing commands.
  char cmd[ 11 ];
  while ( fscanf( fp, "%10s", cmd ) == 1 &&
          strcmp( cmd, "quit" ) != 0 ) {
    if ( strcmp( cmd, "across" ) == 0 ) {
      int row;
      int column;
      char word[ WORD_LIMIT + 1 ];
      word[ WORD_LIMIT ] = '\0'; // Ensure that last character is '\0' to allow for length error checking
      fscanf( fp, "%d%d%27s", &row, &column, word );
      if ( !across( row, column, word ) ) // Execute across command, printing "Invalid command" if invalid
        fprintf( fp, "Invalid command\n" );
    } else if ( strcmp( cmd, "down" ) == 0 ) {
      int row;
      int column;
      char word[ WORD_LIMIT + 1 ];
      word[ WORD_LIMIT ] = '\0'; // Ensure that last character is '\0' to allow for length error checking
      fscanf( fp, "%d%d%27s", &row, &column, word );
      if ( !down( row, column, word ) ) // Execute down command, printing "Invalid command" if invalid
        fprintf( fp, "Invalid command\n" );
    } else if ( strcmp( cmd, "board" ) == 0 ) {
      board( fp ); // Execut board command, specifying printing to client file pointer
    } else { // Command not one specified, so print an error to the client
      fprintf( fp, "Invalid command\n" );
    }

    // Prompt the user for the next command.
    fprintf( fp, "cmd> " );
  }

  // Close the connection with this client.
  fclose( fp );
  return NULL;
}

int main( int argc, char *argv[] ) {
  // Print usage message & exit on invalid command-line arguments.
  if ( argc != 3 )
    usage();

  // Read rows and columns, exiting with usage message if are not integers > 0.
  if ( sscanf( argv[ 1 ], "%d", &rows ) != 1 || sscanf( argv[ 2 ], "%d", &columns ) != 1 ) // Check and store integer values.
    usage();
  if ( sscanf( argv[ 1 ], "%*d%*s" ) != EOF || sscanf( argv[ 2 ], "%*d%*s" ) != EOF ) // Check that there are no other values.
    usage();
  if ( rows <= 0 || columns <= 0 ) // Check that rows and columns are > 0.
    usage();

  // Read in data from words file, if found
  readValidWords();

  // Initialize the board
  currentBoard = malloc( rows * sizeof( *currentBoard ) );
  for ( int i = 0; i < rows; i++ ) {
    currentBoard[ i ] = calloc( columns * sizeof( currentBoard[ 0 ] ), sizeof( currentBoard[ 0 ] ) );
  }

  // Initialize a semaphore to keep track of mutual exclusion in reading/modifying the board.
  sem_init( &lock, 0, 1 );

  // Prepare a description of server address criteria.
  struct addrinfo addrCriteria;
  memset(&addrCriteria, 0, sizeof(addrCriteria));
  addrCriteria.ai_family = AF_INET;
  addrCriteria.ai_flags = AI_PASSIVE;
  addrCriteria.ai_socktype = SOCK_STREAM;
  addrCriteria.ai_protocol = IPPROTO_TCP;

  // Lookup a list of matching addresses
  struct addrinfo *servAddr;
  if ( getaddrinfo( NULL, PORT_NUMBER, &addrCriteria, &servAddr) )
    fail( "Can't get address info" );

  // Try to just use the first one.
  if ( servAddr == NULL )
    fail( "Can't get address" );

  // Create a TCP socket
  int servSock = socket( servAddr->ai_family, servAddr->ai_socktype,
                         servAddr->ai_protocol);
  if ( servSock < 0 )
    fail( "Can't create socket" );

  // Bind to the local address
  if ( bind(servSock, servAddr->ai_addr, servAddr->ai_addrlen) != 0 )
    fail( "Can't bind socket" );
  
  // Tell the socket to listen for incoming connections.
  if ( listen( servSock, 5 ) != 0 )
    fail( "Can't listen on socket" );

  // Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);

  // Fields for accepting a client connection.
  struct sockaddr_storage clntAddr; // Client address
  socklen_t clntAddrLen = sizeof(clntAddr);

  while ( true  ) {
    // Accept a client connection.
    int sock = accept( servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);

    // Create a new thread to handle the client connection & detach it from the main thread.
    pthread_t thread;
    int *arg = &sock;
    pthread_create( &thread, NULL, handleClient, arg );
    pthread_detach( thread );
  }

  // Stop accepting client connections (never reached).
  close( servSock );

  // Remove the semaphore associated with mutual exclusion (never reached).
  sem_destroy( &lock );
  
  return 0;
}
