#ifndef PRIVATE_POLY_HEADER
#define PRIVATE_POLY_HEADER

// Virtual functions pointers
typedef void (*call_func_t)(void *);
typedef void (*forward_func_t)(void *);
typedef void (*fit_transform_func_t)(void *);
typedef void (*transform_func_t)(void *);

// Forward declaration
typedef struct
{
    call_func_t call_func;
    forward_func_t forward_func;
    fit_transform_func_t fit_transform_func;
    transform_func_t transform_func;
} call_t;

#endif