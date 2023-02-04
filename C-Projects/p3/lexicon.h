/**
 * @file lexicon.h
 * @author Christopher Fields (cwfields)
 * 
 * Header file for the component that implements
 * the word list. Includes function prototypes for
 * behavior in reading a given word list, choosing
 * a word from the word list, and checking if a word
 * is in a given word list.
 */

#include <stdbool.h>

/** Maximum lengh of a word on the word list. */
#define WORD_LEN 5

/** Maximum number of words on the word list. */
#define WORD_LIMIT 100000

/**
 * Reads the word list from the file with the given
 * name. Will also detect errors in the word list file
 * and print an error message accordingly.
 * 
 * @param filename the name of the file with the word list
 */
void readWords(char const filename[]);

/**
 * Chooses a word from the current word list using the
 * given seed. Copies the chosen word to the word[] array.
 * 
 * @param seed the seed to use in choosing a word
 * @param word the character array to copy word into
 */
void chooseWord(long seed, char word[]);

/**
 * Check the given word to see if it's in the word list,
 * returning true if so and false otherwise.
 * 
 * @param word the word to check for existence in word list
 */
bool inList(char const word[]);

/**
 * Sorts the wordList global variable using the mergeSort() helper
 * function. Also checks for duplicates in the resulting array after
 * it is in sorted order. 
 */
void sort();
