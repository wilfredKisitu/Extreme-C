#include <stdlib.h>
#include <stdio.h>

typedef enum
{
    SWEET,
    SOUR
} taste_t;

typedef taste_t (*get_taste_func_t)(void *);

// Forward declaration
typedef struct
{
    get_taste_func_t get_taste_func;
} eatable_t;

// Memory allocator
eatable_t *eatable_new()
{
    return (eatable_t *)malloc(sizeof(eatable_t));
}

// Constructor
void eatable_ctor(eatable_t *eatable)
{
    eatable->get_taste_func = NULL;
}

// Destructor
void eatable_dtor(eatable_t *eatable)
{
    // Nothing to do
}

// Virtual Behavior function
taste_t eatable_get_taste_func(eatable_t *eatable)
{
    return eatable->get_taste_func(eatable);
}