#ifndef EATER_HEADER_FILE
#define EATER_HEADER_FILE

// Forward decalration
enum taste_t;

// Forward decalaratin
struct eatable_t;

// Constructor
void eatable_ctor(struct eatable_t*);

// Destructor
void eatable_dtor(struct eatable_t*);

// Bahavior function
taste_t eatable_get_taste_func(struct eatable_t*);


#endif