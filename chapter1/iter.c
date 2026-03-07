#include <stdio.h>

#define SIZE  5


int main(int argc, char const *argv[])
{
    int arr[SIZE];

    arr[0] = 9;
    arr[1] = 22;
    arr[2] = 30;
    arr[3] = 23;
    arr[4] = 18;

    int* int_ptr = &arr[0];

    for(int i  = 0; i < SIZE; i++){
        printf("%i\n", arr[i]);
    }

    for(;;) {
        printf("%d\n", *int_ptr);
        if(int_ptr == &arr[SIZE -1]) {
            break;
        }
        int_ptr++;
    }

    return 0;
}
