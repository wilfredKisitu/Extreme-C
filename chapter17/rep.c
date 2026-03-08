#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    char *args[] = {"echo", "Hello", "World!", 0};
    execvp("echo", args);
    printf("execvp() failed. Erro: %s\n", strerror(errno));
    return 0;
}
