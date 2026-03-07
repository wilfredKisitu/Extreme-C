#ifndef PERSON_HEADER_FILE_2
#define PERSON_HEADER_FILE_2

struct person_t;

// Memory Allocator
struct person_t *person_new();

// Constructor
void person_ctor(
    struct person_t *,
    const char *, /* first name */
    const char *, /* last name */
    unsigned int  /* birth year*/
);

// Destructor
void person_dtor(struct person_t *);

// Behavoir functions
void person_get_first_name(struct person_t *, char *);
void person_get_last_name(struct person_t *, char *);
unsigned int person_get_birth_year(struct person_t *);

#endif