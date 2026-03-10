#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>

#define SHARED_MEM_SIZE 4
#define error_check(cond, msg)                                    \
    if (cond)                                                     \
    {                                                             \
        fprintf(stderr, "ERROR: %s: %s\n", msg, strerror(errno)); \
        exit(1);                                                  \
    }

int shared_fd = -1;
int mutex_shm_fd = -1;
int32_t *counter = NULL;
pthread_mutex_t *mutex = NULL;

void init_control_mechanism()
{
    mutex_shm_fd = shm_open("/mutex0", O_CREAT | O_RDWR, 0600);
    error_check(mutex_shm_fd < 0, "Failed to created shared memory");

    // Allocate and truncate mutex memory
    error_check(
        ftruncate(mutex_shm_fd, sizeof(pthread_mutex_t)) < 0,
        "Truncation of mutex failed");

    // Map mutex memory
    void *map = mmap(0, sizeof(pthread_mutex_t), PROT_WRITE, MAP_SHARED, mutex_shm_fd, 0);
    error_check(map == MAP_FAILED, "Mapping failed");

    mutex = (pthread_mutex_t *)map;

    // Initialize mutex object
    int ret = -1;
    pthread_mutexattr_t attr;

    if ((ret = pthread_mutexattr_init(&attr)))
    {
        fprintf(stderr, "ERROR: Failed to init mutex attrs: %s\n", strerror(ret));
        exit(1);
    }
    if ((ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)))
    {
        fprintf(stderr, "ERROR: Failed to set the mutex attr: %s\n", strerror(ret));
        exit(1);
    }

    if ((ret = pthread_mutex_init(mutex, &attr)))
    {
        fprintf(stderr, "ERROR: Initializing the mutex failed: %s\n");
        exit(1);
    }
    if ((ret == pthread_mutexattr_destroy(&attr)))
    {
        fprintf(stderr, "ERROR: Failed to destroy mutex attrs: %s\n", strerror(ret));
        exit(1);
    }
}
