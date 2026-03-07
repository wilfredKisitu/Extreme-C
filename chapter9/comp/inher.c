#include <string.h>
#include <stdio.h>

typedef struct
{
    char c;
    char d;
} a_t;

typedef struct
{
    a_t parent;
    char str[15];
} b_t;

typedef struct
{
    a_t a;
    b_t b;
} c_t;

int main(int argc, char const *argv[])
{
    b_t b;
    b.parent.c = 'A';
    b.parent.d = 'B';

    strcpy(b.str, "1234");
    printf("%s\n", b.str);

    c_t c_obj;
    a_t *a_ptr = (a_t *)&c_obj;
    b_t *b_ptr = (b_t *)(&c_obj + sizeof(a_t));

    c_t *c_ptr = &c_obj;

    return 0;
}
