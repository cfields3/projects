/**
 * @file reset.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 3, Problem 3. Reset
 * program that creates a shared memory segment that
 * is a representation of a "lights out" game and a named
 * semaphore to allow for mutual exclusion of state. Initializes
 * a board inputted from a file in a shared memory segment.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include "common.h"

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

// Print out a usage message and exit.
static void usage() {
  fprintf( stderr, "usage: reset <board-file>\n" );
  exit( 1 );
}

/**
 * Starting point of the reset program.
 * Creates shared memory segment and stores
 * initial board state from command-line
 * argument.
 *
 * @param argc number of command-line arguments
 * @param argv list of command-line arguments
 * @return exit status of program
 */
int main( int argc, char *argv[] ) {
  // Ensure that there are exactly two CL arguments
  if(argc != 2)
    usage();

  // Generate a specific key based on home directory
  key_t key = ftok(KEY);

  // Create a shared memory segment to store game state
  int shmid = shmget(key, sizeof(GameState), 0666 | IPC_CREAT);

  if(shmid == -1)
    fail("Unable to create shared memory");
  
  // Create a semaphore to be used for mutual exclusion of lightout program
  sem_open( "/cwfields-lightsout-lock", O_CREAT, 0600, 1 );

  // Map the shared memory to the stored GameState struct
  GameState *sharedGameState = (GameState *)shmat(shmid, 0, 0);
  if(sharedGameState == (GameState *) -1)
    fail("Unable to map shared memory to the GameState");
  
  // Open the input file for the board starting state
  char *filename = argv[1];
  FILE *input = fopen(filename, "r");

  if(!input) {
    fprintf(stderr, "Invalid input file: %s\n", filename);
    exit(1);
  }

  // Initialize the board using the inputted file
  for(int row = 0; row < GRID_SIZE; row++) {
    for(int column = 0; column < GRID_SIZE; column++) {
      char light = fgetc(input);
      if (light != '*' && light != '.') {
        fprintf(stderr, "Invalid input file: %s\n", filename);
        exit(1);
      } else {
        sharedGameState->currentBoard[row][column] = light;
      }
    }
    if(fgetc(input) != '\n') {
      fprintf(stderr, "Invalid input file: %s\n", filename);
      exit(1);
    }
  }

  fclose(input);

  // Initialize canUndo flag
  sharedGameState->canUndo = false;

  // Release this program's reference to the shared memory segment
  shmdt(sharedGameState);

  return 0;
}
