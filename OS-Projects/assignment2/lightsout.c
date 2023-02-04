/**
 * @file lightsout.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 2, Problem 3. Lightsout
 * program that communicates with the user by receiving
 * a command as command-line arguments and alters the
 * value of a shared memory segment to reflect changes
 * in board state.
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
#include "common.h"

/** Value of zero in ASCII */
#define ZERO_ASCII 48
/** Value of four in ASCII */
#define FOUR_ASCII 52
/** Size of board as a string */
#define BOARD_STRING 37

/** Store the current and most recent board states */
GameState gameState;

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
 * @param row index of the row
 * @param column index of the column
 */
void invertLight(int row, int column) {
  if(gameState.currentBoard[row][column] == '.') {
    gameState.currentBoard[row][column] = '*';
  } else {
    gameState.currentBoard[row][column] = '.';
  }
}

/**
 * Changes the lights at a particular coordinate (row, column)
 * on the board. Changes the light at the coordinate
 * inputted and all adjacent lights.
 * 
 * @param row index of the row
 * @param column index of the column
 * @return whether the move was successful
 */
bool move(int row, int column) {
  memcpy(gameState.previousBoard, gameState.currentBoard, sizeof(gameState.previousBoard));
  invertLight(row, column);
  if(row != 0)
    invertLight(row - 1, column);
  if(column != 0)
    invertLight(row, column - 1);
  if(row != GRID_SIZE - 1)
    invertLight(row + 1, column);
  if(column != GRID_SIZE - 1)
    invertLight(row, column + 1);
  gameState.canUndo = true;
  return true;
}

/**
 * Undoes the last move the user made. An undo is
 * not permitted after another undo or immediately
 * after program start (before any moves).
 * 
 * @return whether the undo was successful
 */
bool undo() {
  if(gameState.canUndo) {
    memcpy(gameState.currentBoard, gameState.previousBoard, sizeof(gameState.currentBoard));
    gameState.canUndo = false;
    return true;
  } else {
    return false;
  }
}

/**
 * Reports the current state of the board
 * as a string of characters into stdout.
 * 
 * @return whether the report was successful
 */
bool report() {
  char output[BOARD_STRING]; 
  int idx = 0;
  for(int row = 0; row < GRID_SIZE; row++) {
    for(int column = 0; column < GRID_SIZE; column++) {
      output[idx] = gameState.currentBoard[row][column];
      idx++;
    }
    output[idx] = '\n';
    idx++;
  }
  output[idx] = '\0';
  printf(output);
  return true;
}

/**
 * Starting point of the lightsout program.
 * Attatches a shared memory segment created
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

  // Map the shared memory to the stored GameState struct
  GameState *sharedGameState = (GameState *)shmat(shmid, 0, 0);
  if(sharedGameState == (GameState *)-1)
    fail("Unable to map shared memory to local address space");

  gameState = *sharedGameState;

  // Error checking for different commands
  // Also builds the request to send to the server
  if (strcmp(argv[1], "move") == 0) {
    if(argc != 4)
      error();
    if(*argv[2] < ZERO_ASCII || *argv[2] > FOUR_ASCII)
      error();
    if(*argv[3] < ZERO_ASCII || *argv[3] > FOUR_ASCII)
       error();
  } else if (strcmp(argv[1], "undo") == 0) {
    if(argc != 2)
      error();
  } else if (strcmp(argv[1], "report") == 0) {
    if(argc != 2)
      error();
  } else {
    error();
  }

  // Determine the command, perform actions, and update response accordingly
  if (strcmp(argv[1], "move") == 0) {
    int row = *argv[2] - ZERO_ASCII;
    int column = *argv[3] - ZERO_ASCII;
    if (move(row, column)) {
      printf("success\n");
    }
    else {
      error();
    }
  } else if (strcmp(argv[1], "undo") == 0) {
    if (undo()) {
      printf("success\n");
    } else {
      error();
    }
  } else if (strcmp(argv[1], "report") == 0) {
    report();
  }

  // Copy values from local gameState to shared memory
  *sharedGameState = gameState;

  // Release this program's reference to the shared memory segment
  shmdt(sharedGameState);

  return 0;
}
