#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t ret = fork();
    if (ret)
    {
        for (size_t i = 0; i < 5; i++)
        {
            printf("AAA\n");
            sleep(1);
        }
    }
    else
    {
        for (size_t i = 0; i < 5; i++)
        {
            printf("BBBBB\n");
            sleep(1);
        }
    }

    return 0;
}
