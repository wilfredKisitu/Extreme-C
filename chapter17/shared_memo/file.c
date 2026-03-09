#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <wait.h>
#include <string.h>

#define SH_SIZE 16

#define ERROR_CHECK(cond, std_output, msg, error_str)    \
    if (cond)                                            \
    {                                                    \
        fprintf(std_output, "%s: %s\n", msg, error_str); \
        exit(1);                                         \
    }

int shared_fd = -1;
int32_t *counter = NULL;

void init_shared_memo()
{
    shared_fd = open("data.bin", O_CREAT | O_RDWR, 0600);
    ERROR_CHECK(
        shared_fd < 0,
        stderr,
        "ERROR: Failed to created file",
        strerror(errno));
    fprintf(stdout, "Created share File with fd: %d\n", shared_fd);
}

void shared_destroy()
{
    ERROR_CHECK(
        remove("data.bin") < 0,
        stderr,
        "Failed to remove shared resource",
        strerror(errno));
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
    init_shared_memo();

    ERROR_CHECK(
        ftruncate(shared_fd, SH_SIZE * sizeof(char)) < 0,
        stderr,
        "ERROR: Failed to truncate shared file",
        strerror(errno));

    void *map = mmap(0, SH_SIZE, PROT_WRITE, MAP_SHARED, shared_fd, 0);
    ERROR_CHECK(
        map == MAP_FAILED,
        stderr,
        "ERROR: Failed to map shared file",
        strerror(errno));

    counter = (int32_t *)map;
    *counter = 0;

    pid_t pid = fork();

    if (pid)
    {
        inc_counter();
        fprintf(stdout, "The parent process sees %d\n", *counter);
        int status = -1;
        wait(&status);
        fprintf(stdout, "The child process exited with a status: %d\n", status);
    }
    else
    {
        inc_counter();
        fprintf(stdout, "The child process sees %d\n", *counter);
    }

    ERROR_CHECK(
        munmap(counter, SH_SIZE) < 0,
        stderr,
        "ERROR: Failed to unmap shared file",
        strerror(errno));

    ERROR_CHECK(
        close(shared_fd) < 0,
        stderr,
        "ERROR: Failed to close shared file",
        strerror(errno));

    if (pid)
    {
        shared_destroy();
    }

    return 0;
}
