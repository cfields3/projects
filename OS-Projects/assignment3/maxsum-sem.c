/**
 * @file reset.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 3, Problem 2. Calculates
 * the maximum sum for a contiguous sequence of numbers
 * over an inputed list using a certain number of workers
 * as separate threads. Uses semaphores to dynamically
 * divide up work between different workers as the
 * main thread receives input.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <unistd.h>

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( EXIT_FAILURE );
}

// Print out a usage message, then exit.
static void usage() {
  printf( "usage: maxsum-sem <workers>\n" );
  printf( "       maxsum-sem <workers> report\n" );
  exit( 1 );
}

// True if we're supposed to report what we find.
bool report = false;

// Maximum sum we've found.
int max_sum = INT_MIN;

// Fixed-sized array for holding the sequence.
#define MAX_VALUES 500000
int vList[ MAX_VALUES ];

// Current number of values on the list.
int vCount = 0;

// Semaphore ensuring mutual exclusion when modifying the max_sum.
sem_t sumLock;

// Semaphore ensure mutual exclusion when inside the getWork() function.
sem_t workLock;

// Semaphore used to block threads until there are new elements to read.
sem_t readCount;

// Index that is currently being read.
int readIndex = 0;

// Flag indicating whether the input has been fully read.
bool inputRead = false;

// Read the list of values.
void readList() {
  // Keep reading as many values as we can.
  int v;
  while ( scanf( "%d", &v ) == 1 ) {
    // Make sure we have enough room, then store the latest input.
    if ( vCount > MAX_VALUES )
      fail( "Too many input values" );

    // Store the latest value.
    vList[ vCount++ ] = v;

    sem_post( &readCount );
  }
  inputRead = true;
}

/** Helper function for getting the next index and/or blocking threads to wait for input */
int getWork() {
  // Ensure mutual exclusion when running the getWork() function.
  sem_wait( &workLock );
  // Check to see if all input has already been read
  if ( inputRead && readIndex == vCount ) {
    sem_post( &workLock ); // Release the lock to allow other threads to call getWork().
    return -1; // Return sentinel value of -1, signifying end of input
  }

  // Block if there is no input to read
  sem_wait( &readCount );

  readIndex++; // Increment the index (within the mutual exclusion to ensure two threads are not modifying).
  
  // If there is still input to read, return the index of the input to read
  sem_post( &workLock ); // Release the lock to allow other threads to call getWork().
  return readIndex - 1;
}

/** Start routine for each worker. */
void *workerRoutine( void *arg ) {
  int endIndex = getWork();
  int maxSum = INT_MIN;

  // Continue to calculate the sum of new indices while we have not read all input.
  while ( endIndex != -1 ) {
    int sum = 0;
    for ( int i = endIndex; i >= 0; i-- ) { // Iterate from endIndex to start, finding max contiguous sum.
      sum += vList[ i ];
      if ( sum > maxSum )
        maxSum = sum;
    }

    endIndex = getWork();
  }

  if ( report )
    printf("I'm thread %ld. The maximum sum I found is %d.\n", syscall( SYS_gettid ), maxSum );

  // Ensure mutual exclusion when modifying the value of the maxSum.
  sem_wait( &sumLock );
  if ( maxSum > max_sum )
    max_sum = maxSum; // Replace global variable for maxSum.
  sem_post( &sumLock );

  return NULL;
}

int main( int argc, char *argv[] ) {
  int workers = 4;

  // Initialize the max_sum mutual exclusion lock.
  sem_init( &sumLock, 0, 1 );

  // Initialize the getWork() function's mutual exclusion lock.
  sem_init( &workLock, 0, 1 );

  // Initialize the semaphore tracking available elements to read.
  sem_init( &readCount, 0, 0 );
  
  // Parse command-line arguments.
  if ( argc < 2 || argc > 3 )
    usage();
  
  if ( sscanf( argv[ 1 ], "%d", &workers ) != 1 ||
       workers < 1 )
    usage();

  // If there's a second argument, it better be "report"
  if ( argc == 3 ) {
    if ( strcmp( argv[ 2 ], "report" ) != 0 )
      usage();
    report = true;
  }

  // Make each of the workers.
  pthread_t worker[ workers ];
  for ( int i = 0; i < workers; i++ )
    pthread_create( &worker[ i ], NULL, workerRoutine, NULL );

  // Then, start getting work for them to do.
  readList();

  // Wait until all the workers finish.
  for ( int i = 0; i < workers; i++ )
    pthread_join( worker[ i ], NULL );

  // Report the max product and release the semaphores.
  printf( "Maximum Sum: %d\n", max_sum );
  sem_destroy( &sumLock );
  sem_destroy( &workLock );
  sem_destroy( &readCount );
  
  return EXIT_SUCCESS;
}
