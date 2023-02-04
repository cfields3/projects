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
#include <string.h>

// Lock for access to the monitor
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// Condition variable for blocking until there is space in the hall
pthread_cond_t spaceCond = PTHREAD_COND_INITIALIZER;

// Keeps track of hall usage in a dynamically allocated array of values
static char *hall;
// Size of the hall
static int size;

// Represents an organization as used for waiting threads in the monitor
typedef struct OrganizationStruct {
  char *name;
  int age;
  struct OrganizationStruct *next;
} Organization;

// Keeps track of the age of each thread
static Organization *ages;
// Number of waiting threads
static int numAges;

void initMonitor(int n) {
    hall = (char *) calloc(n, sizeof(int));
    size = n;
    ages = NULL;
    numAges = 0;
}

void destroyMonitor() {
    free(hall);
    
    Organization *current = ages;
    while (current != NULL) {
        Organization *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
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

/**
 * Helper function to adds a waiting thread to the list of waiting
 * Organizations.
 * 
 * @param name the name of the thread to add
 * @return the address of the new Organization added
*/
static Organization *addWaitingThread(char const *name) {
    Organization *newOrganization = (Organization *) malloc(sizeof(Organization));
    newOrganization->name = malloc(sizeof(name));
    strcpy(newOrganization->name, name);
    newOrganization->age = 0;
    newOrganization->next = ages;

    ages = newOrganization;
    numAges++;

    return newOrganization;
}

/**
 * Helper function to increment the age of all Organizations
 * in the linked list of ages.
*/
static void ageWaitingThreads() {
    Organization *current = ages;
    while (current != NULL) {
        current->age++;
        current = current->next;
    }
}

/**
 * Helper function to remove an Organization from the list
 * of waiting threads.
 * 
 * @param name the name of the Organization to remove
*/
static void removeWaitingThread(char const *name) {
    Organization *current = ages;
    while (current->next != NULL) {
        if (strcmp(current->next->name, name) == 0) {
            free(current->next);
            current->next = current->next->next;
            numAges--;
            break;
        }
        current = current->next;
    }

    // Base case for if the first organization is the one to remove
    if (ages != NULL && strcmp(ages->name, name) == 0) {
        Organization *nextOrg = ages->next;
        free(ages);
        ages = nextOrg;
        numAges--;
    }
}

/**
 * Helper function to search the list of waiting threads
 * to find if any have an age greater than 100 more than
 * the inputted age.
 * 
 * @param threadAge the age of the thread we are comparing
*/
static bool findOlderThread(int threadAge) {
    Organization *current = ages;
    while (current != NULL) {
        if (current->age > threadAge + 100)  {
            pthread_cond_signal(&spaceCond);
            return true; // Found an older thread
        }
        current = current->next;
    }
    // No significantly older threads were found
    return false;
}

int allocateSpace(char const *name, int width) {
    pthread_mutex_lock(&lock);

    int start; // Index of allocation start for an organization
    bool waitMsgDisp = true; // Flag for whether to print wait message (prints once)

    // Add waiting thread to list of waiting organizations
    Organization *waitingThread = addWaitingThread(name);

    // Check if the thread needs to wait for space to be available (or give priority to an older thread)
    while ((start = findSpace(width)) == -1 || findOlderThread(waitingThread->age)) {
        if (waitMsgDisp) {
            char *allocationReport = generateAllocationReport();
            printf("%s waiting: %s\n", name, allocationReport);
            free(allocationReport);
            waitMsgDisp = false;
        }
        
        pthread_cond_wait(&spaceCond, &lock);
    }

    // Set each allocated room to the first letter of the inputed name
    for (int i = start; i < start + width; i++)
        hall[i] = name[0];

    // Print out a report that the organization was allocated in the hall
    char *allocationReport = generateAllocationReport();
    printf("%s allocated (%d): %s\n", name, waitingThread->age, allocationReport);
    free(allocationReport);

    // Thread has been allocated. Remove it from the list of waiting threads and age any waiting threads
    removeWaitingThread(name);
    ageWaitingThreads();

    pthread_mutex_unlock(&lock);

    return start;
}

void freeSpace(char const *name, int start, int width) {
    pthread_mutex_lock(&lock);

    // Reset the values in the hall to represent free spaces (0)
    for (int i = start; i < start + width; i++)
        hall[i] = 0;
    
    // Print out a report that the organization was freed from the hall
    char *allocationReport = generateAllocationReport();
    printf("%s freed: %s\n", name, allocationReport);
    free(allocationReport);

    pthread_cond_broadcast(&spaceCond);

    pthread_mutex_unlock(&lock);
}