#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#define SH_SIZE 16

int main(int argc, char const *argv[])
{
    // Shared memory creation
    int shm_fd = open_shm("/shm0", O_CREAT | O_RDWR, 0600);

    if (shm_fd < 0)
    {
        fprintf(stderr, "ERROR: Failed to create shared memory: %s\n",
                sterror(errno));
        return 1;
    }
    fprintf(stdout, "Shared memory created with fd: %d\n", shm_fd);

    // Memory truncations
    if (ftruncate(shm_fd, SH_SIZE * sizeof(char) < 0))
    {
        fprintf(stderr, "ERROR: Failed to truncate memory: %s\n",
                strerror(errno));
        return 1;
    }
    // Memory mapping
    void *map = mmap(0, SH_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: Failed to map memory: %s\n",
                strerror(errno));
        return 1;
    }

    // Writing to shared memory
    char *ptr = (char *)map;
    ptr[0] = 'A';
    ptr[1] = 'B';
    ptr[2] = 'C';
    ptr[3] = '\n';
    ptr[4] = '\0';

    while (1)
        ;
    fprintf(stdout, "Data is written to shared memory\n");

    // Unmap
    if (munmap(ptr, SH_SIZE) < 0)
    {
        fprintf(stderr, "ERROR: Memory unmapping failed: %s\n", strerror(errno));
        return 1;
    }

    if (close(shm_fd) < 0)
    {
        printf(stderr, "ERROR: closing shared memory failed: %s\n",
               strerror(errno));
        return 1;
    }

    return 0;
}
