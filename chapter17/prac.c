#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    printf("This is the parent process with the process id: %d\n", getpid());
    printf("Before calling fork()...\n");

    pid_t ret = fork();
    if (ret)
    {
        printf("The process id of created child is: %d\n", ret);
    }
    else
    {
        printf("This is the child process with process id of: %d\n", getpid());
    }

    printf("Type CRTL+C to exit...");
    while (1)
        ;

    return 0;
}
