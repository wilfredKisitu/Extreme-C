#include <stdio.h>
#include <stdlib.h>

#include "Student.h"

int main(int argc, char const *argv[])
{
    // Create and construct student object
    struct student_t *student = student_new();

    student_ctor(student, "John", "Doe", 1987, "TA5667", 134);

    // We need to use student behavior functions
    // i.e student ptr is not person_ptr

    char buffer[32];
    student_get_first_name(student, buffer);
    printf("First name: %s\n", buffer);

    student_get_last_name(student, buffer);
    printf("Last name: %s\n", buffer);

    printf("Birth year: %d\n", student_get_birth_year(student));

    student_get_student_number(student, buffer);
    printf("Student number: %s\n", buffer);
    printf("Passed credits: %d\n",
           student_get_passed_credits(student));

    // Destuct and free the student object
    student_dtor(student);
    free(student);

    return 0;
}
