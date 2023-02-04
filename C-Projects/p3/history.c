/**
 * @file history.c
 * @author Christopher Fields (cwfields)
 * 
 * Implementation of the history component that updates the
 * "scores.txt" file (or creates a new file, if none exists)
 * with frequencies of guesses of the past games. Will print
 * past guess frequencies to stdout after reading and updating
 * text file containing scores.
 */

#include <stdio.h>

/** The highest score range (or higher) tracked by the history component */
#define UPPER_SCORE 10

void updateScore(int guessCount)
{

    // Initialize scores array
    int scores[UPPER_SCORE];

    // Open "scores.txt" as a file stream for reading
    FILE *reader = fopen("scores.txt", "r");

    // Read past scores from given file "scores.txt"
    for (int i = 0; i < UPPER_SCORE; i++) {
        if (reader == NULL) {
            scores[i] = 0;
        } else {
            fscanf(reader, "%d", &scores[i]);
        }
    }

    if (reader != NULL) {
        // Close reader stream
        fclose(reader);
    }
    

    // Increment score according to guessCount
    if (guessCount >= UPPER_SCORE) {
        scores[UPPER_SCORE - 1]++;
    } else {
        scores[guessCount - 1]++;
    }

    // Print score history to stdout
    for (int i = 0; i < UPPER_SCORE; i++) {
        printf("%2d%c : %4d\n", i + 1, (i == UPPER_SCORE - 1) ? '+' : ' ', scores[i]);
    }

    // Open "scores.txt" as a file stream for writing
    FILE *writer = fopen("scores.txt", "w");

    // Write updated scores to to file stream
    for (int i = 0; i < UPPER_SCORE; i++) {
        if (i != 0)
            fprintf(writer, " ");
        fprintf(writer, "%d", scores[i]);
    }

    fprintf(writer, "\n");

    fclose(writer);

}
