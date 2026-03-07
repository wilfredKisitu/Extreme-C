#ifndef ANIMAL_HEADER_PRIVATE_FILE
#define ANIMAL_HEADER_PRIVATE_FILE

// morphs functional pointer
typedef void(*sound_func_t)(void*);

// Forward declaration
typedef struct {
    char* name;
    // Pointer that performs sound behavior
    sound_func_t sound_func;
} animal_t;

#endif