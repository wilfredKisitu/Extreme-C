#include <stdio.h>


void func(int a){
    a = 5;
}

void ptr_func(int* a) {
    int b = 9;
    *a = 5;
    a = &b;
}

int main(int argc, char const *argv[])
{
    int x = 3;
    int* xptr = &x;

    printf("Value before call: %d\n", x);
    printf("Pointer before functional call: %p\n", (void*)xptr);

    ptr_func(xptr);
    printf("Value after call; %d\n", x);
    printf("Pointer value after call: %p\n", (void*)xptr);

    return 0;
}
