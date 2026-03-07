#include <stdio.h>

int main(int argc, char const *argv[])
{
#if __STDC_VERSION__ >= 201710L
    printf("Hello Wordl from C18\n");
#elif __STDC_VERSION__ >= 201112l
    printf("Hello World from C11!\n");

#elif __STDC_VERSION__ >= 199901l
    printf("Hello from C99!\n");
#else
    printf("Hello World from C89/C90!\n");
#endif
    return 0;
}
