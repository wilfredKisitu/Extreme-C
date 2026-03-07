#include <stdio.h>

struct  sample_t
{
    char first;
    char second;
    char third;
    short fourth;
};

void print_size(struct sample_t* var) {
    printf("Size: %lu\n", sizeof(*var));
}

void print_bytes(struct sample_t* v) {
    unsigned char* ptr = (unsigned char*)v;

    for(int i = 0; i < sizeof(*v); i++, ptr++){
        printf("%d ", (unsigned int)*ptr);
    }
    printf("\n");
}



int main(int argc, char const *argv[])
{
    struct sample_t var;
    var.first = 'Z';
    var.second = 'G';
    var.third = 'E';
    var.fourth = 765;

    print_size(&var);
    print_bytes(&var);

    return 0;
}

