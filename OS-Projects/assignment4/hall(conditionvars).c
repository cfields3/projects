/**
 * @file hall.c
 * @author Christopher Fields (cwfields)
 *
 * Code for CSC 246 Assignment 4, Problem 2. Allocates
 * a hall for different organizations using monitors
 * implemented with POSIX mutexes. Provides functions
 * for initializing the monitor, detroying the monitor,
 * allocating space for an orgaization in the hall, and
 * freeing space allocated for an organization from the
 * hall.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Lock for access to the monitor
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// Condition variable for blocking until there is space in the hall
pthread_cond_t spaceCond = PTHREAD_COND_INITIALIZER;

// Keeps track of hall usage in a dynamically allocated array of values
char *hall;
// Size of the hall
int size;

// Keeps track of the age of each thread
int *ages;
// Number of waiting threads
int numAges;

int waitingAge;

void initMonitor(int n) {
    hall = (char *) calloc(n, sizeof(int));
    size = n;
    ages = (int *) malloc(sizeof(int));
    numAges = 0;
}

void destroyMonitor() {
    free(hall);
}

/**
 * Helper function that searches for a space of
 * specified width in the hall.
 * 
 * @param width the width to search for in the hall
 * @return starting index for space found, or -1
*/
static int findSpace(int width) {
    int numContigRooms = 0;
    for (int i = 0; i < size; i++) {
        if (hall[i] == 0) {
            numContigRooms++;
        } else {
            numContigRooms = 0;
        }

        if (numContigRooms == width) // Check if we have a large enough space
            return i - numContigRooms + 1;
    }

    return -1; // Indicate that no space was found
}

/**
 * Generates an allocation report the hall as a sequence of
 * n characters, where n is the width of the hall. Each character
 * in the report shows the current state of one of the spaces, from
 * left to right. Free spaces are indicated with astericks and
 * allocated spaces are indicated with the first letter of the
 * organization currently using the space.
 * 
 * @return string of characters representing the current allocation string
*/
static char *generateAllocationReport() {
    char *allocationReport = malloc(size * sizeof(char) + 1);
    for (int i = 0; i < size; i++) {
        if (hall[i] == 0) {
            allocationReport[i] = '*';
        } else {
            allocationReport[i] = hall[i];
        }
    }
    allocationReport[size] = '\0'; // Add null terminator to string
    return allocationReport;
}

static int *addWaitingThread() {
    ages = realloc(ages, sizeof(int) * (numAges + 1));
    int ageIndex = numAges;
    ages[ageIndex] = 0;
    numAges++;

    return ages + ageIndex;
}

static void ageWaitingThreads() {
    for (int i = 0; i < numAges; i++) {
        if (ages[i] != -1)
            ages[i]++;
    }
}

static void removeWaitingThread(int *age) {
    int ageIndex = age - ages;
    for (int i = ageIndex; i < numAges - 1; i++) {
        ages[i] = ages[i + 1];
        numAges--;
    }
}

static bool compareAge(int age) {
    if (age > waitingAge + 100)
        return true;

    return false;
}

int allocateSpace(char const *name, int width) {
    pthread_mutex_lock(&lock);

    int start; // Index of allocation start for an organization
    bool waitMsgDisp = true; // Flag for whether to print wait message (prints once)

    int age;

    while ((start = findSpace(width)) == -1 || compareAge(age)) {
        if (waitMsgDisp) {
            printf("%s waiting: %s\n", name, generateAllocationReport());
            waitMsgDisp = false;
        }
        
        pthread_cond_wait(&spaceCond, &lock);
        // increment age of thread
        waitingAge = age;
    }

    // Set each allocated room to the first letter of the inputed name
    for (int i = start; i < start + width; i++)
        hall[i] = name[0];
    
    printf("%s allocated (%d): %s\n", name, *age, generateAllocationReport());

    removeWaitingThread(age);
    ageWaitingThreads();

    pthread_mutex_unlock(&lock);

    return start;
}

void freeSpace(char const *name, int start, int width) {
    pthread_mutex_lock(&lock);
    for (int i = start; i < start + width; i++)
        hall[i] = 0;
    
    printf("%s freed: %s\n", name, generateAllocationReport());

    pthread_cond_broadcast(&spaceCond);

    pthread_mutex_unlock(&lock);
}