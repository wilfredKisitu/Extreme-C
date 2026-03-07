#include <stdio.h>
#include "extreme.h"


int main(int argc, char const *argv[])
{
    int array[5];

    // Filling array
    array[0] = 10;
    array[1] = 3;
    array[2] = 5;
    array[3] = -8;
    array[4] = 9;

    // Caluclating average using avg function
    double average = avg(array, 5, NORMAL);
    printf("The average: %f\n", average);

    average = avg(array, 5, SQUARED);
    printf("The squared average: %f\n", average);

    return 0;
}

