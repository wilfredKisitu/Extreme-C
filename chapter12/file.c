#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *fp = fopen("ver.c", "r");
    if (fp == NULL)
    {
        printf("Failed to open file\n");
        exit(1);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp))
    {
        printf("%s", buffer);
    }
    fclose(fp);
}
