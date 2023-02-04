/**
 * @file database.h
 * @author Christopher Fields (cwfields)
 * 
 * Header file for the database component of the Agency Database Management
 * system. Provides prototypes for functions to be declared in the database
 * component, including making a database, freeing a database, reading employees
 * from a file into a database, and listing employees in a database.
 */

#include <stdbool.h>

/** Maximum length of the id string */
#define ID_MAX_LENGTH 4
/** Maximum length of the firstName string */
#define FIRST_NAME_MAX_LENGTH 15
/** Maximum length of the lastName string */
#define LAST_NAME_MAX_LENGTH 15
/** Maximum length of the skill string */
#define SKILL_MAX_LENGTH 15
/** Maximum length of the assignment string */
#define ASSIGNMENT_MAX_LENGTH 20

/**
 * An Employee to be added to a database. Contains strings of varying length,
 * including an id, first name, last name, skill, and assignment. 
 */
typedef struct
{
    // Create ID string with length for ID, null terminator, and 1 additional character (for error checking)
    char id[ID_MAX_LENGTH + 1 + 1]; 
    // Create firstName string with length for firstName, null terminator, and 1 additional character (for error checking)
    char firstName[FIRST_NAME_MAX_LENGTH + 1 + 1];
    // Create lastName string with length for lastName, null terminator, and 1 additional character (for error checking)
    char lastName[LAST_NAME_MAX_LENGTH + 1 + 1];
    // Create skill string with length for skill, null terminator, and 1 additional character (for error checking)
    char skill[SKILL_MAX_LENGTH + 1 + 1];
    // Create assignment string with length for assignment and null terminator
    char assignment[ASSIGNMENT_MAX_LENGTH + 1];
} Employee;

/**
 * A Database to store Employees. Contains the actual list of (pointers to) Employees, 
 * an integer value representing the number of Employees in the list, and
 * an integer value representing the capacity of the list. 
 */
typedef struct
{
    Employee **list;
    int count;
    int capacity;
} Database;


/**
 * Dynamically allocates storage for the Database, initializes its fields,
 * and returns a pointer to the Database.
 * 
 * @return Database pointer to Database created by function
 */
Database *makeDatabase();

/**
 * Frees the memory used to store the given database, including freeing space
 * for all Employees freeing the resizable array of pointers, and freeing space
 * for the Database struct itself.
 * 
 * @param database pointer to Database to free all memory used from
 */
void freeDatabase(Database *database);

/**
 * Reads all the employees from an employee list file with the given name. Makes
 * an instance of the Employee struct for each one and stores a pointer to that
 * Employee in the resizable array in the given Database.
 * 
 * @param filename the name of the file to read Employees from
 * @param database the Database to add Employees to
 */
void readEmployees(char const *filename, Database *database);

/**
 * Sorts the Employees in the given database and then prints them. Uses the first
 * function pointer parameter to decide the order for sorting the Employees. Uses
 * the second function pointer parameter and the string parameter together to decide
 * which Employees to print. Will be used to list Employees for the list commands.
 * 
 * @param database the Database to list Employees from
 * @param compare function pointer to decide the order for sorting Employees
 * @param test function pointer to decide which Employees to print
 * @param str string to match in test function to decide what to print
 */
void listEmployees(Database *database, int (*compare)(void const *va, void const *vb), 
    bool (*test)(Employee const *emp, char const *str), char const *str);