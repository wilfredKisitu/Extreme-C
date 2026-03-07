#include <stdio.h>

int main(int argc, char const *argv[])
{
    int var = 1;
    int* int_ptr = NULL;
    int_ptr = &var;

    char* char_ptr = NULL;
    char_ptr = (char*)&var;

    printf("Before arithmetic: int_ptr: %lu, char_ptr: %lu\n", (unsigned long) int_ptr, (unsigned long)char_ptr);

    int_ptr++;
    char_ptr++;

    printf("After arithmetic: int_ptr %lu, char_ptr: %lu\n", (unsigned long) int_ptr, (unsigned long) char_ptr);



    return 0;
}
