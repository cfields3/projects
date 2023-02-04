/**
 * @file database.c
 * @author Christopher Fields (cwfields)
 *
 * Implementation for the database component of the Agency Database Management
 * system. Provides functions that allow the client to make a database, free
 * all components of a database, add Employees into a database from a file, and
 * list specified Employees in a specified sorted order from a database.
 */

#include "database.h"
#include "input.h"
#include "string.h"

#include <stdlib.h>
#include <stdio.h>

/** Initial capacity for a Database */
#define INIT_CAPACITY 5
/** Number of fields to read from Employee in file (id, firstName, lastName, skill) */
#define NUM_FIELDS 4
/** Multiple for resizing the line string size when at capacity */
#define RESIZE_MULTIPLE 2

Database *makeDatabase()
{
    Database *database = (Database *) malloc(sizeof(Database)); // Allocate space for database
    database->list = (Employee **) malloc(sizeof(Employee *) * INIT_CAPACITY); // Set list to new Employee pointer list
    database->capacity = INIT_CAPACITY; // Set initial capacity
    database->count = 0; // Set initial number of Employees in database

    return database;
}

void freeDatabase(Database *database)
{
    for (int i = 0; i < database->count; i++) {
        free(database->list[i]); // Free each Employee in the Database
    }

    free(database->list); // Free the list of Employees
    free(database); // Free the Database itself
}

void readEmployees(char const *filename, Database *database)
{
    FILE *fp = fopen(filename, "r");

    if (!fp) { // If the file can't be open (is NULL)
        fprintf(stderr, "Can't open file: %s\n", filename);
        freeDatabase(database);
        exit(EXIT_FAILURE);
    }

    char *currentLine = readLine(fp);
    while (currentLine != NULL) {
        Employee *employee = (Employee *) malloc(sizeof(Employee));

        int matches = sscanf(currentLine, "%s%s%s%s", employee->id, employee->firstName, employee->lastName,
                employee->skill);

        free(currentLine); // Free memory allocated for currentLine

        if (matches != NUM_FIELDS) { // If the line did not have all of the required fields for an Employee
            fprintf(stderr, "Invalid employee file: %s\n", filename);
            fclose(fp);
            freeDatabase(database);
            free(employee);
            exit(EXIT_FAILURE);
        }     

        strcpy(employee->assignment, "Available"); // Set assignment to be avaiable for all new Employees from file

        // Check if the Employee id, firstName, lastName, or skill are too long
        if (strlen(employee->id) != ID_MAX_LENGTH || strlen(employee->firstName) > FIRST_NAME_MAX_LENGTH ||
                strlen(employee->lastName) > LAST_NAME_MAX_LENGTH || strlen(employee->skill) > SKILL_MAX_LENGTH) {
            fprintf(stderr, "Invalid employee file: %s\n", filename);
            fclose(fp);
            freeDatabase(database);
            free(employee);
            exit(EXIT_FAILURE);
        }

        // Check to ensure the id of the Employee attempting to add is not already an existing id
        for (int i = 0; i < database->count; i++) {
            if (strcmp(database->list[i]->id, employee->id) == 0) {
                fprintf(stderr, "Invalid employee file: %s\n", filename);
                fclose(fp);
                freeDatabase(database);
                free(employee);
                exit(EXIT_FAILURE);
            }
        }

        // Add the Employee pointer to the Database
        if (database->count >= database->capacity) {
            // Grow the array if at capacity
            database->capacity *= RESIZE_MULTIPLE; // Double capacity field
            database->list = (Employee **) realloc(database->list, sizeof(Employee *) * database->capacity);
        }
        database->list[database->count] = employee;
        database->count++;

        currentLine = readLine(fp); // Read the next line of the file (or NULL if there is no next line)
    }

    fclose(fp);
}

void listEmployees(Database *database, int (*compare)(void const *va, void const *vb), 
    bool (*test)(Employee const *emp, char const *str), char const *str)
{
    // Sort the Employees in the inputted Datebase based on the inputted comparator function pointer
    qsort(database->list, database->count, sizeof(database->list[0]), compare);

    // Print the Employees in the inputted Database, using the inputted test function to decide which to print
    printf("ID   First Name      Last Name       Skill           Assignment\n"); // Print header
    for (int i = 0; i < database->count; i++) {
        Employee *employee = database->list[i];
        if (test(employee, str)) { // If the Employee should be printed, print in specified format in table
            printf("%-4s ", employee->id);
            printf("%-15s ", employee->firstName);
            printf("%-15s ", employee->lastName);
            printf("%-15s ", employee->skill);
            printf("%-20s", employee->assignment);
            printf("\n");
        }
    }
}