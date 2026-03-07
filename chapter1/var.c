#include <stdio.h>

#define nullptr 0


int main(int argc, char const *argv[])
{
    int var = 100;
    int* p = nullptr;

    p = &var;
    *p = 200;

    *p = 800;

    printf("%i\n", *p);
    printf("%i\n", var);

    return 0;
}
