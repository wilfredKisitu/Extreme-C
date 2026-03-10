#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>
#include <wait.h>

#define SH_SIZE 4

#define ERROR_CHECK(cond, msg)                             \
    if (cond)                                              \
    {                                                      \
        fprintf(stderr, "%s: %s\n", msg, strerror(errno)); \
        exit(1);                                           \
    }

int shared_fd = -1;
int32_t *counter = NULL;

void init_shared_resources()
{
    shared_fd = open("data.bin", O_CREAT | O_RDWR, 0600);
    ERROR_CHECK(shared_fd < 0, "ERROR: Faied to open file");
    fprintf(stdout, "Opened shared file with fd: %d\n", shared_fd);
}

void destroy_shared_resource()
{
    ERROR_CHECK(remove("data.bin") < 0, "ERROR: Failed to close shared file");
}

void inc_counter()
{
    usleep(1);
    int32_t temp = *counter;
    usleep(1);
    temp++;
    usleep(1);
    *counter = temp;
    usleep(1);
}

int main(int argc, char const *argv[])
{

    init_shared_resources();

    ERROR_CHECK(
        ftruncate(shared_fd, SH_SIZE * sizeof(char)) < 0,
        "ERROR: Failed to truncate shared resource");

    void *map = mmap(0, SH_SIZE, PROT_WRITE, MAP_SHARED, shared_fd, 0);
    ERROR_CHECK(map == MAP_FAILED, "ERROR: Failed to map shared resource");

    counter = (int32_t *)map;
    *counter = 0;

    pid_t pid = fork();

    if (pid)
    {
        // exec parent
        inc_counter();
        fprintf(stdout, "The parent process sees: %d\n", *counter);
        int status = -1;
        wait(&status);
        fprintf(stdout, "The child exited with status: %d\n", status);
    }
    else
    {
        // exec child
        inc_counter();
        fprintf(stdout, "The child process sees: %d\n", *counter);
    }

    ERROR_CHECK(munmap(counter, SH_SIZE) < 0, "ERROR: Failed to unmap shared memory");
    ERROR_CHECK(close(shared_fd) < 0, "ERROR: Failed to close shared fd");

    if (pid)
    {
        destroy_shared_resource();
    }

    return 0;
}
