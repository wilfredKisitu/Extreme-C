#include <stdlib.h>
#include "privPoly.h"

typedef struct
{
    call_t call;
} child_t;

void __child_call_func(void *prt) {}
void __child_forward_func(void *ptr) {}
void __child_fit_transform_func(void *ptr) {}
void __child_transform_func(void *ptr) {}

// Memory alloctor
child_t *child_new()
{
    return (child_t *)malloc(sizeof(child_t));
}

// Constructor
void child_ctor(child_t *child)
{
    child->call.call_func = __child_call_func;
    child->call.fit_transform_func = __child_fit_transform_func;
    child->call.forward_func = __child_forward_func;
    child->call.transform_func = __child_transform_func;
}

// Destructor
void child_dtor(child_t *child) { /* Nothing to do */ }
