#ifndef DUCK_HEADER_FILE
#define DUCK_HEADER_FILE

// Forward declaration
struct duck_t;

// Memory allocator
struct duck_t* duck_new();

// Constructor
void duck_ctor(struct duck_t*);

// Destructor
void duck_dtor(struct duck_t*);

// All behavioral functions are inherited from animal class

#endif