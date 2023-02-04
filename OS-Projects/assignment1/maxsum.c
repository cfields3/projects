/**
 * @file maxsum.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 1, Problem 3. Calculates
 * the maximum sum for a contiguous sequence of numbers
 * over an inputted list using a certain number of workers
 * as child processes.
 */

#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdbool.h>

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

// Print out a usage message, then exit.
static void usage() {
  printf( "usage: maxsum <workers>\n" );
  printf( "       maxsum <workers> report\n" );
  exit( 1 );
}

// Input sequence of values.
int *vList;

// Number of values on the list.
int vCount = 0;

// Capacity of the list of values.
int vCap = 0;

// Read the list of values.
void readList() {
  // Set up initial list and capacity.
  vCap = 5;
  vList = (int *) malloc( vCap * sizeof( int ) );

  // Keep reading as many values as we can.
  int v;
  while ( scanf( "%d", &v ) == 1 ) {
    // Grow the list if needed.
    if ( vCount >= vCap ) {
      vCap *= 2;
      vList = (int *) realloc( vList, vCap * sizeof( int ) );
    }

    // Store the latest value in the next array slot.
    vList[ vCount++ ] = v;
  }
}

int main( int argc, char *argv[] ) {
  bool report = false;
  int workers = 4;

  // Parse command-line arguments.
  if ( argc < 2 || argc > 3 )
    usage();

  if ( sscanf( argv[ 1 ], "%d", &workers ) != 1 ||
       workers < 1 )
    usage();

  // If there's a second argument, it better be the word, report
  if ( argc == 3 ) {
    if ( strcmp( argv[ 2 ], "report" ) != 0 )
      usage();
    report = true;
  }

  readList();

  // Create a pipe for transferring information
  int pfd[ 2 ];
  if( pipe( pfd ) != 0 )
    fail( "Can't create pipe" );

  int worker; // Stores the result of fork() system call
  int workerID; // Stores the id of created worker (0 indexed)

  // Iterate through an create all workers, assigning them a new ID
  for( int i = 0; i < workers; i++ ) {
    worker = fork();
    if ( worker == -1 )
      fail( "Can't create worker process" );
    if ( worker == 0 ) {
      workerID = i;
      break;
    }     
  }

  if ( worker == 0 ) { // Executed code by all worker processes
    // Close the reading end of the pipe
    close( pfd[ 0 ] );

    // Initialize maxSum to smallest value in list
    int maxSum = vList[ 0 ];
    for( int i = 0; i < vCount; i++ ) {
      if(vList[ i ] < maxSum)
        maxSum = vList[ i ];
    }

    // Calculate the maximum sum for specified worker, skipping
    // indices based on the number of workers
    for( int i = workerID; i < vCount; i += workers ) {
      int sum = 0;
      for( int j = i; j < vCount; j++ ) {
        sum += vList[ j ];
        if ( sum > maxSum )
          maxSum = sum;
      }
    }

    // Print out a report for the work, if requested
    if ( report )
      printf( "I'm process %d. The maximum sum I found is %d.\n", getpid(), maxSum );

    // Lock the file to prevent two workers writing simultaneously
    lockf( pfd[ 1 ], F_LOCK, 0 );

    // Write the maxSum found by current worker to anonymous pipe
    write( pfd[ 1 ], &maxSum, sizeof( int ) );

    // Unlock the pipe again to allow other workers to write
    lockf( pfd[ 1 ], F_ULOCK, 0 );

    // Exit the worker process
    exit( 0 );
  } else { // Executed code by the parent process
    // Close the writing end of the pipe
    close( pfd[ 1 ] );

    // Wait for all workers and calculate maxSum based on max values
    int maxSum = 0;
    for( int i = 0; i < workers; i++ ) {
      wait( NULL );

      int workerSum;
      read( pfd[ 0 ], &workerSum, sizeof( int ) );

      if( workerSum > maxSum )
        maxSum = workerSum;
    }

    printf( "Maximum Sum: %d\n", maxSum );
  }

  return 0;
}
