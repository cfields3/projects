/**
 * @file agency.c
 * @author Christopher Fields (cwfields)
 *
 * Main part of the Temp Agency program for maintaining a
 * Database of Employees. Allows the user to interact with a
 * Employee Database system by entering commands through the
 * command-line, including list, assign, unassign, and quit.
 * Also allows the user to input Employees to use in the program
 * from a file using one or more command-line arguments.
 */

#include "database.h"
#include "input.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/** Maximum length of a command */
#define COMMAND_MAX_LENGTH 8
/** Maximum length of the first parameter of a command */
#define PARAM1_MAX_LENGTH 10
/** Maximum length of the second parameter of a command */
#define PARAM2_MAX_LENGTH 20
/** Number of arguments necessary in listing all Employees */
#define LIST_ALL_ARGS 1
/** Number of arguments necessary in listing Employees with filter */
#define LIST_FILTERED_ARGS 3
/** Number of arguments necessary in assigning an Employee */
#define ASSIGN_ARGS 3
/** Number of arguments necessary in unassigning an Employee */
#define UNASSIGN_ARGS 2
/** Number of argumentss necessary in quitting the program */
#define QUIT_ARGS 1

/**
 * Helper function/"comparator" that will compare inputed items
 * by id, which must of type struct Employee. Returns a negative
 * value if the first parameter should be ordered first, a positive
 * value if the second parameter should be ordered first, and zero
 * if the ids are lexographically identical.
 * 
 * @param va pointer to the first Employee pointer to compare
 * @param vb pointer to the second Employee pointer to compare
 * @return int the "comparator" value representing sorting order
 */
static int compareID(const void *va, const void *vb)
{
    // Translate inputed parameters into Employee type
    Employee *employeeA = *(Employee**)va;
    Employee *employeeB = *(Employee**)vb;

    // Compare the ids of the Employees
    return strcmp(employeeA->id, employeeB->id);
}

/**
 * Helper function/"comparator" that will compare inputed items
 * by skill, which must of type struct Employee. Returns a negative
 * value if the first parameter should be ordered first, a positive
 * value if the second parameter should be ordered first, and zero
 * if the skills are lexographically identical.
 * 
 * @param va pointer to the first Employee pointer to compare
 * @param vb pointer to the second Employee pointer to compare
 * @return int the "comparator" value representing sorting order
 */
static int compareSkill(const void *va, const void *vb)
{
    // Translate inputed parameters into Employee type
    Employee *employeeA = *(Employee**)va;
    Employee *employeeB = *(Employee**)vb;

    // Compare the skills of the Employees
    if (strcmp(employeeA->skill, employeeB->skill) == 0) {
        return strcmp(employeeA->id, employeeB->id);
    } else {
        return strcmp(employeeA->skill, employeeB->skill);
    }
}

/**
 * Helper function to test whether an Employee will be included
 * in a print output called by the program. Will always return
 * true for any input.
 * 
 * @param emp Employee to test
 * @param str string to compare with, should input NULL
 * @return true for all calls of the function
 */
static bool testAll(Employee const *emp, char const *str)
{
    return true;
}

/**
 * Helper function to test whether an Employee will be included
 * in a print output called by the program. Will return whether
 * the Employee's skill matches the inputed string, returning true
 * if so and false if not.
 * 
 * @param emp Employee to test for matching skill
 * @param str string to compare with as matching skill
 * @return boolean value representing whether string matches Employee's skill
 */
static bool testSkill(Employee const *emp, char const *str)
{
    if (strcmp(emp->skill, str) == 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * Helper function to test whether an Employee will be included
 * in a print output called by the program. Will return whether
 * the Employee's assignment matches the inputed string, returning true
 * if so and false if not.
 * 
 * @param emp Employee to test for matching assignment
 * @param str string to compare with as matching assignment
 * @return boolean value representing whether string matches Employee's assignment
 */
static bool testAssignment(Employee const *emp, char const *str)
{
    if (strcmp(emp->assignment, str) == 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * Helper method to find the Employee with the given id
 * in the given Database. Returns NULL if no Employee with
 * the given id exists in the Database.
 * 
 * @param id the id of the Employee to search for
 * @param database the Database to search for emploee in
 * @return the Employee with the given id
 */
static Employee *findEmployee(char *id, Database *database) {
    for (int i = 0; i < database->count; i++) {
        Employee *employee = database->list[i];
        if (strcmp(employee->id, id) == 0) {
            return employee;
        }
    }
    return NULL;
}

/**
 * Program starting point for the Temp Agency program. Inputs
 * filenames on program startup as command-line arguments (must
 * be one or more) and reads Employee information from those files.
 * Then prompts the user for a command (list, assign, unassign, quit),
 * printing the expected output, or "Invalid command" on an invalid
 * command input. Continues to prompt the user until EOF or "quit"
 * command is entered.
 * 
 * @param argc number of command-line arguments
 * @param argv list of command-line arguments
 * @return program exit status
 */
int main(int argc, char *argv[])
{
    // Check if the user entered a valid command (filename(s) given)
    if (argc <= 1) {
        fprintf(stderr, "usage: agency <employee-file>*\n");
        return EXIT_FAILURE;
    }
    
    // Make a new Database to store Employees
    Database *database = makeDatabase();

    // Read Employees from each file inputed and add to the Database
    for (int i = 1; i < argc; i++) {
        readEmployees(argv[i], database);
    }

    printf("cmd> "); // Print first command prompt
    char *commandLine = readLine(stdin); // Read a line from stdin using input.c

    while (commandLine != NULL) {
        char command[COMMAND_MAX_LENGTH + 1]; // Create string with room for max command length and null terminator
        char param1[PARAM1_MAX_LENGTH + 1]; // Create string with room for max first parameter length and null terminator
        char param2[PARAM2_MAX_LENGTH + 1]; // Create string with room for max second parameter length and null terminator
        char paramInvalid[1]; // Create a string to track an additional invalid parameter

        int matches = sscanf(commandLine, "%s%s%s%s", command, param1, param2, paramInvalid); // Read parameters from command-line

        printf("%s\n", commandLine); // Echo command to stdout

        free(commandLine);

        if (strcmp(command, "list") == 0) { // List command
            if (matches == LIST_ALL_ARGS) {
                listEmployees(database, compareID, testAll, NULL);
            } else if (matches == LIST_FILTERED_ARGS) {
                if (strcmp(param1, "skill") == 0) { // List a certain skill
                    listEmployees(database, compareID, testSkill, param2);
                } else if (strcmp(param1, "assignment") == 0) { // List a certain assignment
                    listEmployees(database, compareSkill, testAssignment, param2);
                } else { // Second paramter is not skill or assignment (not valid)
                    printf("Invalid command\n");
                }
            } else { // Number of arguments given is not valid
                printf("Invalid command\n");
            }
        } else if (strcmp(command, "assign") == 0) { // Assign command
            if (matches == ASSIGN_ARGS) {
                Employee *employee = findEmployee(param1, database);
                if (employee == NULL) {
                    printf("Invalid command\n");
                } else if (strcmp(employee->assignment, "Available") != 0) {
                    printf("Invalid command\n");
                } else {
                    strcpy(employee->assignment, param2);
                }
            } else { // Number of arguments given is not valid
                printf("Invalid command\n");
            }
        } else if (strcmp(command, "unassign") == 0) { // Unassign command
            if (matches == UNASSIGN_ARGS) {
                Employee *employee = findEmployee(param1, database);
                if (employee == NULL) {
                    printf("Invalid command\n");
                } else if (strcmp(employee->assignment, "Available") == 0) {
                    printf("Invalid command\n");
                } else {
                    strcpy(employee->assignment, "Available");
                }
            } else { // Number of arguments given is not valid
                printf("Invalid command\n");
            }
        } else if (strcmp(command, "quit") == 0) { // Quit command
            if (matches == QUIT_ARGS) {
                freeDatabase(database); // Free all contents of database
                return EXIT_SUCCESS;
            } else { // Number of arguments given is not valid
                printf("Invalid command\n");
            }
        } else { // Number of arguments given is not valid
            printf("Invalid command\n");
        }

        printf("\n"); // Line of space between commands
        printf("cmd> "); // Print next command prompt
        commandLine = readLine(stdin); // Read next line from stdin using input.c
    }
    
    freeDatabase(database);
    free(commandLine);
}