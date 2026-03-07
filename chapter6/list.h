#ifndef LIST_HEADER_FILE
#define LIST_HEADER_FILE

#include <unistd.h>

struct list_t;

// Allocation function
struct list_t* list_malloc();

// Constructor and destructor
void list_init(struct list_t* );
void list_destroy(struct list_t*);

// Public behavior functions
int list_add(struct list_t*, int);
int list_get(struct list_t*, int, int*);
void list_clear(struct list_t*);
size_t list_size(struct list_t*);
void list_print(struct list_t *);

#endif