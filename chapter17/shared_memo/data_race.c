#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>

#define SH_SIZE 4

int shared_fd = -1;
int32_t *counter = NULL; /* Pointer to shared counter */

void init_shared_memory()
{
    shared_fd = shm_open("/shm0", O_CREAT | O_RDWR, 0600);
    if (shared_fd < 0)
    {
        fprintf(stderr, "ERROR: Failed to create shared memory: %s\n",
                strerror(errno));
        exit(1);
    }
    fprintf(stdout, "Shared memory is created with fd: %d\n",
            shared_fd);
}

void shutdown_shared_resource()
{
    if (shm_unlink("/shm0") < 0)
    {
        fprintf(stderr, "ERROR: Unlinking shared memory failed: %s\n",
                strerror(errno));
        exit(1);
    }
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
    // Paraent pocess needs to intialiaze the share resource
    init_shared_memory();

    // Allocate and truncate the shared memory
    if (ftruncate(shared_fd, SH_SIZE * sizeof(char)) < 0)
    {
        fprintf(stderr, "ERROR: Truncation failed %s\n",
                strerror(errno));
        return 1;
    }
    fprintf(stdout, "The memory region is truncated\n");

    // Map the shared memory and initialize the counter
    void *map = mmap(0, SH_SIZE, PROT_WRITE, MAP_SHARED, shared_fd, 0);
    if (map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: Mapping failed: %s\n",
                strerror(errno));
        return 1;
    }

    counter = (int32_t *)map;
    *counter = 0;

    // For k a new process
    pid_t pid = fork();
    if (pid)
    {
        // The parent process increments
        inc_counter();
        fprintf(stdout, "The parent process sees the counter as %d\n", *counter);

        // Wait for child process to exit
        int status = -1;
        wait(&status);
        fprintf(stdout, "The child process finished with status %d\n", status);
    }
    else
    {
        // The child process increments
        inc_counter();
        fprintf(stdout, "The child process sees the counter as %d\n", *counter);
    }

    // Both processes should unmap the shared memory region
    // and close its file descriptor
    if (munmap(counter, SH_SIZE) < 0)
    {
        fprintf(stderr, "ERORR: Umapping failed: %s\n",
                strerror(errno));
        return 1;
    }

    if (close(shared_fd) < 0)
    {
        fprintf(stderr, "ERROR: Closing shared memory fd failed: %s\n",
                strerror(errno));
        return 1;
    }

    // Only parent process needs to shutdown the shared resource
    if (pid)
    {
        shutdown_shared_resource();
    }

    return 0;
}
