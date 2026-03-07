#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
    #ifdef CONDITION
         int i = 0;
         i++;
         printf("%i\n", i);
    #endif
        int j = 0;
        printf("%i\n", j);

    return 0;
}
