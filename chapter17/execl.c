#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{

    char *evnp[] = {"A=1", "B=2", NULL};
    char *args[] = {"echo", "Hello", "World!", NULL};
    execvp("echo", args);
    printf("Failed to replace process image\n");

    return 0;
}
