#ifndef GUN_HEADER_FILE
#define GUN_HEADER_FILE

typedef int bool_t;

// Type forward declaration
struct gun_t;

// Memory Allocator
struct gun_t *gun_new();

// Constructor
void gun_ctor(struct gun_t *, int);

// Destuctor
void gun_dtor(struct gun_t *);

// Behavior functions
bool_t gun_has_bullets(struct gun_t *);
void gun_trigger(struct gun_t *);
void gun_refill(struct gun_t *);

#endif