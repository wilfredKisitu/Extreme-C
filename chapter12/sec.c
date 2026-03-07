#include <stdio.h>
#include <string.h>
#include <stdnoreturn.h>

#define abs(x) _Generic((x), int: absi, double: absd)(x)

int absi(int a)
{
    return a > 0 ? a : -a;
}

double absd(double a)
{
    return a > 0 ? a : -a;
}

int main(int argc, char const *argv[])
{
    printf("abs(-2) :%d\n", abs(2));
    printf("abs(2.5): %f\n", abs(2.5));
    return 0;
}
