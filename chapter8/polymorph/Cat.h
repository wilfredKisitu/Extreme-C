#ifndef CAT_HEADER_FILE
#define CAT_HEADER_FILE

// Forward declaration
struct cat_t;

// Memory allocator
struct cat_t* cat_new();

// Constructor
void cat_ctor(struct cat_t*);

// Destructor
void cat_dtor(struct cat_t*);

#endif