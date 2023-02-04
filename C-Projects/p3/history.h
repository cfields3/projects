/**
 * @file history.h
 * @author Christopher Fields (cwfields)
 * 
 * Header file for the history component of the Wordle
 * system. Contains a function that will update the scores,
 * stored in a text file, to give the guess frequencies of
 * recent Wordle games. Prints the recent guesses to stdout
 * after the file is updates.
 */

/**
 * Called after the user wins a game with the given
 * number of guesses. It reads the "scores.txt" file,
 * increments the count for the given number of guesses,
 * prints the score history, then saves it back to
 * "scores.txt."
 * 
 * @param guessCount the number of guesses for a particluar game
 */
void updateScore(int guessCount);
