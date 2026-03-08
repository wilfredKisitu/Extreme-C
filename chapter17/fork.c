#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    printf("This is the parent process with the process id: %d\n",
           getpid());

    printf("Before calling fork() ...\n");

    pid_t ret = fork();
    if (ret)
    {
        printf("The child process is spawned with PID: %d\n", ret);
    }
    else
    {
        printf("This is the child process with PID: %d\n", getpid());
    }

    printf("Type CTRL+C to exit ....\n");
    while (1)
        ;
    return 0;
}
