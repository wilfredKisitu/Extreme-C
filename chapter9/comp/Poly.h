#ifndef POLY_HEADER_FILE
#define POLY_HEADER_FILE

struct call_t;

// Memory allocator
struct call_t* call_new();

// Construct
void call_ctor(struct call_t*);

// Destructor
void call_dtor(struct call_t*);

// Virtual behavior functions
void call_fit_transform(struct call_t*);
void call_transfrom(struct call_t*);
void call_call_func(struct call_t*);
void call_forward(struct call_t*);

#endif