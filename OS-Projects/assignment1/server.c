/**
 * @file server.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 1, Problem 4. Server
 * code that manages the representation of a "lights
 * out" game. Initializes a board inputted and waits
 * for commands from the user to change that board
 * (move, report, & undo). Runs until the user provides
 * a keyboard interrupt, at which case it prints out
 * the final state of the board before exiting.
 */

#include "common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define MAX_COMMAND 6

/** Representation of the current board */
char currentBoard[GRID_SIZE][GRID_SIZE];

/** Representation of board before last move */
char previousBoard[GRID_SIZE][GRID_SIZE];

/** Flag representing whether or not the client can undo */
bool canUndo = false;

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

// Flag for telling the server to stop running because of a sigint.
// This is safer than trying to print in the signal handler.
static int running = 1;

/**
 * Inverts an inputted light on the current board.
 * 
 * @param row index of the row
 * @param column index of the column
 */
void invertLight(int row, int column) {
  if(currentBoard[row][column] == '.') {
    currentBoard[row][column] = '*';
  } else {
    currentBoard[row][column] = '.';
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
  memcpy(previousBoard, currentBoard, sizeof(previousBoard));
  invertLight(row, column);
  if(row != 0)
    invertLight(row - 1, column);
  if(column != 0)
    invertLight(row, column - 1);
  if(row != GRID_SIZE - 1)
    invertLight(row + 1, column);
  if(column != GRID_SIZE - 1)
    invertLight(row, column + 1);
  canUndo = true;
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
  if(canUndo) {
    memcpy(currentBoard, previousBoard, sizeof(currentBoard));
    canUndo = false;
    return true;
  } else {
    return false;
  }
}

/**
 * Reports the current state of the board
 * as a string of characters into the 
 * parameter inputted.
 * 
 * @param output string to store board state
 * @return whether the report was successful
 */
bool report(char *output) {
  int idx = 0;
  for(int row = 0; row < 5; row++) {
    for(int column = 0; column < GRID_SIZE; column++) {
      output[idx] = currentBoard[row][column];
      idx++;
    }
    output[idx] = '\n';
    idx++;
  }
  output[idx] = '\0';
  return true;
}

/**
 * Will handle an alert of SIGINT 
 * given by a keyboard interrupt
 * by reporting the board state
 * and exiting.
 * 
 * @param sig error signal
 */
void exitHandler(int sig) {
  printf("\n");
  char output[MESSAGE_LIMIT + 1] = "";
  report(output);
  printf(output);
  running = 0;
}

/**
 * Starting point of the server program.
 * Performs actions based on messages
 * provided by the client and returns
 * message back to the client based on
 * the command executed.
 * 
 * @param argc number of command-line arguments
 * @param argv list of command-line arguments
 * @return program exit status
 */
int main( int argc, char *argv[] ) {
  // Struct to handle alerts
  struct sigaction act;
  act.sa_handler = exitHandler;
  sigemptyset(&(act.sa_mask));
  act.sa_flags = 0;

  // Call the struct on keyboard interrupt
  sigaction(SIGINT, &act, 0);

  // Ensure that there are exactly to CL arguments
  if(argc != 2)
    fail("usage: server <board-file>");

  // Open the input file for the board starting state
  char *filename = argv[1];
  FILE *input = fopen(filename, "r");

  if(!input) {
    fprintf(stderr, "Invalid input file: %s\n", filename);
    exit(1);
  }

  // Initialize the board using the inputted file
  for(int row = 0; row < 5; row++) {
    for(int column = 0; column < GRID_SIZE; column++) {
      char light = fgetc(input);
      if (light != '*' && light != '.') {
        fprintf(stderr, "Invalid input file: %s\n", filename);
        exit(1);
      } else {
        currentBoard[row][column] = light;
      }
    }
    if(fgetc(input) != '\n') {
      fprintf(stderr, "Invalid input file: %s\n", filename);
      exit(1);
    }
  }

  fclose(input);

  // Remove both queues, in case, last time, this program terminated
  // abnormally with some queued messages still queued.
  mq_unlink( SERVER_QUEUE );
  mq_unlink( CLIENT_QUEUE );

  // Prepare structure indicating maximum queue and message sizes.
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 1;
  attr.mq_msgsize = MESSAGE_LIMIT;

  // Make both the server and client message queues.
  mqd_t serverQueue = mq_open( SERVER_QUEUE, O_RDONLY | O_CREAT, 0600, &attr );
  mqd_t clientQueue = mq_open( CLIENT_QUEUE, O_WRONLY | O_CREAT, 0600, &attr );
  if ( serverQueue == -1 || clientQueue == -1 )
    fail( "Can't create the needed message queues" );

  // Repeatedly read and process client messages.
  while ( running ) {
    // Create buffers for the requests and responses to the client
    char request[MESSAGE_LIMIT + 1] = "";
    char response[MESSAGE_LIMIT + 1] = "";

    // Wait from a message from the client, store it in request
    mq_receive(serverQueue, request, sizeof(request), NULL);

    // Store the command, row, and column using the request buffer
    char command[MAX_COMMAND];
    int row;
    int column;
    sscanf(request, "%s %d %d", command, &row, &column);

    // Determine the command, perform actions, and update response accordingly
    if(strcmp(command, "move") == 0) {
      if(move(row, column)) {
        strcpy(response, "success\n");
      } else {
        strcpy(response, "error\n");
      }
        
    } else if(strcmp(command, "undo") == 0) {
      if(undo()) {
        strcpy(response, "success\n");
      } else {
        strcpy(response, "error\n");
      }
    } else if(strcmp(command, "report") == 0) {
      report(response);

    }

    // Send a response back to the client      
    mq_send(clientQueue, response, strlen(response), 0);


  }

  // Close our two message queues (and delete them).
  mq_close( clientQueue );
  mq_close( serverQueue );

  mq_unlink( SERVER_QUEUE );
  mq_unlink( CLIENT_QUEUE );

  return 0;
}
