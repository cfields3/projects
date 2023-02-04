/**
 * @file lexicon.c
 * @author Christopher Fields (cwfields)
 * 
 * Implementation for the component that implements
 * the word list. Includes functions needed for
 * behavior in reading a given word list, choosing
 * a word from the word list, and checking if a word
 * is in a given word list.
 */

#include "lexicon.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "io.h"

/** Large prime multiplier used to choose a word pseudo-randomly. */
#define MULTIPLIER 4611686018453

/** Divisor used to divide numbers in half */
#define HALF 2

/** Array of strings storing the word list */
static char wordList[WORD_LIMIT][WORD_LEN + 1];

/** Number of strings stored in the word list */
static int numWords;

void readWords(char const filename[])
{
    FILE *stream = fopen(filename, "r"); // Open file input stream with filename

    // Check if word list file is invalid
    if (!stream) {
        fprintf(stderr, "Can't open word list: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int index = 0;
    // Iterate through the file until there are no more lines
    while (readLine(stream, wordList[index], WORD_LEN)) {
        if (index == WORD_LIMIT) {
            fprintf(stderr, "Invalid word file\n");
            exit(EXIT_FAILURE);
        }
        numWords++;
        index++; 
    }

    // Invalid - No words in word file
    if (numWords == 0) {
        fprintf(stderr, "Invalid word file\n");
        exit(EXIT_FAILURE);
    }
    
}

void chooseWord(long seed, char word[])
{
    strcpy(word, wordList[(seed % numWords) * MULTIPLIER % numWords]);
}

/**
 * Helper method that implements binary search on an inputed list in order
 * to help support more efficient calls to inList() function. Recursively
 * calls itself in order to complete binary search on subsections of the
 * array. Returns -1 if element is not in list. Note: the binary search
 * algorithm used in this function is based on algorithm implementations
 * found in CSC 316 resources.
 * 
 * @param list the list to search for an element in
 * @param min the minimum index to perform search
 * @param max the maximum index to perform search
 * @param word the word to search for
 * @return int the index of the desired word, or -1 if not in list
 */
static int binarySearch(char list[WORD_LIMIT][WORD_LEN + 1], int min, int max, char const word[WORD_LEN + 1])
{
    if (min > max) {
        return -1;
    }

    int mid = (max + min) / HALF;

    if (strcmp(list[mid], word) == 0) {
        return mid;
    } else if (strcmp(list[mid], word) > 0) {
        return binarySearch(list, min, mid - 1, word);
    } else {
        return binarySearch(list, mid + 1, max, word);
    }
}

bool inList(char const word[])
{
    if (binarySearch(wordList, 0, numWords, word) >= 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * Helper function for mergeSort to sort different parts of
 * the original array recursively. Merges the left and right
 * subarrays that are inputted into original array. Also inputs
 * size of both subarrays so that the function can iterate and
 * check indices more easily. Note: the merge algorithm used 
 * in this function is based on algorithm implementations found
 * in CSC 316 resources.
 * 
 * @param leftSize size of the left subarray inputted
 * @param left the left subarray to merge
 * @param rightSize size of the right subarray inputted
 * @param right the right subarray to merge
 * @param size the size of the array to merge subarrays into
 * @param list the array to merge subarrays into
 */
static void merge(int leftSize, char left[leftSize][WORD_LEN + 1], int rightSize, char right[rightSize][WORD_LEN + 1], int size, char list[size][WORD_LEN + 1])
{
    int rightIndex = 0;
    int leftIndex = 0;

    while (leftIndex + rightIndex < size) {
        if (rightIndex ==  rightSize || (leftIndex < leftSize && strcmp(left[leftIndex], right[rightIndex]) < 0)) {
            strcpy(list[leftIndex + rightIndex], left[leftIndex]);
            leftIndex++;
        } else {
            strcpy(list[leftIndex + rightIndex], right[rightIndex]);
            rightIndex++;
        }
    }
}

/**
 * Helper function to perform merge sort on an inputted array of
 * inputted size. Split elements in left and right subarrays
 * and utilizes the merge() helper function to sort subarrays
 * back into the original array, recursively. Note: the merge
 * sort algorithm used in this function is based on algorithm
 * implementations found in CSC 316 resources.
 * 
 * @param size the size of the list to sort
 * @param list the inputted list to sort
 */
static void mergeSort(int size, char list[size][WORD_LEN + 1])
{
    if (size < HALF) {
        return;
    }
    
    int mid = size / HALF;

    char left[size/HALF + 1][WORD_LEN + 1];
    char right[size/HALF + 1][WORD_LEN + 1];

    int leftSize = 0;
    int rightSize = 0;

    for (int i = 0; i < mid; i++) {
        strcpy(left[i], list[i]);
        leftSize++;
    }

    for (int i = 0; i < size - mid; i++) {
        strcpy(right[i], list[mid + i]);
        rightSize++;
    }

    mergeSort(leftSize, left);
    mergeSort(rightSize, right);

    merge(leftSize, left, rightSize, right, size, list);
}

void sort()
{
    // Sorts the wordList using merge sort algorithm
    mergeSort(numWords, wordList);

    // Check for duplicates in sorted list (since list is sorted can check neighboring indices)
    for (int i = 0; i < numWords - 1; i++) {
        if (strcmp(wordList[i], wordList[i + 1]) == 0) {
            fprintf(stderr, "Invalid word file\n");
            exit(EXIT_FAILURE);
        }
    }
}
