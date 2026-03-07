#ifndef PERSON_HEADER_FILE
#define PERSON_HEADER_FILE

// Forward declaration
struct person_t;

// Memory Allocation
struct person_t* person_new();

// Constructor(ptr, first_name, last_name, birth_year)
void person_ctor(struct person_t*, const char*, 
    const char*, unsigned int);

// Destructor
void person_dtor(struct person_t*);

// Behavior functions
void person_get_first_name(struct person_t*, char*);
void person_get_last_name(struct person_t*, char*);
unsigned int person_get_birth_year(struct person_t*);

#endif