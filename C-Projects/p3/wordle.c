/**
 * @file wordle.c
 * @author Christopher Fields (cwfields)
 *
 * Main part of the Wordle program for interacting with
 * the user and allowing for them to play the game of Wordle.
 * Allows the user to input guesses for a word and outputs guesses
 * based on a word generated from a given list and seed. Outputs
 * color-coded feedback to the user and recent scores when user
 * has correctly guessed word.
 */

#include "lexicon.h"
#include "io.h"
#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/** Command-line argument index of the filename */
#define FILENAME_ARGUMENT 1

/** Command-line argument index of the seed */
#define SEED_ARGUMENT 2

/** Constant to represent that output is printing in default color */
#define COLOR_DEFAULT 97

/** Constant to represent that output is printing in yellow color */
#define COLOR_YELLOW 98

/** Constant to represent that output is printing in green color */
#define COLOR_GREEN 99

/**
 * Helper method to read the next index of a letter in a word,
 * using the character to search for in the word and an index
 * to start iterating through the word on.
 * 
 * @param word the word to check if the character is in
 * @param character the character to check in the word
 * @param start index to start iterating through the word on
 * @return int index of the character in the word, or -1 if doesn't exist
 */
static int nextIndexIn(char word[], char character, int start)
{
    for (int i = start; i < WORD_LEN; i++) {
        if (character == word[i]) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char *argv[])
{   
    // Initialize variables
    long seed;
    char *filename = argv[1];
    char word[WORD_LEN + 1];

    // Read seed based on command-line arguments
    if (argc == FILENAME_ARGUMENT + 1) { // Only includes up to filename argument
        seed = time(NULL);
    } else if (argc == SEED_ARGUMENT + 1) { // Includes up to seed argument
        seed = atol(argv[SEED_ARGUMENT]);
    } else {
        fprintf(stderr, "usage: wordle <word-list-file> [seed number]\n");
        return (EXIT_FAILURE);
    }

    // Read words from file and store them in global variable wordList
    readWords(filename);

    // Choose a word from wordList using the seed and store it in word
    chooseWord(seed, word);

    // Sort the wordList using mergesort & check for duplicates
    sort();

    // Initialize number of guesses it takes to guess word
    int guesses = 0;

    // Create a string to store value read from user
    char guess[WORD_LEN + 1];

    // Initialize the color to be default (from program start)
    int color = COLOR_DEFAULT;

    // Iterate while the guess is not the word
    while (strcmp(guess, word) != 0) {
        
        int matches = scanf("%7s", guess);

        // If the user guesses the word, break out of the loop
        if (strcmp(guess, word) == 0) {
            guesses++;
            break;
        }

        // If the word stored is too long, read the rest of input
        // and notify user guess was invalid
        if (strlen(guess) > WORD_LEN) {
            // If there is actually more input to read in the line
            if (strlen(guess) > WORD_LEN + 1) {
                scanf("%*s");
            }
            printf("Invalid guess\n");
            continue;
        }


        // If the user chooses to quit or no more strings are found in input
        if (strcmp(guess, "quit") == 0 || matches != 1) {
            printf("The word was \"%s\"\n", word);
            return EXIT_SUCCESS;
        }

        // Stores whether an index of a letter from the word has been guessed
        int counts[WORD_LEN] = {1, 1, 1, 1, 1};
        
        // Immediately print error message is word is not in list, print
        // color-coded output to stdout otherwise.
        if (!inList(guess)) {
            printf("Invalid guess\n");
        } else {

            // Decrement the counts for all green characters
            for (int i = 0; i < WORD_LEN; i++) {
                if (guess[i] == word[i]) {
                    counts[i]--;
                }
            }

            // Iterate through all characters in word, printing in appropriate color
            for (int i = 0; i < WORD_LEN; i++) {
                if (guess[i] == word[i]) { // Letter in exact same place in actual word
                    if (color != COLOR_GREEN)
                        colorGreen();
                        color = COLOR_GREEN;
                    putchar(guess[i]);
                } else {
                    int index = nextIndexIn(word, guess[i], 0);
                    while (index >= 0) {
                        if (counts[index] != 0) { // Letter somewhere in word, not already counted
                            if (color != COLOR_YELLOW)
                                colorYellow();
                                color = COLOR_YELLOW;
                            putchar(guess[i]);
                            counts[index]--; // Don't count this letter again
                            break;
                        }
                        index = nextIndexIn(word, guess[i], index + 1);
                    }
                    if (index == -1) { // Letter not in word or already counted
                        if (color != COLOR_DEFAULT)
                            colorDefault();
                            color = COLOR_DEFAULT;
                        putchar(guess[i]);
                    }
                } 
            }

            // Set color back to default and print newline
            if (color != COLOR_DEFAULT)    
                colorDefault();
                color = COLOR_DEFAULT;
            putchar('\n');
            
            guesses++;
        }
    }

    // Print number of guesses Wordle was solved in
    printf("Solved in %d %s\n", guesses, guesses == 1 ? "guess" : "guesses");

    // Updated "scores.txt" file using history component and print past guess frequencies
    updateScore(guesses);

}
