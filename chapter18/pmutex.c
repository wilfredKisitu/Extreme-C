#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/mman.h>
#include <stdint.h>

#define SHARED_MEM_SIZE 4

#define error_check(cond, msg, err_no, ret_fnc)                    \
    if (cond)                                                      \
    {                                                              \
        fprintf(stderr, "ERROR: %s: %s\n", msg, strerror(err_no)); \
        ret_fnc;                                                   \
    }

int shared_fd = -1;
int mutex_shared_fd = -1;
int32_t *counter = NULL;
pthread_mutex_t *mutex = NULL;

void init_control_mechanism()
{
    mutex_shared_fd = shm_open("/mutex0", O_CREAT | O_RDWR, 0600);
    error_check(
        mutex_shared_fd < 0,
        "Failed to create mutex shm", errno, exit(1));

    error_check(
        ftruncate(mutex_shared_fd, sizeof(pthread_mutex_t)) < 0,
        "Failed to truncated shared mutex memory", errno,
        exit(1));

    void *map = mmap(0, sizeof(pthread_mutex_t), PROT_WRITE, MAP_SHARED, mutex_shared_fd, 0);
    error_check(
        map == MAP_FAILED,
        "Failed to map mutex memory", errno, exit(1));

    mutex = (pthread_mutex_t *)map;

    int ret = -1;

    pthread_mutexattr_t attr;
    error_check(
        (ret = pthread_mutexattr_init(&attr)),
        "Failed to initialize mutex attr", ret,
        exit(1););

    error_check(
        (ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)),
        "Failed to set the mutex shared properties", ret,
        exit(1));

    error_check(
        pthread_mutex_init(mutex, &attr),
        "Failed to initialize mutex", errno, exit(1));

    error_check(
        (ret = pthread_mutexattr_destroy(&attr)),
        "Failed to destroy mutex attr", ret, exit(1));
}

void shutdown_control_mechanism()
{
    int ret = -1;
    error_check(
        (ret = pthread_mutex_destroy(mutex)),
        "Failed to destroy mutex", ret, exit(1));
    error_check(
        munmap(mutex, sizeof(pthread_mutex_t)) < 0,
        "Umapping mutex failed", errno, exit(1));
    error_check(
        close(mutex_shared_fd) < 0,
        "Closing mutex failed", errno, exit(1));
    error_check(
        shm_unlink("/mutex0") < 0,
        "Unlinking mutex failed", errno, exit(1));
}

void init_shared_resource()
{
    shared_fd = shm_open("/shm0", O_CREAT | O_RDWR, 0600);
    if (shared_fd < 0)
    {
        fprintf(stderr, "ERROR: Failed to open shared memory: %s\n", strerror(errno));
        exit(1);
    }
    fprintf(stdout, "Created shared memory with fd: %d\n", shared_fd);
}

void shutdown_shared_resource()
{
    if (shm_unlink("/shm0") < 0)
    {
        fprintf(stderr, "ERROR: Failed to unlink shared memory: %s\n", strerror(errno));
        exit(1);
    }
}
void inc_counter()
{
    usleep(1);
    pthread_mutex_lock(mutex);
    int32_t temp = *counter;
    usleep(1);
    temp++;
    usleep(1);
    *counter = temp;
    pthread_mutex_unlock(mutex);
    usleep(1);
}

int main(int argc, char const *argv[])
{
    init_shared_resource();
    init_control_mechanism();

    if (ftruncate(shared_fd, SHARED_MEM_SIZE * sizeof(char)) < 0)
    {
        fprintf(stderr, "ERROR: Failed to truncate shared resource: %s\n", strerror(errno));
        exit(1);
    }

    void *map = mmap(0, SHARED_MEM_SIZE, PROT_WRITE, MAP_SHARED, shared_fd, 0);
    if (map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: Failed to map shared resource: %s\n", strerror(errno));
        exit(1);
    }

    counter = (int32_t *)map;
    *counter = 0;

    pid_t pid = fork();
    if (pid)
    {
        inc_counter();
        fprintf(stdout, "The paranet process sees: %d\n", *counter);
        int status = -1;
        wait(&status);
        fprintf(stdout, "The child process exited with status: %d\n", status);
    }
    else
    {
        inc_counter();
        fprintf(stdout, "The child process sees: %d\n", *counter);
    }

    if (munmap(counter, SHARED_MEM_SIZE) < 0)
    {
        fprintf(stderr, "ERROR: unmapping shared memory failed: %s\n", strerror(errno));
        exit(1);
    }

    if (close(shared_fd) < 0)
    {
        fprintf(stderr, "ERROR: Closing the shared resource failed: %s\n", strerror(errno));
        exit(1);
    }

    if (pid)
    {
        shutdown_shared_resource();
        shutdown_control_mechanism();
    }

    return 0;
}
