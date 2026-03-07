#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "privatAnimal.h"
#include "Animal.h"

typedef struct
{
    animal_t animal;
} cat_t;

void __cat_sound(void *ptr)
{
    animal_t *animal = (animal_t *)ptr;
    printf("%s: Meow\n", animal->name);
}

// Memory allocator
cat_t *cat_new()
{
    return (cat_t *)malloc(sizeof(cat_t));
}

// Constructor
void cat_ctor(cat_t *cat)
{
    animal_ctor((struct animal_t *)cat);
    strcpy(cat->animal.name, "Cat");
    // Override default sound function
    cat->animal.sound_func = __cat_sound;
}

// Destructor
void cat_dtor(cat_t *cat)
{
    animal_dtor((struct animal_t *)cat);
}