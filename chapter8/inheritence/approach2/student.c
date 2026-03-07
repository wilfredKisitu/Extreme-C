#include <stdlib.h>
#include <string.h>

// Public interface of the person class
#include "Person.h"

// Forward declaration
typedef struct
{
    char *student_number;
    unsigned int passed_credits;
    struct person_t *person;
} student_t;

// Memory Allocator
student_t *student_new()
{
    return (student_t *)malloc(sizeof(student_t));
}

// Constructor
void student_ctor(
    student_t *student,
    const char *first_name,
    const char *last_name,
    unsigned int birth_year,
    const char *student_number,
    unsigned int passed_credits)
{
    // Allocate memory for the parent object
    student->person = person_new();
    person_ctor(student->person, first_name, last_name, birth_year);
    student->student_number = (char *)malloc(16 * sizeof(char));
    strcpy(student->student_number, student_number);
    student->passed_credits = passed_credits;
}

// Destructor
void student_dtor(student_t *student)
{
    // We free the child first
    free(student->student_number);
    person_dtor(student->person);
    free(student->person);
}

// Behavior functions
void student_get_first_name(student_t *student, char *buffer)
{
    // Use person's behavior function
    person_get_first_name(student->person, buffer);
}

void student_get_last_name(student_t *student, char *buffer)
{
    person_get_last_name(student->person, buffer);
}

void student_get_student_number(student_t *student, char *buffer)
{
    strcpy(buffer, student->student_number);
}

unsigned int student_get_birth_year(student_t *student)
{
    return person_get_birth_year(student->person);
}

unsigned int student_get_passed_credits(student_t *student)
{
    return student->passed_credits;
}