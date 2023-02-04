/**
 * @file stash.c
 * @author Christopher Fields (cwfields)
 * 
 * Code for CSC 246 Assignment 0, Question 4. Simulates
 * a shell on a Unix system. This shell: stash, or "simple
 * toy assignment shell," has a few features of a typical
 * shell program (both built-in & external commands).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

// Constant representing maximum character length for a shell command
#define MAX_COMMAND_LENGTH 1024

/**
 * Takes a user command (line) as input, breaks the line into
 * individual words, adds null termination between the words, and
 * fills in a pointer in words array to point to the start of each
 * word. Returns the number of words found in the given line.
 * 
 * @param line user command inputted
 * @param words array to contain references to each word in the input line
 * @return the number of words found in the inputted line
 */
int parseCommand(char *line, char *words[])
{
    int idx = 0;
    int numWords = 1;

    // Replace all spaces in the input with null terminators
    while (line[idx] != '\n') {
        if (line[idx] == ' ') {
            line[idx] = '\0';
            numWords++;
        }
        idx++;
    }

    // Replace index after last character with null terminator
    line[idx] = '\0';

    // Ensure that words fits the amount of inputted words
    words = realloc(words, sizeof(char *) * numWords);

    // Add pointers to each word in line into the words array
    idx = 0;
    for(int i = 0; i < numWords; i++) {
        words[i] = &line[idx];
        while (line[idx] != '\0') {
            idx++;
        }
        idx++;
    }

    return numWords;
}

/**
 * Performs the built-in exit command. Inputs a list
 * of pointers to words in the user's command and a 
 * count of the number of words in the array.
 * 
 * @param words list of pointers to words in command
 * @param count number of words in the command
 */
void runExit(char *words[], int count)
{
    // Ensure that exit command is valid with exactly two arguments
    if (count == 2) {
        int status;
        // Ensure that the value of the first argument is an integer
        if (sscanf(words[1], "%d", &status) == 1) {
            free(words);
            exit(status);
        } else {
            printf("Invalid command\n");
        }
    }
    else {
        printf("Invalid command\n");
    }
}

/**
 * Performs the build-in cd command. Inputs a list
 * of pointers to words in the user's command and a
 * count of the number of words in the array.
 * 
 * @param words list of pointers to words in command
 * @param count number of words in the command
 */
void runCd(char *words[], int count)
{
    // Ensure that cd command is valid with exactly two arguments
    if (count == 2) {
        // Run cd with chdir; results in an error by returning -1
        if (chdir(words[1]) == -1) {
            printf("Invalid command\n");
        }
    } else {
        printf("Invalid command\n");
    }
}

/**
 * Runs a (non built-in) command by creating a
 * child process and having it call execvp()
 * to run the given command. Inputs a list of pointers
 * to words in the user's command and a count of
 * the number of words in the array.
 * 
 * @param words list of pointers to words in command
 * @param count number of words in the command
 */
void runCommand(char *words[], int count)
{
    // Initialize running command as background processes to false
    bool background = false;

    // Check if & flag for background processes is present
    if(strcmp(words[count - 1], "&") == 0) {
        background = true;
        count--;
    }

    // Create a new child process, record process ID
    int pid = fork();

    // Check for the child process
    if (pid == 0) {
        words = realloc(words, sizeof(char *) * (count + 1));
        words[count] = NULL;

        // Execute program specified using execvp() or exit with error upon returning
        if (execvp(words[0], words) == -1) {
            printf("Can't run command %s\n", words[0]);
            exit(EXIT_FAILURE);
        }
    } else {
        // If you run a non-background process
        if(!background) {
            int pidDone = wait(NULL);
            if (pidDone != pid) {
                while (pidDone != pid) {
                    printf("[%d done]\n", pidDone);
                    pidDone = wait(NULL);
                }
            }
        } else { // Running a background process
            printf("[%d]\n", pid);
        }
    }
}

/**
 * Program starting point. Prompts the user for
 * and parses a command. Then, runs either a
 * built in command or an external command using
 * other functions.
 * 
 * @param argc number of command-line arguments
 * @param argv list of command-line arguments
 * @return program exit status
 */
int main(int argc, char const *argv[])
{
    // Continue to reprompt forever (unless exit function is run)
    while(true) {
        // Create variables to store the command input line and pointers
        // to each word in that command line
        char line[MAX_COMMAND_LENGTH + 1];
        char **words = malloc(sizeof(char *));

        // Prompt the user to enter a command
        printf("stash> ");
        fgets(line, MAX_COMMAND_LENGTH, stdin);

        // Parse the inputted command and return the number of words in it
        int numWords = parseCommand(line, words);

        // Find the corresponding built-in command, and run input as an external
        // command if input does not match a built-in command
        if(strcmp(words[0], "cd") == 0) {
            runCd(words, numWords);
        } else if(strcmp(words[0], "exit") == 0) {
            runExit(words, numWords);
        } else if(strcmp(words[0], "") == 0) {
            // Ignore an empty line entered & continue to reprompt statement
        } else {
            runCommand(words, numWords);
        }
    }
}
