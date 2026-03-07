#include <stdio.h>
#include <stdlib.h>

void print_mem_map() {
    #ifdef __linux__
    FILE* fd = fopen("/proc/self/maps", "r");

    if(!fd) {
        printf("Could not read memory maps of file. \n");
    }

    char line[1024];
    while (!feof(fd))
    {
        fgets(line, 1024, fd);
        printf("> %s\n", line);
    }

    fclose(fd);
    #endif
}

int main(int argc, char const *argv[])
{   
    // Allocation of 10 bytes without initialization
    char* ptr = (char*) malloc(10 * sizeof(char));
    printf("Adress of ptr: %p\n", (void*)&ptr);
    printf("Memory allocated by malloc %p: ", (void*)ptr);
    for(int i = 0; i < 10; i++) {
        printf("0x%02x ", (unsigned char)ptr[i]);
    }
    printf("\n");
    
    // Allocation of 10 bytes all initialized to zero 
    char* ptr2 = (char*)calloc(10, sizeof(char));
    printf("Adress of ptr2: %p\n", (void*)&ptr2);
    printf("Memory allocated by calloc at %p: ", (void*)ptr2);
    for(int i = 0; i < 10; i++){
        printf("0x%02x ", (unsigned char)ptr2[i]);
    }
    printf("\n");

    print_mem_map();

    free(ptr);
    free(ptr2);

    return 0;
}
