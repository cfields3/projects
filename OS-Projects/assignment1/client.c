/**
 * @file client.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 1, Problem 4. CLient
 * code that communicates with the user by receiving
 * a command as command-line arguments and sends a
 * message to the server requesting to change and
 * receive information about a "lights out" game.
 */

#include "common.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

/** Value of zero in ASCII */
#define ZERO_ASCII 48
/** Value of four in ASCII */
#define FOUR_ASCII 52

// Print out an error message and exit.
static void fail(char const *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
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
 * Starting point of the client program.
 * Opens the message queues and sends messages
 * to the queues using user input, then
 * prints out information based on results
 * sent from the server.
 * 
 * @param argc number of command-line arguments
 * @param argv list of command-line arguments
 * @return exit status of program
 */
int main(int argc, char *argv[]) {
    // Open queues created in server.c
    mqd_t serverQueue = mq_open(SERVER_QUEUE, O_WRONLY);
    mqd_t clientQueue = mq_open(CLIENT_QUEUE, O_RDONLY);

    // Queue opening error checking
    if (serverQueue == -1 || clientQueue == -1)
        fail("Can't create the needed message queues");

    // Create buffers for sending and receiving from server
    char request[MESSAGE_LIMIT + 1] = "";
    char response[MESSAGE_LIMIT + 1] = "";

    // Error checking for different commands
    // Also builds the request to send to the server
    if (strcmp(argv[1], "move") == 0) {
        if(argc != 4)
            error();
        if(*argv[2] < ZERO_ASCII || *argv[2] > FOUR_ASCII)
            error();
        if(*argv[3] < ZERO_ASCII || *argv[3] > FOUR_ASCII)
            error();

        strcat(request, "move ");
        strcat(request, argv[2]);
        strcat(request, " ");
        strcat(request, argv[3]);
    } else if (strcmp(argv[1], "undo") == 0) {
        if(argc != 2)
            error();
        strcat(request, "undo");
    } else if (strcmp(argv[1], "report") == 0) {
        if(argc != 2)
            error();
        strcat(request, "report");
    } else {
        error();
    }

    // Send a request to perform a command to the server
    mq_send(serverQueue, request, strlen(request), 0);

    // Receive output from the server to print to the user
    mq_receive(clientQueue, response, sizeof(response), NULL);

    // Print server's response to the user
    printf(response);

    // Exit successfully, unless there was an error
    if(strcmp(response, "error\n") == 0) {
        return(1);
    } else {
        return(0);
    }

}
