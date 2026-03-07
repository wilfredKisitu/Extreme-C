#ifndef STUDENT_HEADER_2_FILE
#define STUDENT_HEADER_2_FILE

// Forward declaration
struct student_t;

// Memory Allocator
struct student_t *student_new();

// Constructor
void student_ctor(
    struct student_t *,
    const char *, /* first name */
    const char *, /* last name */
    unsigned int, /* student number */
    const char *, /* student number */
    unsigned int  /* passed credits */
);

// Destrcutor
void student_dtor(struct student_t *);

// Behavior functions
void student_get_first_name(struct student_t *, char *);
void student_get_last_name(struct student_t *, char *);
unsigned int student_get_birth_year(struct student_t *);
void student_get_student_number(struct student_t *, char *);
unsigned int student_get_passed_credits(struct student_t *);

#endif