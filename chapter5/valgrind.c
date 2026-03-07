#include <stdlib.h>

int main(int argc, char const *argv[])
{
    // detection of undeallocated mem
    char* ptr = (char*)malloc(16*sizeof(char));
    free(ptr);
    return 0;
}
