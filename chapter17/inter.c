#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t ret = fork();

    if (ret)
    {
        // Parent process code
        for (size_t i = 0; i < 5; i++)
        {
            printf("AAA\n");
            sleep(1);
        }
    }
    else
    {
        // Child process code
        for (size_t i = 0; i < 5; i++)
        {
            printf("BBBBB\n");
            sleep(1);
        }
    }
    return 0;
}
