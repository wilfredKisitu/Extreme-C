#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    // 0 -> reading , 1-> writing

    int fds[2];
    pipe(fds);

    int child_pid = fork();

    if (child_pid == -1)
    {
        fprintf(stderr, "Fork error\n");
        exit(1);
    }

    return 0;
}
