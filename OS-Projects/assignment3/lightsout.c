/**
 * @file lightsout.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 3, Problem 3. Lightsout
 * program that communicates with the user by receiving
 * a command as command-line arguments and alters the
 * value of a shared memory segment to reflect changes
 * in board state. Updated to include mutual exclusion code
 * to protect access to shared memory segment.
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

/** Value of zero in ASCII */
#define ZERO_ASCII 48
/** Value of four in ASCII */
#define FOUR_ASCII 52
/** Size of board as a string */
#define BOARD_STRING 37

/** Lock used for mutual exclusion in accessing game state */
sem_t *lock;

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

/**
 * Prints an error message and exits
 * unsuccessfully.
 */
static void error() {
  printf("error\n");
  exit(1);
}

/**
 * Inverts an inputted light on the current board.
 * 
 * @param state GameState to modify
 * @param row index of the row
 * @param column index of the column
 */
void invertLight(GameState *state, int row, int column) {
  if(state->currentBoard[row][column] == '.') {
    state->currentBoard[row][column] = '*';
  } else {
    state->currentBoard[row][column] = '.';
  }
}

/**
 * Changes the lights at a particular coordinate (row, column)
 * on the board. Changes the light at the coordinate
 * inputted and all adjacent lights.
 * 
 * @param state GameState to modify
 * @param row index of the row
 * @param column index of the column
 * @return whether the move was successful
 */
bool move(GameState *state, int row, int column) {
  #ifndef UNSAFE
    sem_wait(lock);
  #endif
  if ( row < 0 || row >= GRID_SIZE || column < 0 || column >= GRID_SIZE ) {
    #ifndef UNSAFE
      sem_post(lock);
    #endif
    return false;
  }
  memcpy(state->previousBoard, state->currentBoard, sizeof(state->previousBoard));
  invertLight(state, row, column);
  if(row != 0)
    invertLight(state, row - 1, column);
  if(column != 0)
    invertLight(state, row, column - 1);
  if(row != GRID_SIZE - 1)
    invertLight(state, row + 1, column);
  if(column != GRID_SIZE - 1)
    invertLight(state, row, column + 1);
  state->canUndo = true;
  #ifndef UNSAFE
    sem_post(lock);
  #endif
  return true;
}

/**
 * Undoes the last move the user made. An undo is
 * not permitted after another undo or immediately
 * after program start (before any moves).
 * 
 * @param state GameState to modify
 * @return whether the undo was successful
 */
bool undo(GameState *state) {
  #ifndef UNSAFE
    sem_wait(lock);
  #endif
  if(state->canUndo) {
    memcpy(state->currentBoard, state->previousBoard, sizeof(state->currentBoard));
    state->canUndo = false;
    #ifndef UNSAFE
      sem_post(lock);
    #endif
    return true;
  } else {
    #ifndef UNSAFE
      sem_post(lock);
    #endif
    return false;
  }
}

/**
 * Reports the current state of the board
 * as a string of characters into stdout.
 * 
 * @param state GameState to modify
 */
void report(GameState *state) {
  #ifndef UNSAFE
    sem_wait(lock);
  #endif
  char output[BOARD_STRING]; 
  int idx = 0;
  for(int row = 0; row < GRID_SIZE; row++) {
    for(int column = 0; column < GRID_SIZE; column++) {
      output[idx] = state->currentBoard[row][column];
      idx++;
    }
    output[idx] = '\n';
    idx++;
  }
  output[idx] = '\0';
  printf(output);
  #ifndef UNSAFE
    sem_post(lock);
  #endif
}

/**
 * Test interface, for quickly making a given move over and over.
 *
 * @param state GameState to modify 
 * @param n number of times to execute move
 * @param r row to execute move on
 * @param c column to execute move on
 * @return whether the test moves were successful
 */
bool test( GameState *state, int n, int r, int c ) {
  // Make sure the row / column is valid.
  if ( r < 0 || r >= GRID_SIZE || c < 0 || c >= GRID_SIZE )
    return false;
  // Make the same move a bunch of times.
  for ( int i = 0; i < n; i++ )
    move( state, r, c );
  return true;
}

/**
 * Starting point of the lightsout program.
 * Attaches a shared memory segment created
 * by the reset.c program and changes the
 * state of the board stored in shared memory
 * based on the command entered by the user
 * with command-line arguments.
 *
 * @param argc number of command-line arguments
 * @param argv list of command-line arguments
 * @return exit status of program
 */
int main( int argc, char *argv[] ) {
  // Generate a specific key based on home directory
  key_t key = ftok(KEY);

  // Get the shared memory segment created in reset.c
  int shmid = shmget(key, sizeof(GameState), 0666 | IPC_CREAT);

  if(shmid == -1)
    fail("Unable to create shared memory");

  // Open the named semaphore created in reset.c
  lock = sem_open("/cwfields-lightsout-lock", O_RDWR);

  // Map the shared memory to the stored GameState struct
  GameState *sharedGameState = (GameState *)shmat(shmid, 0, 0);
  if(sharedGameState == (GameState *)-1)
    fail("Unable to map shared memory to local address space");

  if (argc < 2)
    fail("You must provide a command to run.");

  // Determine the command, perform actions, and update response accordingly
  if (strcmp(argv[1], "move") == 0) {
    if(argc != 4)
      error();
    int row = *argv[2] - ZERO_ASCII;
    int column = *argv[3] - ZERO_ASCII;
    if (move(sharedGameState, row, column)) {
      printf("success\n");
    } else {
      error();
    }
  } else if (strcmp(argv[1], "undo") == 0) {
    if(argc != 2)
      error();
    if (undo(sharedGameState)) {
      printf("success\n");
    } else {
      error();
    }
  } else if (strcmp(argv[1], "report") == 0) {
    if(argc != 2)
      error();
    report(sharedGameState);
  } else if (strcmp(argv[1], "test") == 0) {
    if(argc != 5)
      error();
    int row = *argv[3] - ZERO_ASCII;
    int column = *argv[4] - ZERO_ASCII;
    if (!test(sharedGameState, *argv[2], row, column))
      error();
  } else {
    error();
  }

  // Release this program's reference to the shared memory segment
  shmdt(sharedGameState);

  return 0;
}
