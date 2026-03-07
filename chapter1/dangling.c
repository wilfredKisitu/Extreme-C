#include <stdio.h>
#include <stdlib.h>

int* create_an_integer(int default_value) {
    int* var_ptr = (int*)malloc(sizeof(int));
    *var_ptr = default_value;
    return var_ptr;
}

int main(int argc, char const *argv[])
{
    int* ptr = NULL;
    ptr = create_an_integer(10);
    printf("%i\n", *ptr);

    free(ptr);
    return 0;
}
