#include <stdio.h>
#include <stdlib.h>

// No Implementation of Person is Exposed
#include "Person.h"
#include "Student.h"

int main(int argc, char const *argv[])
{

    // Create and construct the student obj
    struct student_t *student = student_new();
    student_ctor(student, "John", "Doe", 1978, "TA5667", 134);

    // Usage of person's behavior functions
    char buffer[32];

    // Upcasting to pointer of parent type
    struct person_t *person_ptr = (struct person_t *)student;

    person_get_first_name(person_ptr, buffer);
    printf("First name: %s\n", buffer);

    person_get_last_name(person_ptr, buffer);
    printf("Last name: %s\n", buffer);

    printf("Birth year: %d\n", person_get_birth_year(person_ptr));

    // Read attributes specific to student
    student_get_student_number(student, buffer);
    printf("Student number: %s\n", buffer);

    printf("Passed credits: %d\n",
           student_get_passed_credits(student));

    student_dtor(student);
    free(student);

    return 0;
}
