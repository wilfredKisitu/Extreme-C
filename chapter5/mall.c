#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SIZE 16

void print_bytes(char* ptr, int size) {
    for(int i = 0; i < size; i++){
        printf("0x%02x ", (unsigned char)ptr[i]);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{

    char* ptr = (char *)malloc(SIZE * sizeof(char));
    memset(ptr, 0, SIZE * sizeof(char));

    print_bytes(ptr, SIZE);

    char* ptr2= (char*) malloc(SIZE * sizeof(char));
    ptr2 = (char*)realloc(ptr2, 2 * SIZE * sizeof(char));
    print_bytes(ptr2, SIZE * 2);

    free(ptr);
    free(ptr2);

    return 0;
}
