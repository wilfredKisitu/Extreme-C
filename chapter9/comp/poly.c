#include <string.h>
#include <stdlib.h>

#include "privPoly.h"

// Defualt virtual implementaion
void __forward_func(void *ptr) {}
void __call_func(void *ptr) {}
void __fit_transform_func(void *ptr) {}
void __transform_func(void *ptr) {}

// Memory allocator
call_t *call_new()
{
    return (call_t *)malloc(sizeof(call_t));
}

// Constructor
void call_ctor(call_t *call)
{
    call->call_func = __call_func;
    call->fit_transform_func = __fit_transform_func;
    call->transform_func = __transform_func;
    call->forward_func = __forward_func;
}

// Destrutor
void call_dtor(call_t *call) {}

// virutal behavior functions
void call_foward(call_t *call) { call->forward_func(call); }
void call_fit_transform(call_t *call) { call->fit_transform_func(call); }
void call_transform(call_t *call) { call->transform_func(call); }
void call_call_func(call_t *call) { call->call_func(call); }
// Non Behavior function
